#include "Packet.hpp"

void Packet_P::serialize(BitWriter &wr){
    wr.writeByte(Packet::getType());
    wr.writeShort(x);
    wr.writeShort(y);
}
Packet_N :: Packet_N(int namelen):
Packet(NEW_PLAYER_PACKET),
nameLength(namelen){m_size = NEW_PLAYER_PACKET_SIZE + nameLength;}
void Packet_N:: serialize(BitWriter &wr){
    wr.writeByte(Packet::getType());
    wr.writeByte(id);
    wr.writeShort(x);
    wr.writeShort(y);
    wr.writeByte(nameLength);
    wr.writeString(name,nameLength);
}
void Packet_M ::serialize(BitWriter & wr){
    wr.writeByte(Packet::getType());
    wr.writeByte(id);
    wr.writeShort(x);
    wr.writeShort(y);
}
void Packet_I:: serialize(BitWriter & wr){
    wr.writeByte(getType());
    wr.writeShort(mapsize);
    wr.writeShort(centerX);
    wr.writeShort(centerY);
    wr.writeShort(smallCircleRadius);
    wr.writeShort(moveSpeed);
    wr.writeByte(playerRadius);
    wr.writeByte(foodRadius);
    wr.writeByte(bulletRadius);
    wr.writeByte(itemRadius);
    wr.writeShort(x);
    wr.writeShort(y);
}
void Packet_Ping :: serialize(BitWriter & wr){
    wr.writeByte(getType());
}

