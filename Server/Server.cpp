
#include "Server.h"
#include "protocol/common.h"
#include <algorithm>
#include <chrono>
#include "protocol/BitPack.h"
#include "protocol/Packet.h"
#include "protocol/PacketManager.h"
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <math.h>
#include <assert.h>

GameServer::GameServer(short port) : m_port(port),
                                     playerCount(0)
{
    m_base = event_base_new();
    w_base = event_base_new();
    m_loop = event_base_new();
    m_handle.event = NULL;
    m_handle.five_second.tv_sec = 5;
    m_handle.five_second.tv_usec = 0;
    m_grid = Grid::createGrid(MAPSIZE, MAPSIZE, CELLSIZE);
};
int GameServer::setNonBlock(int fd)
{
    int flags;
    flags = fcntl(fd, F_GETFL);
    if (flags < 0)
        return flags;
    flags |= O_NONBLOCK;
    if ((fcntl(fd, F_SETFL, flags)) < 0)
    {
        return -1;
    }
    return 0;
}
GameServer::~GameServer()
{
    event_base_free(w_base);
    event_base_free(m_base);
    event_base_free(m_loop);
}
void GameServer::invoke_cb_init(int fd, short what, void *agr)
{
    // std::cout << "called" << std::endl;
    (static_cast<GameServer *>(agr))->cb_init(fd, what);
}
void GameServer::invoke_cb_accept(int fd, short what, void *agr)
{
    (static_cast<GameServer *>(agr))->cb_accept(fd, what);
}
void GameServer::invoke_cb_read(int fd, short what, void *agr)
{
    // std::cout << "called " << std::endl;
    (static_cast<GameServer *>(agr))->cb_read(fd, what);
}
void GameServer::invoke_cb_write(int fd, short what, void *agr)
{
    (static_cast<GameServer *>(agr))->cb_write(fd, what);
}

void GameServer::run()
{
    struct sockaddr_in sin;
    int sockfd;
    char *addr = (char *)"192.168.1.40";
    struct event *listen_event;
    int reuseaddr = 1;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        err(1, "socket init");
    }
    if ((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int))) < 0)
        err(1, "fail to setsockopt");
    int optval = 1;

    if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(int)) < 0)
        printf("Cannot set TCP_NODELAY option "
               "on listen socket (%s)\n",
               strerror(errno));
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    // sin.sin_addr.s_addr = inet_addr(addr);
    sin.sin_port = htons(m_port);

    if ((bind(sockfd, (struct sockaddr *)&sin, sizeof(sin))) < 0)
        err(1, "bind");
    if ((listen(sockfd, 10)) < 0)
        err(1, "linsten");
    if ((setNonBlock(sockfd)) < 0)
        err(1, "setNonBlock");
    listen_event = event_new(m_base, sockfd, EV_READ | EV_PERSIST, invoke_cb_accept, this);
    event_add(listen_event, NULL);
    const int MAX_TIME_STEP = 10;
    long currentTime;
    long newTime;
    srand(time(NULL));
    // const float dt = 1000.0f / 30.0f;
    float frameTime;
    generateFood();
    currentTime = getCurrentTime();
    //create event timer
    m_handle.event = evtimer_new(m_loop, invoke_cb_disconnect, this);
    evtimer_add(m_handle.event, &m_handle.five_second);
    bool run = true;
    float totalTime = 0;
    long read_time;
    long left;
    while (run)
    {
        
        newTime = getCurrentTime();
        frameTime = (newTime - currentTime);
        currentTime = newTime;
        read_time = getCurrentTime();
        int loopFlags = event_base_loop(m_base, EVLOOP_NONBLOCK);
        left = getCurrentTime() - read_time;

        while (frameTime > 0.0f)
        {
            float delta = (float)std::min(DT, frameTime);
            // std::cout << delta << std::endl;
            playerUpdate(delta);
            bulletUpdate(delta);
            updateGrid();
            frameTime -= delta;
            m_sendingTick += delta;
        }
        checkCollide();
        processCommand();

        if (m_sendingTick >= SENDING_RATE)
        {
            sendCurrentPlayerPos();
            broadcast();
            sendFoodEat();
            m_sendingTick = 0;
        }
        // std::cout << (dt - (getCurrentTime() - currentTime -left))  * 1000 << std::endl;
        usleep(static_cast<useconds_t>((DT - (getCurrentTime() - currentTime)) * 1000));
    }

    close(sockfd);
}
void GameServer::cb_init(int fd, short what)
{
    if (players.find(fd) == players.end())
        return;
    Packet_I packet;
    packet.mapsize = BIGCIRCLE;
    packet.centerX = MAPSIZE / 2;
    packet.centerY = MAPSIZE / 2;
    packet.smallCircleRadius = SMALLCIRCLE;
    packet.moveSpeed = MOVE_SPEED;
    packet.playerRadius = PLAYER_RADIUS;
    packet.foodRadius = FOOD_RADIUS;
    packet.bulletRadius = BULLET_RADIUS;
    packet.itemRadius = ITEM_RADIUS;
    Point pos;
    pos.m_x = players.at(fd).getPositionX();
    pos.m_y = players.at(fd).getPositionY();
    // packet.x = (uint16_t) pos.m_x;
    // packet.y = (uint16_t ) pos.m_y;
    // packet.x = pos.m_x;
    // packet.y = pos.m_y;
    
    BitWriter wr(packet.getSize());
    packet.serialize(wr);
    if ((write(fd, wr.getBuf(), wr.getLen())) < 0)
    {
        warn("can not write fd: %d\n", fd);
        std::cout << "a player is disconnected" << std::endl;
        // event_del(players.at(fd).write_ev);
        // players.erase(fd);
        close(fd);
        event_del(send_init);
        return;
    }
    players.at(fd).read_ev = event_new(m_base, fd, EV_READ  , invoke_cb_read, this);
    players.at(fd).write_ev = event_new(w_base, fd, EV_WRITE, invoke_cb_write, this);
    event_add(players.at(fd).read_ev, NULL);
    event_add(players.at(fd).write_ev, NULL);
    m_grid->add(players.at(fd)); 
    std::cout << "done " << std::endl;

    event_del(send_init);
}
void GameServer::processCommand()
{
    for (auto it = players.begin(); it != players.end(); it++)
    {
        if (it->second.getCommand().size() > 0)
            it->second.getCommand().pop_front();
    }
}

void GameServer::cb_accept(int fd, short what)
{
    int clientfd;
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    clientfd = accept(fd, (struct sockaddr *)&clientaddr, &len);
    if (clientfd < 0)
    {
        warn("accpet failed");
        return;
    }
    if ((setNonBlock(clientfd)) < 0)
        err(1, "failed to set client non block");

    printf("new connected client from %s\n", inet_ntoa(clientaddr.sin_addr));
    printf("add event success fd: %d  ", fd);
    first = event_new(m_base, clientfd, EV_READ | EV_PERSIST, invoke_cb_read, this);
    event_add(first, NULL);
}

void GameServer::cb_read(int fd, short what)
{

    int len;
    char buf[100];

    len = read(fd, buf, 100);
    if (len == EWOULDBLOCK)
        return;

    // std::cout << "len  " << len << std::endl;

    if (len < 0)
    {
        warn("len < 0");

        close(fd);
        printf("a player is disconnect\n");
        if (players.find(fd) != players.end())
        {
            std::cout << "x :  " << players.at(fd).getPositionX() << " y:  " << players.at(fd).getPositionY() << std::endl;
            event_del(players.at(fd).read_ev);
            event_del(players.at(fd).write_ev);
            m_grid->deletePlayer(players.at(fd));
            players.erase(fd);

            //  free (buf);
            return;
        }
    }
    if (len == 0)
    {

        close(fd);
        printf("a player is disconnect\n");
        event_del(players.at(fd).read_ev);
        event_del(players.at(fd).write_ev);
        std::cout << players.at(fd).getPositionX() << "   " << players.at(fd).getPositionY() << std::endl;
        if (m_grid->deletePlayer(players.at(fd)))
            std::cout << "delete player from grid suceess" << std::endl;
        players.erase(fd);
        return;
    }
    int index = 0;
    if (players.find(fd) != players.end())
        event_add(players.at(fd).read_ev,NULL);
    while (index < len)
    {
        uint8_t type = readUint8((unsigned char *)buf, index);
        index++;
        switch ((short)type)
        {
        case CL_INIT_MSG:
        {
            //1vkl
            char name[len - 1];
            memcpy(name, buf + index, len);

            Point pos = respawnWithinCircle(MAPSIZE / 2, MAPSIZE / 2, BIGCIRCLE, SMALLCIRCLE);
            float angle = myRand(0, 360);
            angle = angle * PI / 180;
//            assert(players.find(fd) == players.end());
            if (players.find(fd) != players.end())
            {

                m_grid->deletePlayer(players.at(fd));
                players.erase(fd);
            }
            players.insert(std::make_pair(fd, Player(name, len - index, fd, pos.m_x, pos.m_y, PLAYER_RADIUS, angle)));
            printf("%s with len %d \n", players.at(fd).getName(), players.at(fd).getNameLength());

            send_init = event_new(m_base, fd, EV_WRITE, invoke_cb_init, this);
            event_add(send_init, NULL);
            std::cout << "send init " << std::endl;
            players.at(fd).setLastMessage(getCurrentTime());
            event_del(first);
            index += len;
        }
        break;
        case CL_ANGLE_MSG:
        {
            uint8_t size = readUint8((unsigned char *)buf, index);
            index++;
            for (int i = 0; i < size; i++)
            {
                FloatInt angle;
                angle.int_value = readUint32((unsigned char *)buf, index);
                players.at(fd).insertCommand(angle.float_value);
                players.at(fd).setAngle(angle.float_value);
                //    std::cout << "angle " << angle.float_value << std::endl;
                index += 4;
            }
        }
        break;
        case PING_PACKET:
        {
            Packet_Ping packet;
            BitWriter wr(packet.getSize());
            packet.serialize(wr);
            if ((write(fd, wr.getBuf(), wr.getLen())) < 0)
            {
                printf("send Ping < 0\n");
                return;
            }
        }
        break;
        case CL_SHOOT_MSG:
            players.at(fd).shoot();
            break;
        };
        
        
    }
}
void GameServer::cb_write(int fd, short what)
{
    std::cout << "writing " << std::endl;
    Packet_P packet;
    BitWriter wr(packet.getSize());
    if (players.find(fd) != players.end())
    {
        packet.x = players.at(fd).getPositionX();
        packet.y = players.at(fd).getPositionY();
        packet.serialize(wr);
        uint16_t x = readUint16((unsigned char *)wr.getBuf(), 1);
        uint16_t y = readUint16((unsigned char *)wr.getBuf(), 3);
        std::cout << "x : " << x << "y:  " << y << std::endl;
        if ((write(fd, wr.getBuf(), wr.getLen())) < 0)
        {
            warn("can not write fd: %d\n", fd);
            std::cout << "a player is disconnected" << std::endl;
            return;
        }
        event_add(players.at(fd).write_ev, NULL);
    }
    else
    {
        std::cout << "not found" << std::endl;
        return;
    }
   
}

long GameServer::getCurrentTime()
{
    using std::chrono::milliseconds;
    using std::chrono::duration_cast;
    using std::chrono::steady_clock;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}
void GameServer::playerUpdate(float dt)
{
    for (auto it = players.begin(); it != players.end(); it++)
    {
        it->second.update(dt);
        if (it->second.getIsShoot())
        {

            // int type = it->second.getFrontItem();
            int key = generateBulletID(it->second.getPositionX(), it->second.getPositionY(), bullets.size());

            Bullet bullet(it->second.getPositionX() + PLAYER_RADIUS * cosf(it->second.getHeadingAngle()), it->second.getPositionY() + PLAYER_RADIUS * sinf(it->second.getHeadingAngle()), 
            it->second.getHeadingAngle(), key, it->first);
           
            bullets.insert(std::make_pair(key, bullet));
            m_grid->add(bullet);
            it->second.setIsShoot(false);

        }
    }
}

void GameServer::broadcast()
{
    // std::cout << "broadcast" << std::endl;
    for (auto it = players.begin(); it != players.end(); it++)
    {

        m_grid->getPointsBoudingBox(it->second.getView(), it->second.m_allNode);
        it->second.filter(it->first);
        it->second.sendDataInqueue();
        it->second.sendNodeAroundMe();
        it->second.deleteNode();
    }
}

void GameServer::sendCurrentPlayerPos()
{

    for (auto it = players.begin(); it != players.end(); it++)
    {
        if (it->second.getIsChanged() ) {
            Packet_p packet;
        packet.x = it->second.getPositionX();
        packet.y = it->second.getPositionY();
        FloatInt angle;
        angle.float_value = it->second.getHeadingAngle();
        packet.angle = angle;
        packet.health = it->second.getHealth();
        BitWriter wr(packet.getSize());
        packet.serialize(wr);
        int len = 0;
        // printf("send my position fd: %d x: %d y: %d\n", it->first, x, y);
        if ((len = write(it->first, wr.getBuf(), wr.getLen())) < 0)
        {
            warn("can not write fd: %d\n", it->first);
            std::cout << "a player is disconnected" << std::endl;

            return;
        }
            it->second.setIsChanged(false);
            
        }
        else {
        Packet_P packet;
        packet.x = it->second.getPositionX();
        packet.y = it->second.getPositionY();
        FloatInt angle;
        angle.float_value = it->second.getHeadingAngle();
        packet.angle = angle;
        BitWriter wr(packet.getSize());
        packet.serialize(wr);
        uint16_t x = readUint16((unsigned char *)wr.getBuf(), 1);
        uint16_t y = readUint16((unsigned char *)wr.getBuf(), 3);
        int len = 0;
        // printf("send my position fd: %d x: %d y: %d\n", it->first, x, y);
        if ((len = write(it->first, wr.getBuf(), wr.getLen())) < 0)
        {
            warn("can not write fd: %d\n", it->first);
            std::cout << "a player is disconnected" << std::endl;

            return;
        }
        // std::cout << len << std::endl;
    }
    }
}
void GameServer::invoke_cb_disconnect(int fd, short what, void *agr)
{
    (static_cast<GameServer *>(agr))->disconnectPlayer(fd, what);
}
void GameServer::disconnectPlayer(int fd, short what)
{
    if (!evtimer_pending(m_handle.event, NULL))
    {
        event_del(m_handle.event);
        evtimer_add(m_handle.event, &m_handle.five_second);
    }
    for (auto it = players.begin(); it != players.end(); it++)
    {
        if (getCurrentTime() - it->second.getLastMessage() >= 5000)
        {
            std::cout << "disconnectPlayer: a player is disconnected > 5000 ms " << std::endl;
            close(it->second.getID());
            event_del(it->second.write_ev);
            event_del(it->second.read_ev);
            players.erase(it->second.getID());
        }
    }
}

void GameServer::updateGrid()
{
    for (auto it = players.begin(); it != players.end(); it++)
    {
        m_grid->update(it->second);
    }
    for (auto it = bullets.begin(); it != bullets.end(); it++)
    {
        
        m_grid->update(it->second);
    }
}
void GameServer::generateFood()
{
    for (int i = 0; i < FOOD_SIZE; i++)
    {
        
        Point pos;
        float angle = rand() % 360;
        angle = angle * PI / 180;
        int ranPoint = BIGCIRCLE - SMALLCIRCLE - 100;
        int offset = rand() % ranPoint;
        pos.m_x = cosf(angle) * SMALLCIRCLE + MAPSIZE / 2 + cosf(angle) * offset;
        pos.m_y = sinf(angle) * SMALLCIRCLE + MAPSIZE / 2 + sinf(angle) * offset;
        int item = rand() % TOTAL_ITEM;
        int id = myHash(pos.m_x, pos.m_y);
        printf("food x: %.1f  y: %.1f item type: %d\n", pos.m_x, pos.m_y, item);
        //  Food(float x, float y, float radius, int item, int id);
        Food food(pos.m_x, pos.m_y, FOOD_RADIUS, item, id);
        m_grid->add(food);
    }
}
void GameServer::checkCollide()
{

    for (auto it = players.begin(); it != players.end(); it++)
    {
        AABB box = it->second.getBox();
        std::unordered_map<int, Entity *> entity;
        m_grid->getPointsBoudingBox(box, entity);
        for (auto i = entity.begin(); i != entity.end(); i++)
        {
            if (i->second->getType() == FOOD_TYPE)
            {
                Food *food = dynamic_cast<Food *>(i->second);
                if (!food)
                    GAMELOG("GameServer checkCollide", "cast failed ");
                    if (food->getItem() == REFILL) {
                        it->second.setBulletSize(MAX_BULLET);
                        m_grid->deleteNode(*food);
                        continue;
                    }
                if (it->second.getItemTypeSize() >= MAX_ITEM)
                    continue;
             
                if (overlap(it->second, *food))
                {
                    it->second.insertItem(food->getItem());
                    node_collide.push_back(*food);

                    m_grid->deleteNode(*food);
                }
            }
            else if (i->second->getType() == BULLET_TYPE){
               
                Bullet *bullet = dynamic_cast<Bullet*>(i->second);
                assert(bullet);
                if (bullet->getOwner() == it->second.getID()) continue;
                it->second.justHit();
                m_grid->deleteNode(*bullet);
                bullets.erase(bullet->getID());
                
            }
        }
    }
}
void GameServer::sendFoodEat()
{
    //  printf("node size: %lu\n",node_collide.size());
    for (auto it = players.begin(); it != players.end(); it++)
    {
        it->second.deleteNodeEat(node_collide);
    }
    node_collide.clear();
}
void GameServer::bulletUpdate(float dt)
{
   
    for (auto it = bullets.begin(); it != bullets.end(); it++)
    {
        Bullet bullet = it->second;
        //   printf("bullet update x: %f y: %f\n", it->second.getPositionX(), it->second.getPositionY());

        bullet.update(dt);
        assert(bullet.getPositionX() >= 0 || bullet.getPositionY() >= 0 );
                it->second = bullet;
      
        if (it->second.getIsDelete())
        {
            m_grid->deleteNode(it->second);
            bullets.erase(it);
        }


    }
    
}
void GameServer::leaderBoard() {
    for (auto it = players.begin(); it != players.end(); it++) {
        std::vector<Player> leaderboard;
        leaderboard.push_back(it->second);
    }
}
