#include "GameMessage.h"
void GameMessage::init(char *d, int length) {
    data = d;
    m_offset = 0;
    m_overflow =false;
    m_maxSize = length;
    m_size = 0;
}
void GameMessage::clear() {
    m_size =0;
    m_overflow = false;
    m_offset =0;
}
char* GameMessage::getNewPoint(int len){
    char *temp;
    if (m_size + len > m_maxSize ){
        clear();
        m_overflow =true;
    }
    temp = data + m_size;
    m_size +=len;
    return temp;
}

void GameMessage::writeByte(uint8_t c){
    char *buf = getNewPoint(1);
    writeUint8((unsigned char*)(buf),c,0);
}
void GameMessage::writeShort(uint16_t c ){
    char *buf = getNewPoint(2);
    writeUint16((unsigned char*)(buf),c,0);
}
void GameMessage::writeFloat(FloatInt c){
    char *buf = getNewPoint(4);
    writeUint32((unsigned char*)buf,c.int_value,0);
}
uint8_t GameMessage::readByte(){
    uint8_t rt;
    if (m_offset + 1 > m_size)
        rt = 0;
    else
        rt = readUint8((unsigned char*)data + m_offset, 0);
    m_offset++;
    return rt;
    
}
uint16_t GameMessage::readShort() {
    uint16_t rt;
    if (m_offset + 2 > m_size)
        rt = 0;
    else
        rt = readUint16((unsigned char*)data + m_offset , 0);
    m_offset +=2 ;
    return rt;
    
}
FloatInt GameMessage::readFloat(){
    FloatInt rt;
    if (m_offset + 4 > m_size)
        rt.float_value = -1;
    else
        rt.int_value = readUint32((unsigned char*)data + m_offset,0);
    return rt;
}