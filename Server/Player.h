#ifndef PLAYER_H
#define PLAYER_H
#pragma once
#include "Entity.h"
#include <unordered_map>
#include <deque>
#include <event2/event.h>
#include <netinet/in.h>
#include <vector>
#include <stdint.h>
#include "AABB.cpp"
#include "Food.h"
#include "Bullet.h"
#include "MyBuffer.h"
#include "MovementEntity.h"

// enum CLIENT_MESSAGE{
//     CL_ANGLE_MSG = 2,
//     CL_BOOST_MSG = 4,
//     CL_ROTATE_MSG = 8,
//     CL_CMD = 5
// };
const int SENDING_OLD_NODE = 250;
const int BULLET_DELAY = 1000;
class Player : public MovementEntity
{
public:
  Player(char *name, int nameLen, int id, float x, float y, int radius, float angle);
  Player(const Player &player);
  Player(int fd);
  ~Player();

  void update(float dt);
  void setLastPos(float x, float y);
 


  bool getCanShoot() const {return canShoot;}
  void shoot();
  bool getIsShoot() const { return isShoot; }
  void setIsShoot(bool b) { isShoot = b; }
  void justHit(){isChanged = true; m_health -= 20;}

  // AABB getBoxForCollide() const;
  void setAngle(float angle);
 

  int getItemTypeSize() const { return (int)m_itemType.size(); }
  void insertItem(int item);
  int getItem(int index) const { return m_itemType.at(index); }
  int getFrontItem();
  
  char *getName() const { return m_name; }

  int getNameLength() const { return m_nameLength; }
  void setNameLength(int len) { m_nameLength = len; }
  int getHealth() const {return m_health;}
  int getBulletSize() const{return m_bulletSize;}
  void setBulletSize(int v) {m_bulletSize = v;}
  void setHealth(int v) {m_health = v;}

  long getLastMessage() const { return lastMessage; }
  void setLastMessage(long tick) { lastMessage = tick; }
  Player &operator=(const Player &player);
  AABB getView() const { return m_view; }
  AABB getCurrentView() const ;
  AABB getDeleteView() const;
  void updateView();
  bool isDie() const { return m_die; }
  bool getInit() const { return m_init; }
  void setInit(bool b) { m_init = b; }
  void insertCommand(float angle) { m_command.push_back(angle); }
  std::deque<float> getCommand() { return m_command; }
  float getHeadingAngle() const {return m_headingAngle;}
  //packet_p sent with health if get shoot
  bool getIsChanged() const {return isChanged;}
  void setIsChanged(bool v ) {isChanged = v;} 
  
  void deleteNode();
  void sendNodeAroundMe();
  void deleteNodeEat(std::vector<Food> food);
  void sendDataInqueue();
 
  void filter(int id);
  struct event *read_ev;
  struct event *write_ev;

  std::unordered_map<int,Entity *> m_allNode;
  std::unordered_map<int, bool> m_sendNode;
  std::unordered_map<int, Food> old_node;
  std::unordered_map<int, Bullet> old_bullet;
  std::unordered_map<int, Player> old_player;
  std::deque<MyBuffer> m_buf;
private:
float m_speed;
float m_steering;
bool isChanged;
bool canShoot;
int m_bulletDelay;
float m_headingAngle;
int m_bulletSize;
int m_health;
  bool m_init;
  bool isShoot;
  int ticks;
  bool m_die;

  char *m_name;
  int m_nameLength;
 
  long lastMessage;
  std::deque<int> m_itemType;
  AABB m_view;
  std::deque<float> m_command;
};
#endif