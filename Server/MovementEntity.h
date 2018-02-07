#ifndef MOVEMENT_ENTITY_H
#define MOVEMENT_ENTITY_H
#include "Entity.h"
class MovementEntity:public Entity {
    public:
    // Entity(int type, float x, float y, int radius, int id);
MovementEntity(int type,float x, float y,float lastX, float lastY, float angle, float radius, int id);
virtual void update(float dt) = 0;
float getLastX() const {return m_lastPos.m_x;}
float getLastY() const {return m_lastPos.m_y;}
float getAngle() const {return m_angle;}
void setAngle(float v) {m_angle = v;}
Point getVelocity() const {return m_velocity;}
virtual ~MovementEntity(){};
    protected:
    Point m_lastPos;
    float m_angle;
    Point m_velocity;
    private:
};
#endif