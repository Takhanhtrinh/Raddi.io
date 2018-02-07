#include "Player.h"
#include "definition.h"
#include <iostream>
#include <math.h>
#include "protocol/common.h"
#include <unistd.h>
#include "protocol/BitPack.h"
#include "protocol/Packet.h"

//Entity(int type, float x, float y, int radius, int id);
// MovementEntity::MovementEntity(int type, float x, float y, float angle, int radius, int id):
Player::Player(char *name, int nameLen, int id, float x, float y, int radius, float angle) : MovementEntity(PLAYER_TYPE, x, y,x,y, PLAYER_RADIUS, angle, id),                                                                               
                                                                                              m_die(false),                                                                                         
                                                                                             lastMessage(0),
                                                                                             m_nameLength(nameLen),
                                                                                             m_bulletSize(MAX_BULLET),
                                                                                             m_health(100),
                                                                                             m_headingAngle(angle),
                                                                                             canShoot(true),
                                                                                             m_bulletDelay(0),
                                                                                             isShoot(false),
                                                                                             m_speed(0),
                                                                                             m_steering(0)
                                                                                            
                                                                                            
{
    m_name = (char *)malloc(m_nameLength);
    memcpy(m_name, name, m_nameLength);
    m_view.m_center.m_x = x;
    m_view.m_center.m_y = y;
    m_view.m_halfDim.m_x = VIEW_RADIUS;
    m_view.m_halfDim.m_y = VIEW_RADIUS;
    m_velocity = Point(cosf(m_angle) * MOVE_SPEED * 2,sinf(m_angle) * MOVE_SPEED * 2 );
   

};
//Entity(int type, float x, float y, int radius, int id);
Player::Player(const Player &player) : 
MovementEntity(PLAYER_TYPE,player.m_pos.m_x, player.m_pos.m_y, player.m_lastPos.m_x, player.m_lastPos.m_y,PLAYER_RADIUS ,player.m_angle,player.m_id),
m_die(player.m_die), 
lastMessage(player.lastMessage),
m_headingAngle(player.m_headingAngle),
m_nameLength(player.m_nameLength),
m_health(player.m_health),
m_bulletSize(player.m_bulletSize),
canShoot(player.canShoot),
m_bulletDelay(player.m_bulletDelay),
isShoot(player.isShoot),
m_speed(player.m_speed),
m_steering(player.m_steering)
{
    m_name = (char *)malloc(m_nameLength);
    memcpy(m_name, player.m_name, player.m_nameLength);
    m_velocity = player.m_velocity;
    m_view = player.m_view;
}
void Player::update(float dt)
{
    m_bulletDelay+= dt;
    ticks += dt;
   m_lastPos.m_x = m_pos.m_x;
   m_lastPos.m_y = m_pos.m_y;
    m_speed += 50 * dt / 1000;
    if (m_speed > MOVE_SPEED)
        m_speed =MOVE_SPEED;
    

    Point target = findTarget(m_pos, m_angle);
    Point desired = findDesiredVec(target, m_pos);
    desired.normalize();
    desired.addMagnitude(m_speed * dt / 1000);

    Point newVec = findSterring(desired, m_velocity);
    m_steering = m_speed * STEERING_FORCE / MOVE_SPEED * dt / 1000;
    newVec.limit(m_steering);
    m_velocity.m_x += newVec.m_x;
    m_velocity.m_y += newVec.m_y;
    m_velocity.limit(m_speed * dt / 1000);
    
    m_pos.m_x += m_velocity.m_x;
    m_pos.m_y +=  m_velocity.m_y;
    m_headingAngle =  angleOf2Points(m_lastPos.m_x, m_lastPos.m_y, m_pos.m_x, m_pos.m_y);

    float dx = m_pos.m_x - MAPSIZE / 2;
    float dy = m_pos.m_y - MAPSIZE / 2;
    float distance = sqrt((dx * dx) + (dy*dy));
    if (distance >= BIGCIRCLE - m_radius ) {
        
        m_pos = m_lastPos;
        // m_velocity.limit((MOVE_SPEED - 200) * dt / 1000);
        m_speed = 50;
        m_pos.m_x -= m_velocity.m_x * 10;
        m_pos.m_y -= m_velocity.m_y * 10;
        
        // m_pos.limit(MOVE_SPEED * dt / 1000);

    }
   
    if (checkOutsideOfMap(getPositionX(), getPositionY(), m_radius, MAPSIZE / 2, MAPSIZE / 2, SMALLCIRCLE))
    {
       m_pos = m_lastPos;
        // m_velocity.limit((MOVE_SPEED - 200) * dt / 1000);
        m_speed = 50;
        m_pos.m_x -= m_velocity.m_x * 10;
        m_pos.m_y -= m_velocity.m_y * 10;
        
    }

    if (ticks > SENDING_OLD_NODE)
    {
        updateView();
        ticks = 0;
    }
    if (m_bulletDelay > BULLET_DELAY) {
        canShoot =true;
        m_bulletDelay = 0;
    }
    // printf("heading angle %f\n", m_headingAngle);
}
void Player::insertItem(int item)
{
    if (getItemTypeSize() < MAX_ITEM)
        m_itemType.push_back(item);
}
int Player::getFrontItem()
{
    int type = m_itemType.front();
    m_itemType.pop_front();
    return type;
}
void Player::shoot()
{
   if (canShoot && m_bulletSize > 0 ) {
    isShoot = true;
    m_bulletSize--;
   }

}

// AABB Player::getBoxForCollide() const
// {
//     AABB box;
//     box.m_center = getPosition();
//     box.m_halfDim.m_x = m_radius;
//     box.m_halfDim.m_y = m_radius;
//     return box;
// }
Player &Player::operator=(const Player &player)
{
    if (this != &player)
    {
        m_steering = player.m_steering;
        m_angle = player.m_angle;
        m_view = player.m_view;
        m_die = player.m_die;
        m_lastPos = player.m_lastPos;
        m_nameLength = player.m_nameLength;
        memcpy(m_name, player.m_name,m_nameLength);
        m_id = player.m_id;
        m_health = player.m_health;
        m_bulletSize = player.m_bulletSize;
        m_bulletDelay = player.m_bulletDelay;
        canShoot = player.canShoot;
        m_radius = player.m_radius;
        m_type = PLAYER_TYPE;
        isShoot = player.isShoot;
        ticks = player.ticks;
        m_speed = player.m_speed;
        m_pos = player.m_pos;
        m_velocity = player.m_velocity;
        for (int i = 0; i < player.m_itemType.size(); i++)
        {
            m_itemType.clear();
            m_itemType.push_back(player.m_itemType.at(i));
        }
    }
    return *this;
}

Player::~Player()
{
    // if (m_name != NULL){
    free(m_name);
    // }
}

void Player::filter(int id)
{
    for (auto it = m_allNode.begin(); it != m_allNode.end(); it++)
    {
        if (it->first == id)
            continue;
        if (m_sendNode.find(it->first) == m_sendNode.end())
        {
            //new node = true oldnode = false
            m_sendNode.insert(std::make_pair(it->first, true));

            // printf("filter new node with id %d\n",m_allNode.at(i)->getID());
        }
    }
}
void Player::sendNodeAroundMe()
{
    int nodeSize = 0;
    for (auto it = m_allNode.begin(); it != m_allNode.end(); it++)
    {
        if (it->first == m_id)
            continue;
        if (it->second->getType() == PLAYER_TYPE)
        {
            Player *player = dynamic_cast<Player *>(it->second);
            if (!player)
            {
                std::cout << "<Send all node> cast failed " << std::endl;
                return;
            }
            if (m_sendNode.at(it->first) == true)
            {
                //send new node

                Packet_N packet(player->getNameLength());
                packet.id = player->getID();
                packet.x = player->getPositionX();
                packet.y = player->getPositionY();

                memcpy(&packet.name, player->getName(), player->getNameLength());
                BitWriter wr(packet.getSize());
                packet.serialize(wr);
                int len;

                len = write(m_id, wr.getBuf(), wr.getLen());
                if (len < 0)
                {
                    printf("write faied\n");
                    return;
                }
                if (len < wr.getLen()) {
                    MyBuffer buf;
                    buf.create(wr.getLen());
                    buf.append(wr.getBuf(), wr.getLen());
                    buf.justWrite(len);
                    m_buf.push_back(buf);
                   

                }
                m_sendNode.at(it->first) = false;
                old_player.insert(std::make_pair(player->getID(), *player));
            }

            else
            {
                Packet_M packet;
                packet.id = player->getID();
                packet.x = player->getPositionX();
                packet.y = player->getPositionY();
                BitWriter wr(packet.getSize());
                // std::cout << "my id: " << m_id << "  My position x:  " << getPositionX() << " y: " << getPositionY() << std::endl;
                //     std::cout << "send old node name: " << " id: " << player->getID() << std::endl;
                //     std::cout << "packet position x: " << packet.x << " y: " << packet.y << std::endl;
                packet.serialize(wr);
                int len;
                //    std::cout <<"send old node" << std::endl;
                 len = write(m_id, wr.getBuf(), wr.getLen());
                if (len < 0)
                {
                    printf("write faied\n");
                    return;
                }
                if (len < wr.getLen()) {
                    MyBuffer buf;
                    buf.create(wr.getLen());
                    buf.append(wr.getBuf(), wr.getLen());
                    buf.justWrite(len);
                    m_buf.push_back(buf);
                 

                }
                Player temp = *player;
                old_player.at(player->getID()) = temp;
            }
        }
        else if (it->second->getType() == FOOD_TYPE)
        {
            if (nodeSize > 20)
                continue;
            Food *food = dynamic_cast<Food *>(it->second);
            if (!food)
            {
                printf("<PLAYER send around me>cast food failed\n ");
                return;
            }
            if (m_sendNode.at(food->getID()) == true)
            {
                Packet_food packet;
                packet.id = food->getID();
                packet.type = food->getItem();
                packet.x = food->getPositionX();
                packet.y = food->getPositionY();
                BitWriter wr(packet.getSize());
                packet.serialize(wr);
                int len;
                 len = write(m_id, wr.getBuf(), wr.getLen());
                if (len < 0)
                {
                    printf("write faied\n");
                    return;
                }
                if (len < wr.getLen()) {
                    MyBuffer buf;
                    buf.create(wr.getLen());
                    buf.append(wr.getBuf(), wr.getLen());
                    buf.justWrite(len);
                    m_buf.push_back(buf);
                 

                }
                Food temp(*food);
                m_sendNode.at(food->getID()) = false;
                old_node.insert(std::make_pair(food->getID(), temp));
                nodeSize++;
                //  printf("send new food with x: %d y: %d with type: %d id: %d\n", packet.x,packet.y,packet.type, packet.id);
            }
        }

        else if (it->second->getType() == BULLET_TYPE)
        {

            Bullet *bullet = dynamic_cast<Bullet *>(it->second);
            if (!bullet)
            {
                GAMELOG("sendNodeAroundMe", " cast bullet failed ");
                return;
            }
            if (m_sendNode.at(bullet->getID()) == true)
            {
                Packet_Bullet packet;
                packet.id = bullet->getID();
                packet.x = bullet->getPositionX();
                packet.y = bullet->getPositionY();
                BitWriter wr(packet.getSize());
                packet.serialize(wr);
                int len;
                len = write(m_id, wr.getBuf(), wr.getLen());
                if (len < 0)
                {
                    printf("write faied\n");
                    return;
                }
                if (len < wr.getLen()) {
                    MyBuffer buf;
                    buf.create(wr.getLen());
                    buf.append(wr.getBuf(), wr.getLen());
                    buf.justWrite(len);
                    m_buf.push_back(buf);
                 

                }
                Bullet temp(*bullet);
                m_sendNode.at(bullet->getID()) = false;
                old_bullet.insert(std::make_pair(bullet->getID(), *bullet));
                printf("just send new bullet \n");
            }
            else
            {

                Packet_Update_Bullet packet;
                packet.id = bullet->getID();
                packet.x = bullet->getPositionX();
                packet.y = bullet->getPositionY();
                BitWriter wr(packet.getSize());
                packet.serialize(wr);
                int len;
                len = write(m_id, wr.getBuf(), wr.getLen());
                if (len < 0)
                {
                    printf("write faied\n");
                    return;
                }
                if (len < wr.getLen()) {
                    MyBuffer buf;
                    buf.create(wr.getLen());
                    buf.append(wr.getBuf(), wr.getLen());
                    buf.justWrite(len);
                    m_buf.push_back(buf);
                }
                if (old_bullet.find(bullet->getID()) != old_bullet.end())
                    old_bullet.at(bullet->getID()) = *bullet;
            }
        }
    }
}

void Player::deleteNode()
{

    std::vector<int> foodToDelete;
    for (auto it = old_node.begin(); it != old_node.end(); it++)
    {
        // if (!getView().intersects(it->second.getBox())){
        if (m_allNode.find(it->first) == m_allNode.end())
        {
            Food food = it->second;
            Packet_rmFood packet;
            packet.id = food.getID();
            BitWriter wr(packet.getSize());
            packet.serialize(wr);
            int len;
            len = write(m_id, wr.getBuf(), wr.getLen());
                if (len < 0)
                {
                    printf("write < 0 \n");
                    return;
                }
                if (len < wr.getLen()) {
                    MyBuffer buf;
                    buf.create(wr.getLen());
                    buf.append(wr.getBuf(), wr.getLen());
                    buf.justWrite(len);
                    m_buf.push_back(buf);
                 

                }
            // printf("deelte node with id: %d\n" , food.getID());
            if (m_sendNode.find(food.getID()) != m_sendNode.end())
            {
                foodToDelete.push_back(food.getID());
            }
        }
    }
    for (int i = 0; i < foodToDelete.size(); i++)
    {
        if (m_sendNode.find(foodToDelete.at(i)) != m_sendNode.end())
            m_sendNode.erase(foodToDelete.at(i));
        if (old_node.find(foodToDelete.at(i)) != old_node.end())
            old_node.erase(foodToDelete.at(i));
    }
    std::vector<int> playerToDelete;
    for (auto it = old_player.begin(); it != old_player.end(); it++)
    {
        // if (!getView().intersects(it->second.getBox())){
        if (m_allNode.find(it->first) == m_allNode.end())
        {
            if (m_sendNode.find(it->first) != m_sendNode.end())
            {
                Player player = it->second;
                Packet_rmPlayer packet;
                packet.id = it->first;
                BitWriter wr(packet.getSize());
                packet.serialize(wr);
                int len = 0;
               len = write(m_id, wr.getBuf(), wr.getLen());
                if (len < 0)
                {
                    printf("write faied\n");
                    return;
                }
                if (len < wr.getLen()) {
                    MyBuffer buf;
                    buf.create(wr.getLen());
                    buf.append(wr.getBuf(), wr.getLen());
                    buf.justWrite(len);
                    m_buf.push_back(buf);
                 

                }
                playerToDelete.push_back(it->first);
            }
            else
                printf("delete player out of range\n");
        }
    }
    for (int i = 0; i < playerToDelete.size(); i++)
    {
        m_sendNode.erase(playerToDelete.at(i));
        old_player.erase(playerToDelete.at(i));
    }
    std::vector<int> bulletToDelete;
    for (auto it = old_bullet.begin(); it != old_bullet.end(); it++)
    {
        // if (!getDeleteView().intersects(it->second.getBox())) {
        if (m_allNode.find(it->first) == m_allNode.end())
        {
            
                Bullet bullet = it->second;
                Packet_rmBullet packet;
                packet.id = bullet.getID();
                BitWriter wr(packet.getSize());
                packet.serialize(wr);
                int len = 0;
                len = write(m_id, wr.getBuf(), wr.getLen());
                if (len < 0)
                {
                    printf("write faied\n");
                    return;
                }
                if (len < wr.getLen()) {
                    MyBuffer buf;
                    buf.create(wr.getLen());
                    buf.append(wr.getBuf(), wr.getLen());
                    buf.justWrite(len);
                    m_buf.push_back(buf);
                 

                }
                printf("delete out of range bullet\n");
                bulletToDelete.push_back(bullet.getID());
             
            }
        }
        for (int i =0 ; i < bulletToDelete.size(); i++) {
            if (old_bullet.find(bulletToDelete.at(i)) != old_bullet.end()) 
                old_bullet.erase(bulletToDelete.at(i));
        }
    
    
}
void Player::deleteNodeEat(std::vector<Food> food)
{
    for (int i = 0; i < food.size(); i++)
    {
        Food node = food.at(i);
        if (getView().intersects(node.getBox()))
        {
            if (old_node.find(node.getID()) != old_node.end())
            {
                Packet_rmFood packet;
                packet.id = node.getID();
                BitWriter wr(packet.getSize());
                packet.serialize(wr);
                int len;
               len = write(m_id, wr.getBuf(), wr.getLen());
                if (len < 0)
                {
                    printf("write faied\n");
                    return;
                }
                if (len < wr.getLen()) {
                    MyBuffer buf;
                    buf.create(wr.getLen());
                    buf.append(wr.getBuf(), wr.getLen());
                    buf.justWrite(len);
                    m_buf.push_back(buf);
                 

                }
                old_node.erase(node.getID());
                if (m_sendNode.find(node.getID()) != m_sendNode.end())
                    m_sendNode.erase(node.getID());
            }
        }
    }
}
void Player::updateView()
{
    m_view.m_center = getPosition();
}
AABB Player::getCurrentView() const
{
    AABB box;
    box.m_center = getPosition();
    box.m_halfDim.m_x = VIEW_RADIUS;
    box.m_halfDim.m_y = VIEW_RADIUS;
    return box;
}
AABB Player::getDeleteView() const
{
    AABB box;
    box.m_center = m_view.m_center;
    box.m_halfDim.m_x = VIEW_RADIUS - 200;

    box.m_halfDim.m_y = VIEW_RADIUS - 200;
    return box;
}
void Player::setAngle(float angle)
{
    

    m_angle = angle;
}
  void Player :: sendDataInqueue() {

      if (m_buf.size() > 0 ) {
          MyBuffer buf = m_buf.front();
          int len;
          len = write(m_id, buf.getBuf() + buf.getWriteTo(), buf.getSizeLeft()); 
          if (len < 0) {
            GAMELOG("Player", "sendDataInqueue");
            return;
          }
          else if (len < buf.getSizeLeft()) {
              buf.justWrite(len);
              m_buf.pop_front();
              m_buf.push_front(buf);
          }
          else if (len == buf.getSizeLeft()) {
              m_buf.pop_front();
              GAMELOG("player sendDataInqueue", " done");
          }
      }
  }