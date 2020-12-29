#ifndef ILRD_94_LINE_HPP
#define ILRD_94_LINE_HPP

#include "shape.hpp"
#include "../utils/utils.hpp"

namespace ilrd
{

class Line: public Shape
{
public:
    explicit Line() noexcept;
    ~Line() noexcept;
    virtual void Draw() const noexcept;


};
}//namespace ilrd
#endif //ILRD_94_LINE_HPP