//
//  World.hpp
//  Raddi
//
//  Created by Trinh Ta on 2/14/17.
//
//

#ifndef World_hpp
#define World_hpp

#include <memory>
#include <iostream>
#include <unordered_map>
#include "Player.hpp"
#include "Food.hpp"
#include "cocos2d.h"
#include "Bullet.hpp"
class GameWorld{
public:
    GameWorld(int mapSize, float centerX, float centerY, int smallCircle,int moveSpeed, int playerRadius,int foodRadius,int bulletRadius, int ItemRadius);
    void createPlayer(float x, float y,float angle, int id, cocos2d::Layer *layer);
    void insertFood(float x, float y, int item, int id,cocos2d::Layer *layer);
    int getSmallCircle() const {return m_smallCircle;}
    int getMapSize() const {return m_mapSize;}
    cocos2d::Vec2 getCenter() const {return m_center;}
    int getPlayerRadius() const {return m_playerRadius;}
    int getBulletRadius() const {return m_bulletRadius;}
    int getFoodRadius() const {return m_foodRadius;}
    void insertPlayer(float x, float y, int id,cocos2d::Layer *layer);
    void insertBullet(float x, float y, int id,cocos2d:: Layer *layer);
    void drawFood(cocos2d::DrawNode * drawFood);
    void drawBullet(cocos2d::DrawNode *drawBullet) const ;
    Player* getPlayer() ;
    void drawOtherPlayer(cocos2d::DrawNode *draw) const;
    void updateOtherPos(int id, float x, float y,int time, cocos2d::Layer* layer);
    Player *getOtherPlayer(int id) {return m_others.at(id);}
    void deleteNode(int id,cocos2d::Layer *layer);
    void drawLabel(cocos2d::Label *label);
     std::unordered_map<int, Food* > foods;
    void update(float dt);
    void deletePlayer(int id, cocos2d::Layer *layer);
    void updateBullet(float x, float y, int time ,int id,cocos2d:: Layer *layer);
    void removeBullet(int id,cocos2d:: Layer *layer);
    ~GameWorld(){};
private:
    Player* player;
    std::unordered_map<int, Bullet*> m_bullet;
    std::unordered_map<int, Player* > m_others;
   
    int m_mapSize;
    int m_smallCircle;
    int m_foodRadius;
    int m_bulletRadius;
    int m_itemRadius;
    int m_playerRadius;
    int m_moveSpeed;

    cocos2d::Vec2 m_center;
    
    
};

#endif /* World_hpp */
