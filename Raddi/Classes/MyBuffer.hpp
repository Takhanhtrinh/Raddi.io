//
//  MyBuffer.hpp
//  TestBuffer
//
//  Created by Trinh Ta on 3/7/17.
//  Copyright © 2017 Trinh Ta. All rights reserved.
//

#ifndef MyBuffer_hpp
#define MyBuffer_hpp

#include <iostream>
#include <stdlib.h>
class MyBuffer{
public:
    MyBuffer();
    //    static MyBuffer *create(int size, char * buf);
    void create(char type,int size);
    void setType(char type) {m_type[0] = type;}
    char getType() const {return m_type[0];}
    bool isFull() const;
    int getSizeLeft() const  {return m_sizeLeft;}
    void reset();
    void append(char * buf, int len);
    char *getBuf()const {return m_buf;}
    ~MyBuffer();
    
private:
    char *m_buf;
    int m_size;
    int m_currentSize;
    int m_sizeLeft;
    bool m_canFree;
    char m_type[1];
};

#endif /* MyBuffer_hpp */
