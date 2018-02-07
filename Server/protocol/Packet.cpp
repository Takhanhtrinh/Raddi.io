#include "Packet.h"
void Packet_P::serialize(BitWriter &wr){
    wr.writeByte(Packet::getType());
    wr.writeShort(x);
    wr.writeShort(y);
    wr.writeFloat(angle);
}
void Packet_p::serialize(BitWriter &wr){
     wr.writeByte(Packet::getType());
    wr.writeShort(x);
    wr.writeShort(y);
    wr.writeByte(health);
    wr.writeFloat(angle);
}
   Packet_N :: Packet_N(int namelen):
   Packet(NEW_PLAYER_PACKET),
   nameLength(namelen){m_size = NEW_PLAYER_PACKET_SIZE + nameLength;}
void Packet_N:: serialize(BitWriter &wr){
    wr.writeByte(Packet::getType());
     wr.writeByte(nameLength);
    wr.writeByte(id);
    wr.writeShort(x);
    wr.writeShort(y);
   
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
//    wr.writeShort(x);
//    wr.writeShort(y);
}
void Packet_Ping :: serialize(BitWriter & wr){
    wr.writeByte(getType());
} 


void Packet_food :: serialize(BitWriter & wr){
    wr.writeByte(getType());
    wr.writeShort(id);
    wr.writeByte(type);
    wr.writeShort(x);
    wr.writeShort(y);
}
void Packet_rmFood :: serialize(BitWriter &wr){
    wr.writeByte(getType());
    wr.writeShort(id);
}
void Packet_rmPlayer::serialize(BitWriter &wr){
    wr.writeByte(getType());
    wr.writeByte(id);
}
void Packet_Bullet:: serialize(BitWriter &wr) {
    wr.writeByte(getType());
    wr.writeShort(id);
    wr.writeShort(x);
    wr.writeShort(y);
}
void Packet_Update_Bullet::serialize(BitWriter &wr) {
    wr.writeByte(getType());
    wr.writeShort(id);
    wr.writeShort(x);
    wr.writeShort(y);
}
void Packet_rmBullet:: serialize(BitWriter &wr){
    wr.writeByte(getType());
    wr.writeShort(id);
}