#ifndef COMMON_H
#define COMMON_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../definition.h"
#include "../Entity.h"
#include <stdint.h>
#include "../AABB.cpp"
#include <sys/time.h>
#include <time.h>
union FloatInt{
    float float_value;
    uint32_t int_value;
};
int distanceOf2Points(float x1, float y1, float x2, float y2);
int myHash(int x, int y) ;
int generateBulletID(int x, int y,int size);
float angleOf2Points(float x1, float y1, float x2, float y2) ;
void writeUint8(unsigned char *buf,uint8_t number, int offset );
void writeUint16(unsigned char *buf, uint16_t number,int offset);
void writeUint32(unsigned char *buf, uint32_t number,int offset);
void writeString(unsigned char *buf, unsigned char *data, int len, int offset);
uint8_t readUint8(unsigned char *buf,int offset) ;
uint16_t readUint16(unsigned char *buf, int offset);
uint32_t readUint32(unsigned char *buf, int offset);
int findTick(int frameRate, int ms);
char *readString(unsigned char *buf,int len, int offset);
float getMin(float frameTime,float dt);
bool overlap(const Entity &e, const Entity &e1);
Point respawnWithinCircle(int x, int y, int radius, int smallCircle);
int myRand(int from, int to);
bool checkOutsideOfMap(float x, float y, float radius, float centerX, float centerY, float mapRadius);
long getCurrentTime();
void GAMELOG(const char * func, const char * content);
float lerp(float v0, float v1, float t);
Point findTarget(Point pos,float angle);
Point findDesiredVec(Point target, Point pos );
Point findSterring(Point desired, Point vec );
#endif
