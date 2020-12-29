#include <iostream>/*cout*/


#include "shape.hpp" 
#include "line.hpp" 
#include "circle.hpp" 
#include "square.hpp" 
#include "rectangle.hpp" 
#include "../utils/utils.hpp"
namespace ilrd
{


Shape::Shape() noexcept: 
m_offset(0) 
{}

Line::Line() noexcept
{}

Circle::Circle() noexcept
{}

Rectangle::Rectangle() noexcept
{}

Square::Square() noexcept
{}

Shape::~Shape() noexcept
{}

Line::~Line() noexcept
{}

Circle::~Circle() noexcept
{}


Rectangle::~Rectangle() noexcept
{}

Square::~Square() noexcept
{}

inline size_t Shape::GetOffset() const
{
    return m_offset;
}

void Shape::PrintOffset(size_t steps)
{
    for(size_t i = 0; i < steps; ++i)
    {
        std::cout << " ";
    }
}


Shape& Shape::Move(size_t steps_) noexcept
{
    m_offset += steps_;
    return *this;
}

void Line::Draw() const noexcept
{
    PrintOffset(GetOffset());
    std::cout << "LINE" << std::endl;
}

void Circle::Draw() const noexcept
{
    PrintOffset(GetOffset());
    std::cout << "CIRCLE" << std::endl;
}

void Rectangle::Draw() const noexcept
{
    PrintOffset(GetOffset());
    std::cout << "RECTANGLE" << std::endl;
}

void Square::Draw() const noexcept
{
    PrintOffset(GetOffset());
    std::cout << "SQUARE" << std::endl;
}



}//namespace ilrd