//
//  Bullet.hpp
//  Raddi
//
//  Created by Trinh Ta on 2/28/17.
//
//

#ifndef Bullet_hpp
#define Bullet_hpp

#include <iostream>
#include "Entity.hpp"
#include <deque>
#include "Command.h"

class Bullet: public Entity{
//     Entity(int type, float radius, int id )
public:
    Bullet(float x, float y, float radius, int id);
    static Bullet* createBullet(float x, float y, float radius, int id);
    void update(float dt);
     static float interpolate(float goal, float current, float dt, float time);
    void insertCommand(float x, float y, int time);
    void findFrameTake(int time,float dt);
private:
    std::deque<Command> m_command;
    int ticks;
    int currentTick;
    bool m_cal;
    float m_interpolateX;
    float m_interpolateY;
};
#endif /* Bullet_hpp */
