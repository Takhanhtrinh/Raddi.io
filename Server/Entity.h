#ifndef ENTITY_H
#define ENTITY_H

#include "AABB.cpp"

enum ENTITY_TYPE{
    FOOD_TYPE,
    BULLET_TYPE,
    ITEM_TYPE,
    PLAYER_TYPE
};
enum ItemType{
    REFILL,SLOWDOWN,LEFT,RIGHT,OBSTACLE, TOTAL_ITEM
};

    class Entity {
        public:
        Entity(int type, float x, float y, int radius, int id);
         Entity(const Entity &data);
        int getType() const {return m_type;}
        void setPos(float x, float y) {m_pos.m_x = x; m_pos.m_y = y;}
        void setPos(const Point &a) {setPos(a.m_x, a.m_y);}
        int getRadius() const {return m_radius;}
        Point getPosition() const {return m_pos;}
        float getPositionX() const {return m_pos.m_x;}
        float getPositionY() const {return m_pos.m_y;}
        void setPositionX(float x) {m_pos.m_x = x;}
        void setPositionY(float y) {m_pos.m_y = y;}
       
        AABB getBox() const;
        virtual ~Entity(){};
        int getID() const {return m_id;}
        protected:
        Point m_pos;
        int m_radius;
        int m_type;
        int m_id;
        private:
     
        
       
        

    };
#endif