#ifndef FOOD_H
#define FOOD_H
#pragma once
#include "Entity.h"
#include "definition.h"
    class Food: public Entity {
        public:
        Food(float x, float y, float radius, int item, int id);
        Food(const Food &food);
        int getItem() const {return m_item;}
        Food& operator=(const Food& food);
        void setIsEat(bool b) {isEat = b;}
        bool getIsEat() const {return isEat;}
        // AABB getBox() const ;
        private:
        int m_item;
        bool isEat;
        
    };
    

#endif