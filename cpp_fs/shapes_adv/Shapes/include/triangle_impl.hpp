#ifndef SHAPES_TRIANGLE_IMPL
#define SHAPES_TRIANGLE_IMPL

#include <cmath>//sin cos
#include "point.hpp"
#include "triangle.hpp"
#include "glut_utils.h" // drawpolygon
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
        Point ret((m_p1.GetX() + m_p2.GetX() + m_p3.GetX())/3,
                     (m_p1.GetY() + m_p2.GetY(), m_p3.GetY())/3);
        return ret;
    }
    void Triangle::Draw(COLORS color) const
    {
        DrawPolygon(color, 3, m_p1.GetX(), m_p1.GetY(), m_p2.GetX(), 
                                m_p2.GetY(),m_p3.GetX(), m_p3.GetY());
    }

}
#endif //SHAPES_TRIANGLE_IMPL