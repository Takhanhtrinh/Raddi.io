//
//  AABB.hpp
//  testClient
//
//  Created by Trinh Ta on 1/22/17.
//
//

#ifndef AABB_hpp
#define AABB_hpp


#include <iostream>
namespace tt{
    struct Point{
        Point(): m_x(0), m_y(0) {};
        Point(float x, float y) : m_x(x), m_y(y){};
        float m_x;
        float m_y;
        bool operator ==(const Point &point ) const {
            if (m_x == point.m_x && m_y == point.m_y) return true;
            return false;
        }
        bool operator != (const Point &point) const {
            if (*this == point ) return false;
            return true;
            
        }
    };
};
using namespace tt;
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
        if (m_center.m_x + m_halfDim.m_x > aabb.m_center.m_x - aabb.m_halfDim.m_x ||
            m_center.m_x - m_halfDim.m_x < aabb.m_center.m_x + aabb.m_halfDim.m_x ){
            if (m_center.m_y - m_halfDim.m_y < aabb.m_center.m_y + aabb.m_halfDim.m_y ||
                m_center.m_y + m_halfDim.m_y > aabb.m_center.m_y - aabb.m_halfDim.m_y) {
                return true;
            }
        }
        return false;
    }
    
    
    
    
};


#endif /* AABB_hpp */
