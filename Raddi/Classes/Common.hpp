#ifndef COMMON_H
#define COMMON_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definition.h"
#include "Entity.hpp"
#include <stdint.h>
#include "AABB.hpp"
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

union FloatInt{
    float float_value;
    uint32_t int_value;
};
int distanceOf2Points(float x1, float y1, float x2, float y2);
int myHash(int x, int y) ;
float angleOf2Points(int x1, int y1, int x2, int y2) ;
void writeUint8(unsigned char *buf,uint8_t number, int offset );
void writeUint16(unsigned char *buf, uint16_t number,int offset);
void writeUint32(unsigned char *buf, uint32_t number,int offset);
void writeString(unsigned char *buf, unsigned char *data, int len, int offset);
uint8_t readUint8(unsigned char *buf,int offset) ;
uint16_t readUint16(unsigned char *buf, int offset);
uint32_t readUint32(unsigned char *buf, int offset);
char* readString(unsigned char *buf,int len, int offset);
float getMin(float frameTime,float dt);
bool overlap(const Entity &e, const Entity &e1);
Point respawnWithinCircle(int x, int y, int radius, int offset);
int myRand(int from, int to);
bool checkOutsideOfMap(float x, float y, float radius, float centerX, float centerY, float mapRadius);
long getCurrentTime();
float lerp(float v0, float v1, float t);
inline void GAMELOG(const char *ch, const char *content){printf("<%s> %s\n",ch,content);}
  int ReadAll(int sockfd, char *buf, int size);
#endif
