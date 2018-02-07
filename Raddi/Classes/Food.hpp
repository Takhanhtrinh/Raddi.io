//
//  Food.hpp
//  testClient
//
//  Created by Trinh Ta on 1/22/17.
//
//

#ifndef Food_hpp
#define Food_hpp

#include <iostream>
#include "cocos2d.h"
#include "Entity.hpp"

class Food:  public Entity{
public:
    static Food* createFood(float x, float y, float radius, int item, int id);
    ~Food();
    Food(float x, float y, float radius, int item, int id);
    Food(const Food &food);
    int getItem() const {return m_item;}
private:
    int m_item;
    //Food(float x, float y);
    
};

#endif /* Food_hpp */
