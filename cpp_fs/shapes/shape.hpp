#ifndef ILRD_94_SHAPE_HPP
#define ILRD_94_SHAPE_HPP
#include <iosfwd> //size_t
#include "../utils/utils.hpp"


namespace ilrd
{

class Shape
{  
public:
    explicit Shape() noexcept;
    virtual ~Shape() noexcept;
    //  cctor, op= genrated are good enough
   virtual void Draw() const = 0;
   Shape& Move(size_t steps_ = 1) noexcept;

protected:
   inline size_t GetOffset() const;
   static void PrintOffset(size_t steps);
   
private:
   size_t m_offset;
};

}//namespace ilrd


#endif  // ILRD_94_SHAPE_HPP