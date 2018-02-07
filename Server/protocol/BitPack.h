#ifndef BITPACK_H
#define BITPACK_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "common.h"

 struct Buffer{
        char *buf;
        int index;
        int len;
    };

class BitWriter {
    public:

    BitWriter(int maxBuffer);
    void writeByte( uint8_t data);
    void writeShort( uint16_t data);
    void writeFloat(const FloatInt &data);
    void writeString(const char *ch,int len);
    int canWrite() const;
    char* getBuf() const {return m_buf;}
    int getLen() const {return m_len;}

    ~BitWriter();
    


    private:
     char *m_buf;
    int m_index;
    int m_len;
    int m_max;
};
class BitReader{
    public:
    BitReader(char * buf, int len);
   void readByte(uint8_t *data) ;    
   void readShort(uint16_t *data) ;
   void readString(char *&ch, int len) ;
   int canRead() const {return m_len - m_index;}
   char *getBuf() const {return (char*) m_buf;}
   ~BitReader();
    private:
   unsigned char *m_buf;
    int m_index;
    int m_len;
    
};
#endif