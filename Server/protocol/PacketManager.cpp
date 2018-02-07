#include "PacketManager.h"
Packet* PacketManager:: create(int type ) {
    switch(type){
        case INIT_PACKET:
            return new Packet_I();
            break;
        case MOVE_PACKET:
            return new Packet_P();
            break;
        case NEW_PLAYER_PACKET:
            return new Packet_N(2);
            break;
    }
    return nullptr;
}
void PacketManager::deletePacket(Packet *packet) {
    if (packet != nullptr) 
        delete packet;
}