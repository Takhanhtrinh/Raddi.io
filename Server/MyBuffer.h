#ifndef MY_BUFFER_H
#define MY_BUFFER_H
#include <iostream>

class MyBuffer
{
  public:
    MyBuffer();

    void create(int size);
    void append(char *buf, int size);
    int getSizeLeft() const { return m_size - m_writeTo; }
    char *getBuf() const { return m_buf; }
    int getSize() const { return m_size; }
    int getWriteTo() const { return m_writeTo; }
    void justWrite(int size);
    ~MyBuffer(){if (m_delete) free(m_buf);}

  private:
    char *m_buf;

    int m_size;
    int m_writeTo;
    bool m_delete;
};

#endif
