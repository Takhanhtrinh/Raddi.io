#include "HelloWorldScene.h"
#include <iostream>
#include <err.h>
#include <errno.h>
#include <stdint.h>
#include "Common.hpp"
#include <string.h>
#include <arpa/inet.h>
#include <algorithm>
#include "SimpleAudioEngine.h"
#include "PacketFactory.hpp"
#include "BitReader.hpp"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if( !Layer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    struct sockaddr_in sin;
    
    char *ip = (char*)"192.168.1.40";
//    sin.sin_addr.s_addr = inet_addr(ip);
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(5454);
    sin.sin_family = AF_INET;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd < 0)
        err(1,"sockfd init\n");
    
    if ((connect(sockfd,(struct sockaddr*)&sin,sizeof(sin))) < 0){
        err(1, "can not connect\n");
    }
    sendFirstInit();
    m_delay = 50;
    char buf[19];
    readCount =0;
    int index =0;
    int len =0;
    bool gameInit = false;
   
    
        
        if ((len = read(sockfd,buf,19)) < 0) {
            warn("read");
            free(buf);
            return false;
        }
        std::cout << " len " << len << std::endl;
        while (index < len ) {
            uint8_t type = readUint8((unsigned char*)buf, index);
            //        int type = 1;
            index++;
            switch(type){
                    
                case INIT_PACKET:
                {
                    
                    uint16_t mapsize = readUint16((unsigned char*)buf, index);
                    index+=2;
                    uint16_t centerX = readUint16((unsigned char*)buf, index);
                    index+=2;
                    uint16_t centerY = readUint16((unsigned char*)buf, index);
                    index+=2;
                    uint16_t smallCircle = readUint16((unsigned char*)buf, index);
                    index+=2;
                    uint16_t movespeed = readUint16((unsigned char*)buf, index);
                    index+=2;
                    uint8_t playerradius = readUint8((unsigned char*)buf, index);
                    index++;
                    uint8_t foodRadius = readUint8((unsigned char*)buf, index);
                    index++;
                    uint8_t bulletRadius = readUint8((unsigned char*)buf, index);
                    index++;
                    uint8_t itemRadius = readUint8((unsigned char*)buf, index);
                    index++;
                                        std::cout << "mapsize: " << mapsize << "  centerX:" << centerX << "  centerY: " << centerY << "  smallCircle: " << smallCircle << " Move spped:  " << movespeed << "  playerRadius: " << (unsigned)playerradius << "  foodRWadius: " << (unsigned)foodRadius << "  bullet radius: " << (unsigned)bulletRadius << " itemRadius:  " << (unsigned)itemRadius <<  std::endl;
                    if (mapsize && centerX && centerY && smallCircle && movespeed && playerradius &&foodRadius &&bulletRadius && itemRadius  ){
                        gameInit = true;
                        
                        //   GameWorld(int mapSize, float centerX, float centerY, int smallCircle,int moveSpeed, int playerRadius,int foodRadius,int bulletRadius, int ItemRadius);
                        m_game = std::unique_ptr<GameWorld>(new GameWorld(mapsize,centerX, centerY,smallCircle,movespeed,playerradius,foodRadius,bulletRadius,itemRadius));
                       
                        if ((setNonBlock(sockfd)) < 0)
                            err(1,"set non block failed");
                        
                     
                    }
                    
                }
                    break;
            }
        }
        index =0;
     

    
   
   

    
    gettimeofday(&m_lastMessage, NULL);
    time = 100;
    base = event_base_new();
    read_ev = event_new(base,sockfd,EV_READ, invoke_cb_read, this);
    event_add(read_ev,NULL);
    
    
    drawCirlcle = cocos2d::DrawNode::create();
    drawFood = cocos2d::DrawNode::create();
  
    controller = Controller::createController();
    controller->addEvent();
    if (!drawCirlcle) return false;
    if (!drawFood) return false;
//    drawCirlcle->drawCircle(m_game->getCenter(), m_game->getMapSize(), 360, 50, false, 1, 1, cocos2d::Color4F::BLACK);
   
    drawCirlcle->drawSolidCircle(m_game->getCenter(), m_game->getMapSize() + 10, 360, 60, 1, 1, cocos2d::Color4F::BLACK);
      drawCirlcle->drawSolidCircle(m_game->getCenter(), m_game->getMapSize(), 360, 60, 1, 1, cocos2d::Color4F::GRAY);
        drawCirlcle->drawSolidCircle(m_game->getCenter(), m_game->getSmallCircle(), 360, 60, 1, 1, cocos2d::Color4F::BLACK);
    drawCirlcle->drawSolidCircle(m_game->getCenter(), m_game->getSmallCircle()- 10, 360, 60, 1, 1, cocos2d::Color4F::WHITE);
//    drawCirlcle->drawSolidCircle(m_game->getCenter(), m_game->getMapSize(), 360, 100, 1, 1, cocos2d::Color4F::GRAY);
    


    auto listener = cocos2d::EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    ui::Button *shootButton =ui::Button::create("res/ShootButton.png", "res/ShootButton.png");
    shootButton->setPosition(Vec2(visibleSize.width - 150, 150));
 
    this->addChild(shootButton);
    
    this->schedule(schedule_selector(HelloWorld::sendCommand), 2/60.f);
    this->schedule(schedule_selector(HelloWorld::sendPing), 0.25f);

   auto camera = Camera::create();
    camera->initDefault();

     this->addChild(camera);
//    this->addChild(m_game->getPlayer());

    this->addChild(controller);
    this->addChild(drawCirlcle,-1);
    
  
    this->addChild(drawFood,1);
//    this->setCameraMask((unsigned short) CameraFlag::DEFAULT,true);
      camera->setCameraFlag( CameraFlag::USER1);
    
    shootButton->setCameraMask((unsigned short) CameraFlag::USER1);

    shootButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::touchEvent, this));

 

   
    Director::getInstance()->setClearColor(Color4F::WHITE);
    
  
    

    this->scheduleUpdate();
    
        gettimeofday(&m_start, NULL);
    std::cout << "time start:   "  << m_start.tv_usec / 1000;
 

    
    return true;
}
void HelloWorld::update(float dt ){

  event_base_loop(base,EVLOOP_NONBLOCK);
   
    storeCommand();
    m_game->update(dt);
   
//    drawFood->clear();
//
//    m_game->drawFood(drawFood);
   
    auto cam = Director::getInstance()->getRunningScene()->getDefaultCamera();
    if (cam && m_game->getPlayer()){
        cam->setPosition(m_game->getPlayer()->getPosition());
        cam->setScale(3, 3);
        
    }
    
//    camera->setPosition(m_game->getPlayer()->getPosition());
  

}
int HelloWorld:: setNonBlock(int fd) {
    int flags;
    flags = fcntl(fd,F_GETFL);
    if (flags < 0) return flags;
    flags |= O_NONBLOCK;
    if ((fcntl(fd,F_SETFL,flags)) < 0) {
        return -1;
    }
    return 0;
}
void HelloWorld:: invoke_cb_read(int fd, short what, void *agr){
    (static_cast<HelloWorld*>(agr))->cb_read(fd,what);
}
void HelloWorld:: cb_read(int fd, short what){
      char MessageType[1];
    int len;
    while(true) {
        if (!getQueueBuffer(fd)){
            event_add(read_ev,NULL);
            return;
        }
        len = read(fd,MessageType,1);
        if (len < 0) {
//            GAMELOG("read", "len < 0");
            break;
        }
    
    switch (MessageType[0]) {
        case MOVE_PACKET:
            
        {
           
            
            char buf[MOVE_PACKET_SIZE - 1];
            int len;
            if ((len = read(fd,buf,MOVE_PACKET_SIZE - 1)) < 0 ) {
                m_buf.create(MessageType[0], MOVE_PACKET_SIZE - 1);
                return;
            }
            if (len < MOVE_PACKET_SIZE - 1) {
                m_buf.create(MessageType[0], MOVE_PACKET_SIZE - 1);
                m_buf.append(buf, len);
            }
            else if (len == MOVE_PACKET_SIZE - 1) {
                messageParse(MessageType[0], buf);
            }
        }
            
            break;
        case MOVE_PACKET_WITH_HEALTH:
        {
            char buf[MOVE_PACKET_WITH_HEALTH_SIZE - 1];
            int len;
            if ((len = read(fd,buf,MOVE_PACKET_WITH_HEALTH_SIZE - 1)) < 0 ) {
                m_buf.create(MessageType[0], MOVE_PACKET_WITH_HEALTH_SIZE - 1);
                return;
            }
            if (len < MOVE_PACKET_WITH_HEALTH_SIZE - 1) {
                m_buf.create(MessageType[0], MOVE_PACKET_WITH_HEALTH_SIZE - 1);
                m_buf.append(buf, len);
            }
            else if (len == MOVE_PACKET_WITH_HEALTH_SIZE - 1) {
                messageParse(MessageType[0], buf);
            }

        }
            break;
        case NEW_PLAYER_PACKET:
        {
            int name_len= 0;
            char nameLength[1];
            if ((ReadAll(fd, nameLength, 1)) == 1) {
            uint8_t playerNameLength = readUint8((unsigned char*)nameLength, 0);
            
            int dataLen =0;
            
            char buf[playerNameLength + NEW_PLAYER_PACKET_SIZE - 1];
                if ((ReadAll(fd, buf, NEW_PLAYER_PACKET_SIZE - 1)) == 1) {
                uint8_t id = readUint8((unsigned char* ) buf, 0);
                uint16_t x = readUint16((unsigned char* )buf, 1);
                uint16_t y = readUint16((unsigned char*) buf, 3);
                char *name = readString((unsigned char*) buf, name_len, 5);
//                printf("my position x: %.1f y: %.1f\n", m_game->getPlayer()->getPositionX(),m_game->getPlayer()->getPositionY());
                printf("new player in range with name %s at x: %d y: %d\n", name,x,y);
                
                m_game->insertPlayer(x, y, id,this);
                }
            }
            
            
        }
            break;
        case UPDATE_NODE_PACKET:
        {
            int datalen = 0;
            char buf[UPDATE_NODE_SIZE - 1];
            if ((datalen = read(fd, buf,UPDATE_NODE_SIZE - 1)) < 0 ) {
                 m_buf.create(MessageType[0], UPDATE_NODE_SIZE - 1);
                return;
            }
            if (datalen < UPDATE_NODE_SIZE - 1) {
                m_buf.create(MessageType[0], UPDATE_NODE_SIZE - 1);
                m_buf.append(buf, datalen);
            }
            else if (datalen == UPDATE_NODE_SIZE - 1) {
                messageParse(MessageType[0], buf);
            }

            
        }
            break;
        
        case PING_PACKET:
        {
            struct timeval ping;
            gettimeofday(&ping, NULL);
            int temp = (ping.tv_usec - m_lastPing.tv_usec) /1000;
            if (temp > 0)
                timePing =temp / 2;
            
            

        }
            break;
        case FOOD_PACKET:
        {
            int datalen=0;
            char buf[FOOD_PACKET_SIZE -  1];
            if ((datalen  = read(fd, buf, FOOD_PACKET_SIZE - 1)) < 0 ) {
                 m_buf.create(MessageType[0], FOOD_PACKET_SIZE - 1);
                return;
            }
            if (datalen < FOOD_PACKET_SIZE -1 ) {
               
                m_buf.create(MessageType[0], FOOD_PACKET_SIZE - 1);
                m_buf.append(buf, datalen);
            }
            else if (datalen == FOOD_PACKET_SIZE - 1)
                messageParse(MessageType[0], buf);
            
        }
            break;
        case FOOD_RM_PACKET:
        {
           
            
            int datalen=0;
            char buf[ FOOD_RM_PACKET_SIZE -  1];
            if ((datalen  = read(fd, buf,  FOOD_RM_PACKET_SIZE - 1)) < 0 ) {
                       m_buf.create(MessageType[0], FOOD_RM_PACKET_SIZE - 1);
                return;
            }
            if (datalen < FOOD_RM_PACKET_SIZE -1 ) {
                m_buf.create(MessageType[0], FOOD_RM_PACKET_SIZE - 1);
                m_buf.append(buf, datalen);

            }
            else if (datalen ==  FOOD_RM_PACKET_SIZE - 1)
                messageParse(MessageType[0], buf);
        }
             break;
        case PLAYER_RM_PACKET:
        {
              int datalen=0;
            char buf[PLAYER_RM_PACKET_SIZE -  1];
            if ((datalen  = read(fd, buf, PLAYER_RM_PACKET_SIZE - 1)) < 0 ) {
                m_buf.create(MessageType[0], PLAYER_RM_PACKET_SIZE - 1);
                return;
            }
            if (datalen < PLAYER_RM_PACKET_SIZE -1 ) {
                m_buf.create(MessageType[0], PLAYER_RM_PACKET_SIZE - 1);
                m_buf.append(buf, datalen);
            }
            else if (datalen == PLAYER_RM_PACKET_SIZE - 1)
                messageParse(MessageType[0], buf);
        }
            
            break;
        case BULLET_PACKET:
        {
            
            int datalen=0;
            char buf[BULLET_PACKET_SIZE -  1];
            if ((datalen  = read(fd, buf, BULLET_PACKET_SIZE - 1)) < 0 ) {
                m_buf.create(MessageType[0], BULLET_PACKET_SIZE - 1);
                return;
            }
            if (datalen < BULLET_PACKET_SIZE -1 ) {
                m_buf.create(MessageType[0], BULLET_PACKET_SIZE - 1);
                m_buf.append(buf, datalen);
            }
            else if (datalen == BULLET_PACKET_SIZE - 1)
                messageParse(MessageType[0], buf);
        }
            break;
        case UPDATE_BULLET_PACKET:
        {
            
            int datalen=0;
            char buf[UPDATE_BULLET_PACKET_SIZE -  1];
            if ((datalen  = read(fd, buf, UPDATE_BULLET_PACKET_SIZE - 1)) < 0 ) {
                  m_buf.create(MessageType[0], UPDATE_BULLET_PACKET_SIZE - 1);
                return;
            }
            if (datalen < UPDATE_BULLET_PACKET_SIZE -1 ) {
                m_buf.create(MessageType[0], UPDATE_BULLET_PACKET_SIZE - 1);
                m_buf.append(buf, datalen);
            }
            else if (datalen == UPDATE_BULLET_PACKET_SIZE - 1)
                messageParse(MessageType[0], buf);
        }
            break;
        case REMOVE_BULLET_PACKET:
        {
            
           
            int datalen=0;
            char buf[REMOVE_BULLET_PACKET_SIZE -  1];
            if ((datalen  = read(fd, buf, REMOVE_BULLET_PACKET_SIZE - 1)) < 0 ) {
                 m_buf.create(MessageType[0], REMOVE_BULLET_PACKET_SIZE - 1);
                return;
            }
            if (datalen < REMOVE_BULLET_PACKET_SIZE -1 ) {
    
                m_buf.create(MessageType[0], REMOVE_BULLET_PACKET_SIZE - 1);
                m_buf.append(buf, datalen);
                
               
            }
            else if (datalen == REMOVE_BULLET_PACKET_SIZE - 1)
                messageParse(MessageType[0], buf);
        }
              break;
        default:
            break;
    }
    }
    event_add(read_ev,NULL);
    
    

}
void HelloWorld:: invoke_cb_write(int fd, short what, void *agr){
    (static_cast<HelloWorld*> (agr))->cb_write(fd, what);
}
void HelloWorld:: cb_write(int fd, short what){
    
}
void HelloWorld:: sendCommand(float dt) {
    if (m_sendingCommand.size() > 0)
    {
        char buf[m_sendingCommand.size() * 4 + 2];
        writeUint8((unsigned char*) buf, CL_ANGLE_MSG, 0);
        writeUint8((unsigned char*) buf, m_sendingCommand.size(), 1);
        for (int i =0; i < m_sendingCommand.size();i++) {
            FloatInt angle;
            angle.float_value = m_sendingCommand.at(i).angle;
//        std::cout <<"angle  " <<  angle.float_value<< std::endl;
            writeUint32((unsigned char*) buf, angle.int_value, 2 + i * 4);
    
        }
        int len =0;
        if ((len = write(sockfd,&buf,m_sendingCommand.size() * 4 + 2)) < 0) {
        warn("write faile");
        return;
    }
        
    m_sendingCommand.clear();
    }
    
}
void HelloWorld::messageParse(char type,char *buf) {
    switch (type) {
        case MOVE_PACKET:
        {
            struct timeval diff;
            gettimeofday(&diff, NULL);
            int temp = (diff.tv_usec - m_lastMessage.tv_usec) / 1000;
            
            if (temp >0){
                
                time = temp;
            }
            m_lastMessage = diff;
            //            std::cout << "time " << time << std::endl;
            uint16_t x = readUint16((unsigned char*)buf, 0);
            uint16_t y = readUint16((unsigned char*)buf, 2);
            float angle = readUint32((unsigned char*) buf, 4);
            if (!m_game->getPlayer()){
        
                m_game->createPlayer(x,y, angle, -1,this);
            }
            else{
                m_game->getPlayer()->insertPos(x, y, time);
                m_game->getPlayer()->setHeadingAngle(angle);
            }
          
        }
            break;
        case MOVE_PACKET_WITH_HEALTH:
        {
            struct timeval diff;
            gettimeofday(&diff, NULL);
            int temp = (diff.tv_usec - m_lastMessage.tv_usec) / 1000;
            
            if (temp >0){
                
                time = temp;
            }
            m_lastMessage = diff;
            //            std::cout << "time " << time << std::endl;
            uint16_t x = readUint16((unsigned char*)buf, 0);
            uint16_t y = readUint16((unsigned char*)buf, 2);
            uint8_t health = readUint8((unsigned char* ) buf, 4);
            float angle = readUint32((unsigned char*) buf, 5);
            printf("my health %d\n", health);
            if (!m_game->getPlayer()){
                
                m_game->createPlayer(x,y, angle, -1,this);
            }
            else{
                m_game->getPlayer()->insertPos(x, y, time);
                m_game->getPlayer()->setHeadingAngle(angle);
            }

        }
        case NEW_PLAYER_PACKET:
        {
            
        }
            break;
        case UPDATE_NODE_PACKET:
        {
            uint8_t id = readUint8((unsigned char*) buf, 0);
            uint16_t x = readUint16((unsigned char*) buf, 1);
            uint16_t y = readUint16((unsigned char*) buf, 3);
            m_game->updateOtherPos(id, x, y, time,this);
        }
            break;
        case FOOD_PACKET:
        {
            uint16_t id = readUint16((unsigned char*)buf,0 );
            uint8_t type = readUint8((unsigned char*) buf, 2);
            uint16_t x= readUint16((unsigned char*)buf, 3);
            uint16_t y = readUint16((unsigned char*)buf, 5);
            m_game->insertFood(x, y, type, id,this);

        }
            break;
        case FOOD_RM_PACKET:
        {
            uint16_t id = readUint16((unsigned char*)buf, 0);
            //                printf("delete node with id: %d\n", id);
            m_game->deleteNode(id,this);
        }
            break;
        case PLAYER_RM_PACKET:
        {
            
            uint8_t id = readUint8((unsigned char*)buf, 0);
            printf("delete  a player with id: %d \n",id);
            m_game->deletePlayer(id,this);
        }
            break;
        case BULLET_PACKET:
        {
            uint16_t id = readUint16((unsigned char*) buf, 0);
            uint16_t x = readUint16((unsigned char*) buf, 2);
            uint16_t y = readUint16((unsigned char*) buf, 4);
            m_game->insertBullet(x, y, id,this);
            printf("new Bullet id: %d x: %d y: %d" , id , x, y);

        }
            break;
        case UPDATE_BULLET_PACKET:
        {
            uint16_t id = readUint16((unsigned char*) buf, 0);
            uint16_t x = readUint16((unsigned char*) buf, 2);
            uint16_t y = readUint16((unsigned char*) buf, 4);
            m_game->updateBullet(x,y,time, id,this);
        }
            break;
        case REMOVE_BULLET_PACKET:
        {
            uint16_t id = readUint16((unsigned char*) buf, 0);
            
            printf("remove bullet out of range %d\n", id);
            m_game->removeBullet(id,this);

        }
            break;
        default:
            break;
    }
}
bool HelloWorld::getQueueBuffer(int fd){
    if (m_buf.getType() != 0) {
        char buf[m_buf.getSizeLeft()];
        int len = read(fd,buf,m_buf.getSizeLeft());
        if (len < 0 ){
            printf("len < 0");
            return false;
        }
        m_buf.append(buf, len);
        if (m_buf.isFull()) {
            messageParse(m_buf.getType(),m_buf.getBuf());
            m_buf.reset();
            return true;
        }
        else
            return false;
        
    }
    return true;

}
void HelloWorld::storeCommand() {
    Command c;
    if (m_game->getPlayer()){
    if (controller->getChaged()) {
    float angle = controller->getAngle();
    c.x =(uint16_t)m_game->getPlayer()->getPositionX();
    c.y =(uint16_t)m_game->getPlayer()->getPositionY();
    c.angle = angle;
    m_sendingCommand.push_back(c);
    }
    }
    m_currentCommand.push_back(c);
//    m_game->getPlayer()->setAngle(angle);
    
}
void HelloWorld:: sendFirstInit(){
    char name[1];
//    scanf("%20s", name);
//    printf("string len: %d ", strlen(name));
    name[0] = ' ';
    name[1] = '\0';
    char buf[21];
    buf[0] = CL_INIT_MSG;
    memcpy(buf + 1, name, strlen(name));
//    std::cout << buf << std::endl;
    int len;
    if ((len = write(sockfd,buf,strlen(buf))) < 0)
    {
        err(1,"Write failed");
    }
    std::cout << "send success len: " << len <<  std::endl;
}
void HelloWorld::sendPing(float dt){
    Packet_Ping packet;
    BitWriter wr(packet.getSize());
    packet.serialize(wr);
    if ((write(sockfd,wr.getBuf(),wr.getLen())) < 0) {
        printf("send Ping < 0\n");
        return;
    }
    gettimeofday(&m_lastPing, NULL);
}
void HelloWorld:: onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event){
    switch(keyCode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
        {
            char buf[2];
            buf[0] = CL_BUTTON_MSG;
            buf[1] = SHOOT_BUTTON;
            if (write(sockfd,buf,2) < 0) {
                printf("send shoot failed \n");
                return;
            }
        }
            break;
        case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW: {
            char buf[2];
            buf[0] = CL_BUTTON_MSG;
            buf[1] = SPEED_BUTTON;
            if (write(sockfd,buf,2) < 0) {
                printf("send shoot failed \n");
                return;
            }

        }
            break;
       
    }
}
void HelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event){
    switch(keyCode) {
        case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            break;
            
            
    }
}
void HelloWorld::touchEvent(cocos2d:: Ref *sender, cocos2d::ui::Widget::TouchEventType type){
    switch(type){
        case ui::Widget::TouchEventType::BEGAN:
            printf("just touch\n");
            break;
        case ui::Widget::TouchEventType::ENDED:{
            char buf[2];
            buf[0] = CL_BUTTON_MSG;
            buf[1] = SHOOT_BUTTON;
            if (write(sockfd,buf,2) < 0) {
                printf("send shoot failed \n");
                return;
            }
        }
            break;
    }
}
