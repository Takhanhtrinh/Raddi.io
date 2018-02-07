#include "MyBuffer.h"
#include <assert.h>
#include <stdlib.h>

void MyBuffer::create(int size)
{
    assert(size > 0);
    m_buf = (char *)malloc(size);
    m_size = size;
    m_writeTo = 0;
    m_delete = true;
}
void MyBuffer::append(char *buf, int size)
{
    assert(buf != nullptr);
    assert(size + m_writeTo <= m_size);
    memcpy(m_buf + m_writeTo,buf, size );
}
void MyBuffer::  justWrite(int size){
    assert(m_writeTo + size <= m_size);
    m_writeTo += m_size;
}
MyBuffer::MyBuffer() : m_size(0),
                       m_writeTo(0),
                       m_delete(false),
                       m_buf(nullptr) {}