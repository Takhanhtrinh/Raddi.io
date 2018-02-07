#include "BitPack.h"
#include "PacketManager.h"
#include "Packet.h"
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <stdint.h>


int main() {
   
   Packet *packet = PacketManager::create(INIT_PACKET);
   BitWriter wr(26);
   Packet_I *pk = nullptr;
   Packet_I *rdpk = nullptr;
   if (packet == nullptr){
       std::cout << " null" << std::endl;
   }
   if (packet->getType() == INIT_PACKET) 
    pk = dynamic_cast<Packet_I*> (packet);
   
   pk->x = 5440;
   pk->y = 3333;
   pk->name =(char*) "trinh ta";
   pk->serialize(wr);
   Packet *readpk = PacketManager::create(INIT_PACKET);
   if (readpk -> getType()== INIT_PACKET) {
       rdpk = dynamic_cast<Packet_I*> (readpk);
   }

   BitReader rd(wr.getBuf(), wr.getLen());
   uint8_t type;
   rd.readByte(&type);
   rd.readShort(&(rdpk->x));
   rd.readShort(&(rdpk->y));
   rdpk->name = (char*) malloc(NAME_LENGTH);
   rd.readString(rdpk->name,NAME_LENGTH);
 //  std:: cout << strlen(rdpk->name) << std::endl;
   std::cout << "name " << rdpk->name <<   " x:  " << rdpk->x <<"  y: " << rdpk->y << std::endl;
   
   
   PacketManager::deletePacket(packet);
   PacketManager::deletePacket(readpk);
   free(rdpk->name);
   
    //PacketManager::deletePacket(pk);
}