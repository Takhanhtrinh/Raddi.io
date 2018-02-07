#ifndef GAME_MESSAGE_H
#define GAME_MESSAGE_H
#include <iostream>
#include <stdint.h>
#include "protocol/common.h"

    class GameMessage{
        public:
        GameMessage(){};
        ~GameMessage(){};
        char *data;
        void init(char *data,int length);
        char *getNewPoint(int len);
        void clear();
        void writeByte(uint8_t c);
        void writeShort(uint16_t c);
        void writeFloat(FloatInt c);
        void beginRead() {m_offset = 0;}
        uint8_t readByte();
        uint16_t readShort();
        FloatInt readFloat();




        private:
        bool m_overflow;
        int m_size;
        int m_offset;
        int m_maxSize;

    };


#endif