//
//  MyBuffer.cpp
//  TestBuffer
//
//  Created by Trinh Ta on 3/7/17.
//  Copyright © 2017 Trinh Ta. All rights reserved.
//

#include "MyBuffer.hpp"
#include <assert.h>
#include <string.h>
MyBuffer::MyBuffer():
m_size(0),
m_currentSize(0),
m_sizeLeft(0),
m_buf(nullptr),
m_canFree(false)

{m_type[0] = 0;};
bool MyBuffer::isFull() const {
    if (m_sizeLeft > 0)
        return false;
    return true;
}
void MyBuffer::append(char *buf, int len) {
    assert(buf != nullptr);
    assert(len + m_currentSize <= m_size);
    if (!isFull()) {
        memcpy(m_buf + m_currentSize,buf, len);
        m_currentSize += len;
        m_sizeLeft = m_size - m_currentSize;
    }
}
void MyBuffer::create(char type, int size) {
    assert(size > 0);
    m_size = size;
    m_buf = (char*) malloc(size);
    m_type[0] = type;
    m_canFree= true;
}
void MyBuffer::reset(){
    if (m_canFree){
        m_size =0;
        m_currentSize = 0;
        m_sizeLeft = 0;
        assert(m_buf != nullptr);
        free(m_buf);
        m_canFree = false;
        m_type[0] = 0;
    }
}
MyBuffer::~MyBuffer(){
    if (m_canFree)
        free(m_buf);
}
