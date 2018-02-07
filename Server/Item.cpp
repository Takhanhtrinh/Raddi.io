//
//  Item.cpp
//  testCocos
//
//  Created by Trinh Ta on 2/8/17.
//
//

#include "Item.h"
#include "definition.h"
//Entity::Entity(int type, float x, float y, int radius,int id)
Item::Item(float x, float y, int item, int id):
Entity(ITEM_TYPE,x,y,ITEM_RADIUS,id),
m_stopUpdate(false),
m_item(item) {
    

    gettimeofday(&m_firstTime, NULL);
    switch(item) {
        case SLOWDOWN:
            m_color = BLUE;
            break;
        case LEFT:
            m_color = YELLOW;
            break;
        case RIGHT:
            m_color = GREEN;
            break;
        case OBSTACLE:
            m_color = RED;
            break;
    }


}
Item:: Item(const Item &item):
Item(item.getPositionX(),item.getPositionY(),item.m_item,item.m_id){};

void Item ::update(float dt) {
    if (!m_stopUpdate){
    struct timeval current;
    gettimeofday(&current, NULL);
        if ((current.tv_sec - m_firstTime.tv_sec) * 1000  > ITEM_TIME){
        m_stopUpdate = true;
        }
 
    }
}
Item& Item:: operator= (const Item &item){
    if (this != &item) {
        m_id = item.m_id;
        m_item = item.m_item;
        m_radius = item.m_radius;
        m_stopUpdate = item.m_stopUpdate;
        m_pos = item.m_pos;
        m_type = item.m_type;
        m_firstTime = item.m_firstTime;
        m_color = item.m_color;
    }
    return *this;
}


