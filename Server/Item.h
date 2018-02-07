//
//  Item.hpp
//  testCocos
//
//  Created by Trinh Ta on 2/8/17.
//
//

#ifndef Item_H
#define Item_H

#include <iostream>
#include "Entity.h"
#include "time.h"
#include <sys/time.h>
enum ITEM_COLOR{
    RED,BLUE,GREEN,YELLOW
};
class Item : public Entity {
public:
    Item(float x, float y, int m_item, int id);
    Item(const Item &item);
    void update(float);
    Item& operator= (const Item &item);
    bool getStopUpdate() const {return m_stopUpdate;}
    int getItemType() const{return m_item;}
   
private:
    int m_item;
    struct timeval m_firstTime;
    bool m_stopUpdate;
    int m_color;
    
};
#endif /* Item_hpp */