#ifndef PACKET_HPP
#define PACKET_HPP

#define MAX_FOOD 50

#include <stdint.h>
#include <iostream>
#include "definition.h"
#include "BitWriter.hpp"
enum PacketTypes{
    INIT_PACKET = 'I',
    MOVE_PACKET = 'P',
    MOVE_PACKET_WITH_HEALTH ='p',
    NEW_PLAYER_PACKET = 'E',
    UPDATE_NODE_PACKET = 'A',
    PING_PACKET = 'N',
    FOOD_PACKET = 'F',
    FOOD_RM_PACKET = 'R',
    PLAYER_RM_PACKET = 'H',
    BULLET_PACKET = 'B',
    UPDATE_BULLET_PACKET = 'C',
    REMOVE_BULLET_PACKET = 'K'
    
};
const int MOVE_PACKET_SIZE = 9;
const int INIT_PACKET_SIZE = 15;
const int NEW_PLAYER_PACKET_SIZE = 7;
const int UPDATE_NODE_SIZE = 6;
const int PING_PACKET_SIZE = 1;
const int FOOD_PACKET_SIZE = 8;
const int FOOD_RM_PACKET_SIZE = 3;
const int PLAYER_RM_PACKET_SIZE = 2;
const int BULLET_PACKET_SIZE = 7;
const int UPDATE_BULLET_PACKET_SIZE = 7;
const int REMOVE_BULLET_PACKET_SIZE = 3;
const int MOVE_PACKET_WITH_HEALTH_SIZE = 10;


class Packet{
public:
    Packet(int type):m_type(type){};
    int getType() const{return m_type;}
    virtual void serialize(BitWriter & wr) = 0;
    virtual ~Packet(){};
    int getSize() const {return m_size;}
protected:
    int m_size;
private:
    uint8_t m_type;
};
struct Packet_P : public Packet{
    uint16_t x;
    uint16_t y;
    Packet_P() : Packet(MOVE_PACKET){m_size = MOVE_PACKET_SIZE;};
    void serialize(BitWriter & wr);
};
struct Packet_I : public Packet {
    Packet_I(): Packet(INIT_PACKET){m_size = INIT_PACKET_SIZE;};
    uint16_t mapsize;
    uint16_t centerX;
    uint16_t centerY;
    uint16_t smallCircleRadius;
    uint16_t moveSpeed;
    uint8_t playerRadius;
    uint8_t foodRadius;
    uint8_t bulletRadius;
    uint8_t itemRadius;
    uint16_t x;
    uint16_t y;
    void serialize(BitWriter & wr);
    
    
};
struct Packet_N: public Packet{
    Packet_N(int namelen);
    uint8_t id;
    uint16_t x;
    uint16_t y;
    void serialize(BitWriter & wr);
    uint8_t nameLength;
    char name[NAME_LENGTH];
    
};
struct Packet_M: public Packet{
    Packet_M(): Packet(UPDATE_NODE_PACKET){m_size = UPDATE_NODE_SIZE;}
    uint8_t id;
    uint16_t x;
    uint16_t y;
    void serialize(BitWriter & wr);
    
};
struct Packet_Ping: public Packet{
    Packet_Ping(): Packet(PING_PACKET){m_size = PING_PACKET_SIZE;}
    void serialize(BitWriter & wr);
};




#endif
