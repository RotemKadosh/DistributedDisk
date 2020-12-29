#include "shape.hpp"
#include "line.hpp"
#include "circle.hpp"
#include "square.hpp"
#include "rectangle.hpp"
#include <iostream> // cout
#include <list> // std::list, begin, end
#include <iterator> //std :: iter

using namespace ilrd;


void PrintList(std::list <Shape *> shapes);

int main()
{
    Line l1;
    Circle c1;
    Square s1;
    Rectangle r1;
    
    l1.Draw();
    c1.Draw();
    s1.Draw();
    r1.Draw();
    l1.Move().Draw();
    c1.Move(2).Draw();
    s1.Move(3).Draw();
    r1.Move(4).Draw();
    l1.Draw();
    c1.Draw();
    s1.Draw();
    r1.Draw();

    std::list <Shape *> shapes_list;

    shapes_list.push_back(&l1);
    shapes_list.push_back(&c1);
    shapes_list.push_back(&s1);
    shapes_list.push_back(&r1);
    shapes_list.push_back(&r1);
    PrintList(shapes_list);
    return 0;
}

void PrintList(std::list <Shape *> shapes)
{
    std::list <Shape *> :: iterator iter; 
    for(iter = shapes.begin(); iter != shapes.end(); ++iter)
    {
        (*iter)->Draw();
    }
        
}