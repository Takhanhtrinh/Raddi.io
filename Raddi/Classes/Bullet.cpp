//
//  Bullet.cpp
//  Raddi
//
//  Created by Trinh Ta on 2/28/17.
//
//

#include "Bullet.hpp"
#include "cocos2d.h"
//     Entity(int type, float radius, int id )
Bullet::  Bullet(float x, float y, float radius, int id):
Entity(BULLET_TYPE, radius,id ){
    setPosition(x, y);
}
Bullet* Bullet::createBullet(float x, float y, float radius, int id){
    Bullet *bullet = new Bullet(x,y,radius,id);
    
    if ( bullet && bullet->initWithFile("res/bullet.png")){
  
        bullet->autorelease();
        return bullet;
        
    }
    CC_SAFE_DELETE(bullet);
    return NULL;
}
float Bullet:: interpolate(float goal, float current, float dt, float time){
    float difference = goal - current;
    int framesTake = ceilf( time / (dt*1000));
    //    std::cout << "frame rate " << cocos2d::Director::getInstance()->getFrameRate() << std::endl;
    return difference * dt * ceilf(cocos2d::Director::getInstance()->getFrameRate() / framesTake);
}
void Bullet:: insertCommand(float x, float y, int time) {
    m_command.push_back(Command(x,y,time));
}
void Bullet::findFrameTake(int time, float dt) {
     ticks = ceilf (time / (dt * 1000));
}
void Bullet::update(float dt) {
    if (m_command.size() == 0 ) return;
    currentTick++;
    if (!m_cal) {
        Command command = m_command.front();
        m_interpolateX = interpolate(command.x, getPositionX(), dt, command.time + (dt * 1000));
        m_interpolateY = interpolate(command.y, getPositionY(), dt, command.time + (dt * 1000));
        findFrameTake(command.time - (dt * 1000), dt);
        m_cal = true;
    }
    setPosition(m_interpolateX + getPositionX(), m_interpolateY + getPositionY());
    if (currentTick >= ticks) {
        m_cal = false;
        m_command.pop_front();
        currentTick = 0;
    }
    
}
