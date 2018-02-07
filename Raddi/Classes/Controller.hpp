//
//  Controller.hpp
//  testClient
//
//  Created by Trinh Ta on 1/21/17.
//
//

#ifndef Controller_hpp
#define Controller_hpp

#include <iostream>
#include "cocos2d.h"
class Controller :public cocos2d::Sprite{
public:
    static Controller* createController();
    float getAngle() const {return m_angle;}
    void setFirstTouch(const cocos2d::Vec2 &firstTouch) {m_firstTouch = firstTouch;}
    void setLastTouch(const cocos2d::Vec2 &lastTouch) {m_lastTouch = lastTouch;}
    void update();
    void setAngle(float angle ) {m_angle = angle;}
    void dragEvent(cocos2d::Touch *touch);
    void addEvent();
    bool getChaged() {return m_lastAngle != m_angle;}
    ~Controller(){};
private:
    Controller(){};
    float m_angle;
    float m_lastAngle;
    cocos2d::Vec2 m_firstTouch;
    cocos2d::Vec2 m_lastTouch;
    
};

#endif /* Controller_hpp */
