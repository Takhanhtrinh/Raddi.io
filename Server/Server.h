#ifndef SERVER_H
#define SERVER_H
#pragma once
#include "Player.h"
#include "Grid.h"
#include "Food.h"
#include "Entity.h"
#include "memory"
#include "definition.h"
#include <unordered_map>
#include <event2/event.h>
#include <queue>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
//#include "protocol/Message.h"
const float SENDING_RATE = 50.0f;

enum CLIENT_MESSAGE
{
    CL_INIT_MSG = 1,
    CL_ANGLE_MSG = 2,
    CL_SHOOT_MSG = 3,
    

};
struct handleDisconnect
{
    struct event *event;
    struct timeval five_second;
};
class GameServer
{
  public:
    GameServer(short port);
    void run();
    ~GameServer();
    static void invoke_cb_init(int fd, short what, void *agr);
    void cb_init(int fd, short what);
    static void invoke_cb_accept(int fd, short what, void *agr);
    void cb_accept(int fd, short what);
    static void invoke_cb_read(int fd, short what, void *agr);
    static void invoke_cb_disconnect(int fd, short what, void *agr);
    void cb_read(int fd, short what);
    static void invoke_cb_write(int fd, short what, void *agr);
    void cb_write(int fd, short what);
    static int setNonBlock(int fd);
    void readPackage(char *buf, uint8_t type, Player &player);
    void playerUpdate(float dt);
    void bulletUpdate(float dt);

    static long getCurrentTime();
    void checkCollide();
    void updateGrid();
    void sendCurrentPlayerPos();
    void broadcast();
    void processCommand();
    void generateFood();
    void sendFoodEat();
    void leaderBoard();
    // kick a  player if dont get msg within 5 seconds
    void disconnectPlayer(int fd, short what);

  private:
  std::vector<Food> node_collide;
//   std::vector <Bullet> bullet_delete;
    float m_sendingTick;
    std::unique_ptr<Grid> m_grid;
    std::unordered_map<int, Player> players;
    std::unordered_map<int, Bullet> bullets;
    struct event_base *m_base;
    struct event_base *w_base;
    struct event_base *m_loop;
    struct event *read_event;
    struct event *send_init;
    struct event *first;
    handleDisconnect m_handle;

    short m_port;
    int playerCount;
};
#endif