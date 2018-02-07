
#include<iostream>
#include "Entity.h"

Entity::Entity(int type, float x, float y, int radius,int id):
m_type(type),
m_pos(x,y),
m_radius(radius),
m_id (id){}

Entity::Entity(const Entity &data):
m_type(data.getType()),
 m_radius(data.getRadius()),
 m_pos(data.m_pos),
 m_id (data.m_id){}
 
 AABB Entity:: getBox() const {
     AABB box;
     box.m_center = getPosition();
     box.m_halfDim.m_x = m_radius;
     box.m_halfDim.m_y = m_radius;
     return box;
 }