//
//  BitReader.cpp
//  Raddi
//
//  Created by Trinh Ta on 2/20/17.
//
//

#include "BitReader.hpp"
BitReader::BitReader(char * buf, int type) :

m_index(0),
m_buf(buf){
    switch (type) {
        case MOVE_PACKET:
            m_max = MOVE_PACKET_SIZE;
            break;
            
        case UPDATE_NODE_PACKET:
            m_max = UPDATE_NODE_SIZE;
            break;
        case INIT_PACKET:
            m_max = INIT_PACKET_SIZE;
            break;
    }
};

int BitReader:: readByte(){
    if (m_index + 1 < m_max){
        uint8_t rt = readUint8((unsigned char*)m_buf, 0);
        m_buf = getNewPoint(1);
        m_index++;
        return rt;
    }
    printf("<readByte>m_index + 1 < m_max \n");
    return -1;
}
int BitReader:: readShort() {
    if (m_index + 2 < m_max) {
        uint16_t rt = readUint16((unsigned char*)m_buf, 0);
        m_buf = getNewPoint(2);
        m_index += 2;
        return rt;
    }
    printf("<readShort>m_index + 2 < m_max \n");
    return -1;
}
int BitReader:: readInt(){
    if (m_index + 4 < m_max) {
        uint16_t rt = readUint32((unsigned char*)m_buf, 0);
        m_buf = getNewPoint(4);
        m_index += 2;
        return rt;
    }
    printf("<readInt>m_index + 4 < m_max \n");
    return -1;
}

float BitReader:: readFloat(){
    if (m_index + 4 < m_max) {
        FloatInt rt;
        rt.int_value = readUint32((unsigned char*)m_buf, 0);
        m_buf = getNewPoint(4);
        m_index += 2;
        return rt.float_value;
    }
    printf("<readFloat>m_index + 4 < m_max \n");
    return -1.0f;
}

char* BitReader:: readString(int len){
    if (m_index + len < m_max) {
        char *rt = (char*)malloc(len);
        memcpy(rt, m_buf, len);
        m_buf = getNewPoint(len);
        m_index += len;
        return rt;
        
    }
    return NULL;
}

char* BitReader:: getNewPoint(int index){
    if (m_index + index < m_max)
        return m_buf + index;
    return NULL;
}
