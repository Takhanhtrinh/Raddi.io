//
//  Entity.hpp
//  testClient
//
//  Created by Trinh Ta on 1/22/17.
//
//

#ifndef Entity_hpp
#define Entity_hpp

#include <iostream>
#include "cocos2d.h"
enum ENTITY_TYPE{
    FOOD_TYPE,
    BULLET_TYPE,
    ITEM_TYPE,
    PLAYER_TYPE
};
enum ItemType{
    REFILL,SLOWDOWN,LEFT,RIGHT,OBSTACLE, TOTAL_ITEM
};
class Entity :public cocos2d::Sprite {
    
public:
    Entity(const Entity &e);
    Entity(int type, float radius, int id ) : m_type(type), m_id(id), m_radius(radius){};
    int getType() const {return m_type;}
    int getID() const {return m_id;}
    void setID(int id ) {m_id = id;}
    float getRadius() const {return m_radius;}
    void setRadius(float radius) {m_radius = radius;}
protected:
    int m_id;
    float m_radius;
    int m_type;
private:
    
    
    
    
    
};

#endif /* Entity_hpp */
