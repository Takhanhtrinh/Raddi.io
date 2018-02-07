#include "BitPack.h"
#include <string.h>
BitWriter::BitWriter(int maxBuffer):
m_len(0),
m_index(0),
m_max(maxBuffer)
{
     m_buf = (char *) malloc(m_max);
}
BitWriter::~BitWriter() {
    if(m_buf!= NULL)
    free(m_buf);
}
void BitWriter::writeByte( uint8_t data) {
    if (m_len + 1 <= m_max){
        m_buf[m_index] = data & 0xff;
        m_index++;
        m_len++;
        
    }
    else
     printf("warm: writeByte: m_index + len ");
}
void BitWriter::writeShort( uint16_t data) {
    if (m_len + 2 <= m_max) {
        uint8_t byte1;
        uint8_t byte2;
        byte1 = data & 0xff;
        byte2 = (data >> 8) & 0xff;
        *(m_buf + m_index++) = byte1;
        *(m_buf + m_index++) = byte2;
        m_len += 2;
    }else
     printf("warm: writeShort: m_index + len ");
}
void BitWriter::writeFloat(const FloatInt &data) {
    if (m_len + 4 <= m_max) {
    uint32_t number = data.int_value;
    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;
    uint8_t byte4;
    byte4 = number >> 24 & 0xff;
    byte3 = number >> 16 & 0xff;
    byte2 = number >> 8 & 0xff;
    byte1 = number & 0xff;
    m_buf[m_index++] = byte1;
    m_buf[m_index++] = byte2;
    m_buf[m_index++] = byte3;
    m_buf[m_index++] = byte4;
    m_len += 4;
    }
    else
         printf("warm: writeShort: m_index + len ");
}
void BitWriter::writeString(const char *ch, int len){
    if (m_len + len <= m_max) {
        memcpy(m_buf + m_index, ch, len);
        m_index += len;
        m_len += len;
    }else
     printf("warm: writeString: m_index + len ");
}
int BitWriter::canWrite() const {
    return m_max - m_len;
}
BitReader::BitReader( char *buf, int len):
m_len(len),
m_index(0){
    m_buf =(unsigned char*) buf;
};

void BitReader::readByte(uint8_t *data)  {
    if (m_index <= m_len) {
        *data = *(m_buf + m_index++);

    }else
     printf("warm: readByte: m_index + len ");
}
void BitReader::readShort(uint16_t *data) {
    if (m_index  + 2 <= m_len ) {
        uint16_t byte1 = *(m_buf + m_index++);
        uint16_t byte2 = *(m_buf + m_index++) << 8;
        *data = byte2 | byte1;

    }else
    printf("warm: readShort: m_index + len ");
}
void BitReader::readString(char *&ch, int len) {
    
    if (m_index + len <= m_len) {
        memcpy(ch, m_buf, len);
        m_index+= len;
    }
    else 
        printf("warm:readString: m_index + len ");
    
} 
BitReader::~BitReader() {
   
}