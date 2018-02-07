#include "common.h"
#include <time.h>
#include <sys/time.h>

int distanceOf2Points(float x1, float y1, float x2, float y2) {
    int x = pow(x2 - x1, 2);
    int y = pow(y2 - y1, 2);
    return sqrt(x+y);
}
int myHash(int x, int y) {
    int key;
    
    key = MAXIMUM_PLAYER + distanceOf2Points(MAPSIZE / 2 ,MAPSIZE / 2,x,y) + y;
    
    return key;
}
int generateBulletID(int x, int y, int size) {
    int key;
    key = MAXIMUM_PLAYER + distanceOf2Points(MAPSIZE / 2 ,MAPSIZE / 2,x,y) + y + size + 1;
    return key;
}
float angleOf2Points(float x1, float y1, float x2, float y2) {
//    float x = m_lastTouch.x - m_firstTouch.x;
//    float y = m_lastTouch.y - m_firstTouch.y;
//    m_angle = atan2(y,x);
    float x = x2 - x1;
    float y = y2 - y1;
    return atan2(y,x);
}
void writeUint8(unsigned char *buf,uint8_t number, int offset ){
    buf[offset] = number & 0xff;
}
void writeUint16(unsigned char *buf, uint16_t number,int offset){
    uint8_t byte1;
    uint8_t byte2;
    byte2 = number >> 8 & 0xff;
    byte1 = number & 0xff;
    buf[offset] = byte1;
    buf[offset + 1] =byte2;
    // memcpy(buf + offset,&number,2 );
}
void writeString(unsigned char *buf, char *data, int len, int offset){
    memcpy(buf + offset, data,len);
}
uint8_t readUint8(unsigned char *buf,int offset) {
    uint8_t result;
    return result = buf[offset];
}
uint16_t readUint16(unsigned char *buf, int offset){
    uint16_t result;
    uint16_t byte1;
    uint16_t byte2;
    byte1 = buf[offset];
    byte2 = buf[offset + 1] << 8;
    
    result = byte2  | byte1;
    //memcpy(&result, buf + offset,2);
    return result;
}

uint32_t readUint32(unsigned char *buf, int offset){
    uint32_t result;
    uint32_t byte1;
    uint32_t byte2;
    uint32_t byte3;
    uint32_t byte4;
    byte1 = buf[offset];
    byte2  = buf[offset + 1] << 8;
    byte3 = buf[offset + 2 ] << 16;
    byte4 = buf[offset + 3] << 24;
    result = byte4 | byte3 | byte2 | byte1;
    return result;
}
void writeUint32(unsigned char *buf, uint32_t number,int offset){
    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;
    uint8_t byte4;
    byte4 = number >> 24 & 0xff;
    byte3 = number >> 16 & 0xff;
    byte2 = number >> 8 & 0xff;
    byte1 = number & 0xff;
    buf[offset] = byte1;
    buf[offset + 1 ] = byte2;
    buf[offset + 2 ] = byte3;
    buf[offset + 3 ] = byte4;
    
}
char *readString(unsigned char *buf,int len, int offset){
    char *result = (char*) malloc(len);
    memcpy(result,buf+offset,len);
    return result;
}
float getMin(float frameTime,float dt){
    if (frameTime < dt)
        return frameTime;
    else
        return dt;
}
bool overlap(const Entity &e, const Entity &e1){
    if (e.getID() == e1.getID()) return false;
    float radius = e.getRadius() + e1.getRadius();
    if (distanceOf2Points(e.getPositionX(), e.getPositionY(), e1.getPositionX(), e1.getPositionY()) < radius) return true;
    return false;
}
Point respawnWithinCircle(int x, int y, int radius, int smallCircle){
    Point pos;
   
    
    float angle = rand() % 360;
    angle = angle  * PI / 180;
   
    pos.m_x = cos(angle) * smallCircle + x + cos(angle) * (radius  - smallCircle) / 2 ;
    pos.m_y =  sin(angle) * smallCircle + y + sin(angle) * (radius  - smallCircle) / 2;
    return pos;
}


int myRand(int from, int to){
    srand(time(NULL));
    int rt = rand() % to + from;
    return rt;
}
bool checkOutsideOfMap(float x, float y, float radius, float centerX, float centerY, float mapRadius){
    float sradius = radius + mapRadius;
    if (distanceOf2Points(x, y, centerX, centerY) <= sradius){
        return true;
    }
    
    
    return false;
    
}
long getCurrentTime(){
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_usec;
}
float lerp(float v0, float v1, float t) {
    return (1 - t) * v0 + t * v1;
}
void GAMELOG(const char * func, const char * content){
    printf("<%s> %s\n",func,content);
}
int findTick(int frameRate, int ms){
    int tick;
    tick = ms * frameRate/1000;
    return tick;
}
Point findTarget(Point pos,float angle) {
    Point rt;
    rt.m_x = cos(angle) * PLAYER_RADIUS + pos.m_x;
    rt.m_y = sin(angle) * PLAYER_RADIUS  + pos.m_y;
    return rt;
}
Point findDesiredVec(Point target, Point pos ){
    Point rt;
    rt.m_x = target.m_x - pos.m_x;
    rt.m_y = target.m_y - pos.m_y;
    return rt;
}
Point findSterring(Point desired, Point vec ){
    Point rt;
    rt.m_x = desired.m_x - vec.m_x;
    rt.m_y = desired.m_y - vec.m_y;
    return rt;
}