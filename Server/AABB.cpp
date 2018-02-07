#ifndef AABB_CPP
#define AABB_CPP
#include <iostream>
#include <math.h>

struct Point{
    Point(): m_x(0), m_y(0) {};
    Point(float x, float y) : m_x(x), m_y(y){};
    Point (const Point& p) : m_x(p.m_x), m_y(p.m_y){};
    float m_x;
    float m_y;
    void normalize(){
        float length = sqrt((m_x * m_x ) + (m_y * m_y));
        m_x = m_x / length;
        m_y = m_y / length;
    }
    void addMagnitude(float value) {
        m_x = m_x * value;
        m_y = m_y * value;
    }
    void limit(float v) {
        normalize();
        addMagnitude(v);
    }
    float length() const {
        float rt;
        rt = sqrt((m_x * m_x ) + (m_y * m_y ));
        return rt;
     
    }
};
struct AABB{
    Point m_center;
    Point m_halfDim;
    AABB(){
        m_center.m_x = 0;
        m_center.m_y = 0;
        m_halfDim.m_x = 0;
        m_halfDim.m_x = 0;
    }
   
    AABB(Point center, Point halfDim):
    m_center(center), m_halfDim(halfDim){};
    
    bool isCotainPoint(const Point &point) const {
        if (point.m_x < m_center.m_x + m_halfDim.m_x && point.m_x > m_center.m_x - m_halfDim.m_x) {
            if (point.m_y < m_center.m_y + m_halfDim.m_y && point.m_y > m_center.m_y - m_halfDim.m_y) 
            return true;
        }
        return false;
    }
    bool intersects(const AABB &aabb) const {
        if (m_center.m_y - m_halfDim.m_y <= aabb.m_center.m_y + aabb.m_halfDim.m_y && m_center.m_y + m_halfDim.m_y >= aabb.m_center.m_y - aabb.m_halfDim.m_y &&
              m_center.m_x + m_halfDim.m_x >= aabb.m_center.m_x - aabb.m_halfDim.m_x && m_center.m_x - m_halfDim.m_x <= aabb.m_center.m_x + aabb.m_halfDim.m_x)
              return true;
          
          return false;
    }
    


};
#endif