//
//  Bullet.cpp
//  testCocos
//
//  Created by Trinh Ta on 2/8/17.
//
//

#include "Bullet.h"
#include "definition.h"
#include "protocol/common.h"
   //Entity(int type, float x, float y, int radius, int id);

   //MovementEntity(int type,float x, float y,float lastX, float lastY, float angle, float radius, int id);
Bullet::Bullet(float x, float y,float angle, int id, int owner ):
MovementEntity(BULLET_TYPE, x,y,x,y, angle, BULLET_RADIUS, id),

m_stopUpdate(false),
m_delete(false),
m_owner(owner),
m_ticks(0)
{
    
    m_velocity = Point(cosf(m_angle) * MOVE_SPEED * 2,sinf(m_angle) * MOVE_SPEED * 2);
};
Bullet ::Bullet (const Bullet &bullet):
MovementEntity(BULLET_TYPE,bullet.m_pos.m_x , bullet.m_pos.m_y, bullet.m_lastPos.m_x, bullet.m_lastPos.m_y, bullet.m_angle, BULLET_RADIUS , bullet.m_id),

m_stopUpdate(bullet.m_stopUpdate),
m_delete(bullet.m_delete),
m_owner(bullet.m_owner),
m_ticks(bullet.m_ticks)
// m_velocity(bullet.m_velocity)
{
    m_velocity = bullet.m_velocity;
   
};


void Bullet::update(float dt) {
    
    // printf("velocity x: %f velocity y: %f \n" , m_velocity.m_x, m_velocity.m_y);
    m_ticks += dt;
     if (m_ticks > BULLET_ALIVE) m_stopUpdate = true;
    
    if (m_ticks > BULLET_STOP) m_delete = true;
    if (m_stopUpdate) return;
 
         m_lastPos = m_pos;
        
            m_velocity.limit(MOVE_SPEED  * dt/ 1000);

        
        m_pos.m_x += m_velocity.m_x;
         m_pos.m_y += m_velocity.m_y;
        float dx = m_pos.m_x - MAPSIZE/ 2;
        float dy = m_pos.m_y  - MAPSIZE / 2;
        float distanceFromCenter = sqrt((dx * dx) + (dy * dy));
        if (distanceFromCenter >= BIGCIRCLE - m_radius || distanceFromCenter <= SMALLCIRCLE + m_radius){
            m_pos = m_lastPos;
           m_hit = true;
            float normalMag = distanceFromCenter;
            float normalX = dx / normalMag;
            float normalY = dy / normalMag;
            float tangentX = -normalY;
            float tangentY = normalX;
            float normalSpeed = -(normalX * m_velocity.m_x + normalY * m_velocity.m_y);
            float tangentSpeed = tangentX * m_velocity.m_x + tangentY * m_velocity.m_y;
            m_velocity.m_x = normalSpeed * normalX + tangentSpeed * tangentX  ;
            m_velocity.m_y = normalSpeed * normalY + tangentSpeed * tangentY ;
            m_velocity.limit(MOVE_SPEED * dt / 1000);

        }
       


   
    
}
Bullet& Bullet::operator=(const Bullet &data) {
    if (this != &data) {
        m_pos = data.m_pos;
        m_lastPos = data.m_lastPos;
        m_owner = data.m_owner;
 
        m_stopUpdate = data.m_stopUpdate;
        m_id = data.m_id;
        m_radius = data.m_radius;
        m_delete  = data.m_delete;
        m_ticks = data.m_ticks;
        m_velocity = data.m_velocity;
        m_newVel = data.m_newVel;
        m_hit = data.m_hit;

    }
    return *this;
}
