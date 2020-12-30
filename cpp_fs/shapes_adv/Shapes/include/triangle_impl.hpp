#ifndef SHAPES_TRIANGLE_IMPL
#define SHAPES_TRIANGLE_IMPL

#include <iosfwd>
#include <cmath>//sin cos
#include "point.hpp"
#include "triangle.hpp"

namespace ilrd
{
    Triangle::Triangle()
    :m_p1(Point(0,0)),
    m_p2(Point(10,10)),
    m_p3(Point(0,10))
    {
    }
    
    Triangle::Triangle(double x1 , double y1 ,
                            double x2 , double y2 ,
                            double x3, double y3):
    m_p1(Point(x1,y1)),
    m_p2(Point(x2,y2)),
    m_p3(Point(x3,y3))
    {
    }
    Triangle& Triangle::Move(double xDelta, double yDelta)
    {
        Step(&m_p1, xDelta, yDelta);
        Step(&m_p2, xDelta, yDelta);
        Step(&m_p3, xDelta, yDelta);
        return *this;
    }
    Triangle& Triangle::Revolve(double angle ,const Point& pivot)
    {
        m_p1.Revolve(pivot, angle);
        m_p2.Revolve(pivot, angle);
        m_p3.Revolve(pivot, angle);
        return *this;
    }
    Triangle& Triangle::Rotate(double angle)
    {
        Point pivot = FindCenter();
        m_p1.Revolve(pivot, angle);
        m_p2.Revolve(pivot, angle);
        m_p3.Revolve(pivot, angle);
        return *this;
    }
    Point Triangle::FindCenter()
    {
        
    }
#endif //SHAPES_TRIANGLE