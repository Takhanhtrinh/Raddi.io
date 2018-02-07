#ifndef BITWRITER_HPP
#define BITWRITER_HPP

#include <iostream>
#include <Common.hpp>
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
#endif
