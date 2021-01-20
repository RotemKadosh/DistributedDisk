#ifndef SHAPES_TRIANGLE
#define SHAPES_TRIANGLE


#include "point.hpp"
#include "glut_utils.h"//COLOROS

namespace ilrd
{
    class Triangle
    {
    private:
        Point m_p1;
        Point m_p2;
        Point m_p3;
        Point FindCenter();
    public:
        explicit Triangle(double x1 = 100, double y1 = 500,
                            double x2 = 300 , double y2 = 100,
                            double x3 = 500, double y3 = 500 );
        explicit Triangle(Point p1, Point p2, Point p3);
        //~Triangle(); default
        //CCtor & = default;
        Triangle& Move(double xDelta, double yDelta);
        Triangle& Revolve(double angle ,const Point& pivot);
        Triangle& Rotate(double angle);
        
        void Draw(COLORS color) const;
    };
    

    
}

#endif //SHAPES_TRIANGLE



/*
Shapes advanced:
Super advanced (might take a couple days):
Try to play with the program in the attachment.
Start your research from the readme file. 


Based on the library given, implement following shapes:
- Triangle
- Square
- Rectangle
- Circle
- Group (of shapes)

That can do following actions:
- Draw
- Rotate
- Save
- Move
- Revolve
*/