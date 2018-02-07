//
//  Entity.cpp
//  testClient
//
//  Created by Trinh Ta on 1/22/17.
//
//

#include "Entity.hpp"
Entity::Entity(const Entity & e):
m_type(e.m_type),
m_radius(e.m_radius)
{
    cocos2d::Vec2 pos = e.getPosition();
    setPosition(pos);
}
