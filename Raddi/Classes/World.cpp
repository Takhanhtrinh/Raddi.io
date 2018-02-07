//
//  World.cpp
//  Raddi
//
//  Created by Trinh Ta on 2/14/17.
//
//

#include "World.hpp"
GameWorld ::GameWorld(int mapSize, float centerX, float centerY, int smallCircle,int moveSpeed, int playerRadius,int foodRadius,int bulletRadius, int ItemRadius):
m_mapSize(mapSize),
m_center(centerX,centerY),
m_smallCircle(smallCircle),
m_moveSpeed(moveSpeed),
m_playerRadius(playerRadius),
m_foodRadius(foodRadius),
m_bulletRadius(bulletRadius),
m_itemRadius(ItemRadius),
player(nullptr){};
void GameWorld:: createPlayer(float x, float y, float angle ,int id, cocos2d::Layer *layer) {

    player= Player::createPlayer(x,y,m_moveSpeed,m_mapSize,angle,m_playerRadius,id);
    cocos2d::Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
   
      player->setScale(2);
     player->getTexture()->setTexParameters(texParams);
    
    layer->addChild(player,2);
}
void GameWorld:: insertFood(float x, float y, int item, int id, cocos2d::Layer *layer){
//    Food(float x, float y, float radius, int item, int id);
    Food *food = Food::createFood(x, y, m_foodRadius, item, id);
    cocos2d::Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
    food->getTexture()->setTexParameters(texParams);
    food->setScale(2);
    layer->addChild(food,1);
    foods.insert(std::make_pair(id, food));
}
void GameWorld:: update(float dt){
    if (player != NULL){
        player->update(dt);
//        printf("my pos x: %f  y: %f\n" , player->getPositionX(), player->getPositionY());
    }
    for (auto it = m_others.begin(); it!= m_others.end(); it++ ){
        it->second->update(dt);
//        printf("other pos x: %f  y: %f\n" , it->second->getPositionX(), it->second->getPositionY());
    }
    for (auto it = m_bullet.begin(); it!= m_bullet.end(); it++) {
        it->second->update(dt);
    }
       
}

Player* GameWorld:: getPlayer() {
    if(player)
        return player;
    return nullptr;
}
void GameWorld:: insertPlayer(float x, float y, int id, cocos2d::Layer* layer){
    Player *temp = Player::createPlayer(x, y, m_moveSpeed, m_mapSize, PI/2, m_playerRadius, id);
    cocos2d::Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
    temp->getTexture()->setTexParameters(texParams);
    temp->setScale(2);
    m_others.insert(std::make_pair(id,temp));
    layer->addChild(temp,2);
}
void GameWorld:: updateOtherPos(int id, float x, float y, int time, cocos2d::Layer* layer){
    if (m_others.find(id) == m_others.end()) {
        Player *temp = Player::createPlayer(x, y, m_moveSpeed, m_mapSize, PI/2, m_playerRadius, id);
        cocos2d::Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
        temp->getTexture()->setTexParameters(texParams);
        temp->setScale(2);
        m_others.insert(std::make_pair(id,temp));
        layer->addChild(temp,2);
        return;
        
    }
    m_others[id]->insertPos(x, y, time);
}

void GameWorld:: drawOtherPlayer(cocos2d::DrawNode *draw) const{
        for (auto it = m_others.begin(); it != m_others.end();it++){
            draw->drawSolidCircle(it->second->getPosition(), m_playerRadius, 360, 50, 1, 1, cocos2d::Color4F::RED);
        }
}
void GameWorld:: drawLabel(cocos2d::Label *label){
    for (auto it = m_others.begin(); it != m_others.end();it++){
        auto info = cocos2d::__String::createWithFormat("x: %.1f  y: %.1f", it->second->getPositionX(),it->second->getPositionY());
        label->setString(info->getCString());
        label->setPosition( it->second->getPositionX(), it->second->getPositionY() + 10);
    }
}
void GameWorld:: drawFood(cocos2d::DrawNode * drawFood){
    
//    printf("food size %d\n ", foods.size());
    for (auto it = foods.begin(); it!= foods.end();it++) {
        switch (it->second->getItem()) {
            case REFILL:
                drawFood->drawSolidCircle(it->second->getPosition(), m_foodRadius, 360, 10, 1, 1, cocos2d::Color4F::GREEN);
                break;
            case SLOWDOWN:
                 drawFood->drawSolidCircle(it->second->getPosition(), m_foodRadius, 360, 10, 1, 1, cocos2d::Color4F::RED);
                break;
            case OBSTACLE:
                drawFood->drawSolidCircle(it->second->getPosition(), m_foodRadius, 360, 10, 1, 1, cocos2d::Color4F::BLACK);
                break;
            case LEFT:
                drawFood->drawSolidCircle(it->second->getPosition(), m_foodRadius, 360, 10, 1, 1, cocos2d::Color4F::YELLOW);
                break;
            case RIGHT:
                drawFood->drawSolidCircle(it->second->getPosition(), m_foodRadius, 360, 10, 1, 1, cocos2d::Color4F::BLUE);
                break;
            default:
                break;
        }
        
        
    }
}
void GameWorld:: deleteNode(int id, cocos2d::Layer *layer){
    if (foods.find(id ) != foods.end()) {
        layer->removeChild(foods.at(id));
        foods.erase(id);
//         printf("delete Sucess \n");
    }
    else {
        printf("cant find node to delete \n");
    }
}
void GameWorld::deletePlayer(int id, cocos2d::Layer* layer ){
    if (m_others.find(id) != m_others.end()) {
        layer->removeChild(m_others.at(id));
        m_others.erase(id);
        printf("delete player sucess\n");
        
    }
    else
         printf("cant find player to delete \n");
}
void GameWorld::insertBullet(float x, float y, int id, cocos2d:: Layer *layer) {
    if (m_bullet.find(id) != m_bullet.end()) {
        Bullet* bullet = Bullet::createBullet(x, y, m_bulletRadius, id);
        cocos2d::Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
        bullet->getTexture()->setTexParameters(texParams);
        
        bullet->setScale(2);
        m_bullet.insert(std::make_pair(id,bullet));
        layer->addChild(bullet,2);

    }
  
    
    
}
void GameWorld::updateBullet(float x, float y, int time,int id,cocos2d:: Layer *layer ) {
    if (m_bullet.find(id) == m_bullet.end()) {
        Bullet* bullet = Bullet::createBullet(x, y, m_bulletRadius, id);
        cocos2d::Texture2D::TexParams texParams = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
        bullet->getTexture()->setTexParameters(texParams);
         bullet->setScale(2);
        m_bullet.insert(std::make_pair(id,bullet));
        layer->addChild(bullet,2);
    }
    else
        m_bullet.at(id)->insertCommand(x, y, time);
}
void GameWorld::drawBullet(cocos2d::DrawNode *drawBullet) const {
    for (auto it = m_bullet.begin(); it != m_bullet.end(); it++ ){
        drawBullet->drawSolidCircle(it->second->getPosition(), m_bulletRadius, 360, 10, 1, 1, cocos2d::Color4F::ORANGE);
    }
}
void GameWorld::removeBullet(int id,cocos2d:: Layer *layer  ) {
    if (m_bullet.find(id) != m_bullet.end()) {
        layer->removeChild(m_bullet.at(id));
        m_bullet.erase(id);
        
    }
    else
    {
         printf("cant find bulllet with id:%d ", id);
        for (auto it = m_bullet.begin(); it!= m_bullet.end(); it++ ) {
            printf("bullet id: %d\n", it->first);
        }
    }
    
    
}
