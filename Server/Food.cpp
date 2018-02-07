
#include <iostream>
#include "Food.h"
 Food ::Food (float x, float y, float radius, int item, int id):
 Entity(FOOD_TYPE, x,y ,radius, id),
 m_item(item) {};
Food ::Food (const Food &food):
    Food(food.m_pos.m_x, food.m_pos.m_y, food.m_radius, food.m_item, food.m_id){};
 Food& Food::operator=(const Food& food){
     if (this != &food){
     m_pos.m_x = food.m_pos.m_x;
     m_pos.m_y = food.m_pos.m_y;
     m_id = food.m_id;
     m_item = food.m_item;
     m_radius = food.m_radius;
     }
     return *this;
     
 }
//  AABB Food:: getBox() const {
//      AABB box;
//      box.m_center.m_x = getPositionX();
//      box.m_center.m_y = getPositionY();
//      box.m_halfDim.m_x = m_radius;
//       box.m_halfDim.m_y = m_radius;
//  }