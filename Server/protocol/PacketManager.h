#ifndef PACKETMANAGER_H
#define PACKETMANAGER_H
#include "Packet.h"
#include <iostream>
class PacketManager {
    public:
    PacketManager(){};
    static Packet *create(int type); 
    static void deletePacket(Packet *packet);
    ~PacketManager(){};


};



#endif