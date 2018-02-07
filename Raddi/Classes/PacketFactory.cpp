//
//  PacketFactory.cpp
//  Raddi
//
//  Created by Trinh Ta on 2/20/17.
//
//
//class Packet{
//public:
//    Packet(int type):m_type(type){};
//    int getType() const{return m_type;}
//    virtual void serialize(BitWriter & wr) = 0;
//    virtual ~Packet(){};
//    int getSize() const {return m_size;}
//protected:
//    int m_size;
//private:
//    uint8_t m_type;
//};
//struct Packet_P : public Packet{
//    uint16_t x;
//    uint16_t y;
//    Packet_P() : Packet(MOVE_PACKET){m_size = MOVE_PACKET_SIZE;};
//    void serialize(BitWriter & wr);
//};
//struct Packet_I : public Packet {
//    Packet_I(): Packet(INIT_PACKET){m_size = INIT_PACKET_SIZE;};
//    uint16_t mapsize;
//    uint16_t centerX;
//    uint16_t centerY;
//    uint16_t smallCircleRadius;
//    uint16_t moveSpeed;
//    uint8_t playerRadius;
//    uint8_t foodRadius;
//    uint8_t bulletRadius;
//    uint8_t itemRadius;
//    uint16_t x;
//    uint16_t y;
//    void serialize(BitWriter & wr);
//    
//    
//};
//struct Packet_N: public Packet{
//    Packet_N(int namelen);
//    uint8_t id;
//    uint16_t x;
//    uint16_t y;
//    void serialize(BitWriter & wr);
//    uint8_t nameLength;
//    char name[NAME_LENGTH];
//    
//};
//struct Packet_M: public Packet{
//    Packet_M(): Packet(UPDATE_NODE_PACKET){m_size = UPDATE_NODE_SIZE;}
//    uint8_t id;
//    uint16_t x;
//    uint16_t y;
//    void serialize(BitWriter & wr);
//    
//};


#include "PacketFactory.hpp"
Packet* PacketFactory:: createPacket(int type, int len){
    switch (type) {
        case INIT_PACKET_SIZE:
            return new Packet_I();
            break;
        case MOVE_PACKET:
            return new Packet_P();
            break;
        case NEW_PLAYER_PACKET:
            return new Packet_N(len);
            break;
        case UPDATE_NODE_SIZE:
            return new Packet_M();
            break;
        default:
            printf("<PacketFactory> no packet type");
            return nullptr;
            
            break;
    }
}
