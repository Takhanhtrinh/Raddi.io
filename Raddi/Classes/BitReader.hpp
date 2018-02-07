//
//  BitReader.hpp
//  Raddi
//
//  Created by Trinh Ta on 2/20/17.
//
//

#ifndef BitReader_hpp
#define BitReader_hpp

#include <iostream>
#include "Common.hpp"
#include <stdint.h>
#include "Packet.hpp"
class BitReader{
public:
    BitReader(char * buf, int type);
    int readByte();
    int readShort();
    int readInt();
    float readFloat();
    char* readString(int len);
    char* getNewPoint(int index);
private:
    char *m_buf;
    int m_max;
    int m_index;
};

#endif /* BitReader_hpp */
