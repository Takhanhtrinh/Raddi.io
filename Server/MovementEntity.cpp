#include "MovementEntity.h"
// Entity(int type, float x, float y, int radius, int id);
MovementEntity::MovementEntity(int type,float x, float y,float lastX, float lastY, float angle, float radius, int id):
Entity(type,x,y, radius,id),
m_angle(angle),
m_lastPos(lastX,lastY){};