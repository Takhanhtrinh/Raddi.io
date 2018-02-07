//
//  Player.cpp
//  Radii_client
//
//  Created by Trinh Ta on 1/18/17.
//
//

#include <math.h>
#include "Player.hpp"
#include "Common.hpp"
#include <algorithm>
#include <math.h>


Player* Player::createPlayer(float x, float y,int moveSpeed,int mapSize, float angle,float radius, int id, bool interpolate) {
    Player* player = new Player(x,y,moveSpeed,mapSize,angle,radius,id,interpolate);
    if ( player && player->initWithFile("res/player.png")){
         player->autorelease();
        return player;
    }
    CC_SAFE_DELETE(player);
    return NULL;
    
}
Player::Player(float x, float y,int moveSpeed,int mapSize, float angle,float radius, int id, bool interpolate):
Entity(PLAYER_TYPE,radius,id),
m_speed(moveSpeed),
m_angle(angle),
m_mapSize(mapSize),
isInterpolate(true)
{
    
    m_velocity = cocos2d::Vec2(0,0);
    setPosition(cocos2d::Vec2(x,y));
    m_lastPos = (cocos2d::Vec2(x,y));
    //    m_view.m_center.m_x  = x;
    //    m_view.m_center.m_y = y;
    //    m_view.m_halfDim.m_x =250;
    //    m_view.m_halfDim.m_y = 250;
//    m_mass = PLAYER_RADIUS ;
//    m_updateRadius = true;
//    m_radiusShoot  = 0;
    std::cout << m_angle<< std::endl;
    
    
}
Player::Player(const Player &player):
Entity(PLAYER_TYPE,player.m_radius,player.m_id)
//m_view(player.m_view)
{
    this->setPosition(player.getPosition());
    m_lastPos = player.getPosition();
}

void Player::update(float dt){
     if (queue_pos.size() == 0  ) return;
    if (isInterpolate) {
        m_lastPos = getPosition();
//    std::cout << "queue size : " << queue_pos.size() << std::endl;
    currentTick++;
    if(!m_cal){
        Command command = queue_pos.front();
        m_interpolateX = interpolate(command.x, getPositionX(), dt, command.time + (dt*1000));
        m_interpolateY = interpolate(command.y, getPositionY(), dt, command.time +(dt*1000));
        findFrame(command.time - (dt*1000), dt);
        m_cal = true;
    }

    setPosition(getPositionX() + m_interpolateX, getPositionY() + m_interpolateY);
        m_headingAngle = CC_RADIANS_TO_DEGREES((atan2f(getPositionX() - m_lastPos.x, getPositionY() - m_lastPos.y)));
        setRotation(m_headingAngle);
        if (currentTick >= ticks){
        queue_pos.pop_front();
        m_cal =false;
        currentTick= 0;
    }
    }
    else {
        Command command = queue_pos.front();
        setPosition(command.x, command.y);
        queue_pos.pop_front();
    }
    if (queue_pos.size() > 10) {
        Command command = queue_pos.back();
        setPosition(command.x, command.y);
        queue_pos.clear();
  
    }
//    std::cout << "heading angle " << m_headingAngle << std::endl;
//    printf("queue size %lu\n", queue_pos.size());
}



//AABB Player:: getBoxForCollide() const {
//    AABB box;
//    box.m_center= m_view.m_center;
//    box.m_halfDim.m_x = m_mass;
//    box.m_halfDim.m_y = m_mass;
//    return box;
//}

void Player::setAngle(float angle) {
    
    m_angle = angle;
}
void Player:: insertItem(int item) {
    if (getItemTypeSize() < MAX_ITEM )
        m_itemType.push_back(item);
}
bool Player:: shot(){
    if (getItemTypeSize() > 0){
        m_itemType.pop_front();
        return true;
    }
    return false;
}
Player& Player:: operator= (const Player &player){
    
    if (this != &player) {
//        m_deceleration = player.m_deceleration;
//        m_mass = player.m_mass;
//        m_radiusShoot = player.m_radiusShoot;
        m_angle = player.m_angle;
        //        m_view= player.m_view;
        //        m_circle = player.m_circle;
        m_lastPos = player.m_lastPos;
        m_velocity = player.m_velocity;
        m_speed = player.m_speed;
//        m_orientation = player.m_orientation;
        m_id = player.m_id;
        m_radius = player.m_radius;
        m_type = PLAYER_TYPE;
        setPosition(player.getPosition());
        for (int i =0; i < player.m_itemType.size(); i++){
            m_itemType.clear();
            m_itemType.push_back(player.m_itemType.at(i));
        }
        
        
    }
    return *this;
    
}
void Player:: insertPos(uint16_t x, uint16_t y,int time){
    Command command;
    command.x = x;
    command.y = y;
    command.time = time;
    queue_pos.push_back(command);
}
float Player:: interpolate(float goal, float current, float dt, float time){
    float difference = goal - current;
    int framesTake = ceilf( time / (dt*1000));
//    std::cout << "frame rate " << cocos2d::Director::getInstance()->getFrameRate() << std::endl;
    return difference * dt * ceilf(cocos2d::Director::getInstance()->getFrameRate() / framesTake);
    
    
    
}
void Player:: findFrame(int time, float dt){
    ticks = ceilf (time / (dt * 1000));

}
