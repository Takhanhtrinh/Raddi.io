//
//  Bullet.hpp
//  testCocos
//
//  Created by Trinh Ta on 2/8/17.
//
//

#ifndef BULLET_H
#define BULLET_H

#include <iostream>
#include "Entity.h"
#include "MovementEntity.h"
// bullet move within 3seconds and then stop for 5 more seconds
const int BULLET_ALIVE = 3000;
const int BULLET_STOP =  6000;
class Bullet :  public MovementEntity {
public:
    
    Bullet(float x, float y, float angle, int id, int owner );
    Bullet (const Bullet &bullet);
    void update(float dt);
    bool getStopUpdate() const {return m_stopUpdate;}
    int getOwner() const {return m_owner;}
    bool getIsDelete() const {return m_delete;}
    // AABB getBox() const;
    Bullet &operator= (const Bullet & data);
private:


    Point m_newVel;
    bool m_stopUpdate;
    int m_ticks;
    int m_owner;
    bool m_delete;
    bool m_hit;
};
#endif /* Bullet_hpp */
