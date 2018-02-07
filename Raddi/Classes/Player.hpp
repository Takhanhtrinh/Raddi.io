//
//  Player.hpp
//  Radii_client
//
//  Created by Trinh Ta on 1/18/17.
//
//

#ifndef Player_hpp
#define Player_hpp
#include <stdint.h>
#include <iostream>
#include "cocos2d.h"
#include "Entity.hpp"
//#include "AABB.hpp"
#include <vector>
#include "definition.h"
#include <deque>
#include "Command.h"
//#include "Bullet.hpp"
#define PLAYER_RADIUS 10
//using namespace tt;
class Player:  public Entity {
public:
    
    
    
    static Player *createPlayer(float x, float y, int moveSpeed,int mapSize,float angle, float radius,int id,bool interpolate = true);
    Player(float x, float y, int moveSpeed,int mapSize,float angle, float radius,int id,bool interpolate = true);
    
    void update(float);
 
    Player(const Player &player);
    cocos2d::Vec2 getLastPos() const {return m_lastPos;}
    void setLastPos(const cocos2d::Vec2 & pos) {m_lastPos = pos;}
    void setlastPos(float x, float y) {m_lastPos = cocos2d::Vec2(x,y);}
    static float interpolate(float goal, float current, float dt, float time);
    void setAngle(float angle);
    float getAngle() const {return m_angle;}
    int getItemTypeSize() const {return (int)m_itemType.size();}
    void insertItem(int item);
    void setInterpolation(float time) {m_interpolationTime = time;}
    int getItem( int index ) const { return m_itemType.at(index);}

    bool shot();
    Player& operator= (const Player &player);
    void insertPos(uint16_t x, uint16_t y,int time);
    void setStartTime(long start) {m_start = start;}
    float getHeadingAngle() const {return m_headingAngle;}
    void setHeadingAngle(float v) {m_headingAngle = v;}
    
    
    
    
private:

    float m_interpolateX;
    float m_interpolateY;
    bool isInterpolate;
    long m_start;
    void findFrame(int time, float dt);
    bool m_cal;
    int m_speed;
    int m_mapSize;
    std::deque<int> m_itemType;
    int ticks;
    int currentTick;
    float m_angle;
    float m_headingAngle;

    cocos2d::Vec2 m_range;
    std::deque<Command> queue_pos;
    cocos2d::Vec2 m_lastPos;
    cocos2d::Vec2 m_velocity;
    float m_interpolationTime;
    

    
    
};

#endif /* Player_hpp */
