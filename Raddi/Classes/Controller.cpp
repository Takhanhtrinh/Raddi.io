//
//  Controller.cpp
//  testClient
//
//  Created by Trinh Ta on 1/21/17.
//
//

#include "Controller.hpp"
#include <math.h>
#include "definition.h"
Controller* Controller::createController(){
    Controller *rt = new (std::nothrow ) Controller();
    if (rt && rt->create()) {
        rt->autorelease();
    }
    else {
        CC_SAFE_DELETE(rt);
    }
    return rt;
}
void Controller::update(){
    m_lastAngle = m_angle;
    float x = m_lastTouch.x - m_firstTouch.x;
    float y = m_lastTouch.y - m_firstTouch.y;
    m_angle = atan2(y,x);
    if (m_angle < 0 ) {
        m_angle *= -1;
        m_angle = 2 * PI - m_angle;
    }
//    printf("angle :%f\n" , m_angle);
    
}
void Controller::addEvent() {
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        m_firstTouch = touch->getLocation();
        
        return true;
    };
    listener->onTouchMoved = [=](cocos2d::Touch* touch, cocos2d::Event* event) {
        //  std::cout << "drag began" << std::endl;
        dragEvent(touch);
    };
    
    listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        //std::cout << "end Touch" << std::endl;
        //m_firstTouch = m_lastTouch = cocos2d::Vec2(0,0);
    };
    
    
    cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);
    
}

void Controller::dragEvent(cocos2d::Touch *touch) {
    m_lastTouch =touch->getLocation();
    
    update();
    
}
