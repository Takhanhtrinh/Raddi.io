//
//  Food.cpp
//  testClient
//
//  Created by Trinh Ta on 1/22/17.
//
//

#include "Food.hpp"
Food* Food::createFood(float x, float y, float radius, int item, int id){
    auto food = new Food(x,y,radius,item, id);
    if (food && food->initWithFile("res/Food 16x16.png")){
         food->autorelease();
        return food;
    }
    CC_SAFE_DELETE(food);
    return nullptr;
    
}
Food::~Food(){};
Food::Food(float x, float y, float radius, int item , int id):
Entity(FOOD_TYPE, radius, id),
m_item(item)
{
    cocos2d::Vec2 pos (x,y);
    setPosition(pos);
};
Food ::Food(const Food &food):
Food(food.getPosition().x, food.getPosition().y, food.m_radius, food.m_item, food.m_id){
    
}
