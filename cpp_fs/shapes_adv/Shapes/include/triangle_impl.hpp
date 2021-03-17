#ifndef SHAPES_TRIANGLE_IMPL
#define SHAPES_TRIANGLE_IMPL

#include <cmath>//sin cos
#include "point.hpp"
#include "triangle.hpp"
#include "glut_utils.h" // drawpolygon
namespace ilrd
{
    
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
        double a = GetDistance(m_p1,m_p2);
        double b = GetDistance(m_p2,m_p3);
        double c = GetDistance(m_p3,m_p1);
        double x1 = m_p1.GetX();
        double y1 = m_p1.GetY();
        double x2 = m_p2.GetX();
        double y2 = m_p2.GetY();
        double x3 = m_p3.GetX();
        double y3 = m_p3.GetY();
        double sum = a + b + c;
        Point Center((a*x1 + b*x2 +c*x3)/sum, (a*y1 + b*y2 +c*y3)/sum);
        return Center;
    }
    void Triangle::Draw(COLORS color) const
    {
        DrawPolygon(color, 3,(int) m_p1.GetX(), (int)m_p1.GetY(), (int)m_p2.GetX(), 
                                (int)m_p2.GetY(),(int)m_p3.GetX(), (int)m_p3.GetY());
    }

}
#endif //SHAPES_TRIANGLE_IMPL