#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Controller.hpp"
#include "World.hpp"
#include <memory>
#include <event2/event.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include "Command.h"
#include "ui/CocosGUI.h"
#include "MyBuffer.hpp"
const float SENDING_RATE = 33;
enum CLIENT_MESSAGE
{
    CL_INIT_MSG = 1,
    CL_ANGLE_MSG = 2,
    CL_BUTTON_MSG = 3,
    
    
};
enum COMMAND{
    SPEED_BUTTON = 0x01,
    SHOOT_BUTTON = 0x02,
    USE_ITEM_BUTTON = 0x03
};
//enum PacketTypes{
//    INIT_PACKET = 'I' -65,
//    MOVE_PACKET = 'P' - 65,
//    NEW_PLAYER_PACKET = 'N' - 65,
//    UPDATE_NODE_PACKET = 'M' -65
//};
class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    struct timeval m_lastPing;
    
    struct timeval tv;
    bool m_init;
    int timePing;
    struct event *ev;
    void sendFirstInit();
    struct event_base *base;
    int ticks;
    struct timeval m_lastMessage;
    int m_delay;
    Controller *controller;
    static void invoke_cb(int fd, short what, void *agr);
    void cb(int fd, short what);
    void update(float dt) override;
      cocos2d::DrawNode *drawCirlcle;
    cocos2d::DrawNode *drawPlayer;
    int setNonBlock(int fd);
    static void invoke_cb_read(int fd, short what, void *agr);
    void cb_read(int fd, short what);
    static void invoke_cb_write(int fd, short what, void *agr);
    void cb_write(int fd, short what);
    struct event *read_ev;
    virtual bool init() override;
    void sendCommand(float dt);
    void sendPing(float dt);
    float m_sending;
    void storeCommand();
    void messageParse(char type,char *buf);
    struct timeval m_start;
  
    bool getQueueBuffer(int fd);
   
    std::deque<Command> m_sendingCommand;
    std::deque<Command> m_currentCommand;
 int sockfd;
   
    int time;
    int readCount;
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void touchEvent(cocos2d:: Ref *sender, cocos2d::ui::Widget::TouchEventType type);
    // a selector callback

    cocos2d::DrawNode *drawFood;

    std::unique_ptr<GameWorld> m_game;
    
    MyBuffer m_buf;
 

 
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
