#ifndef __ILRD_94_BIT_ARRAY_FUNCTORS_HPP__ 
#define __ILRD_94_BIT_ARRAY_FUNCTORS_HPP__ 

#include "bit_array.hpp"


namespace ilrd
{

static const unsigned char g_byte_count_lut[256] =  {
            0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
            4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
                };

template<size_t N>
class And 
{  
public: 
    And(const BitArray<N>& other_ ): m_other_ref(other_), m_index(0)  { } 
  
    int operator () (size_t arr_part)  
    { 
        size_t ans = arr_part & m_other_ref.m_bit_arr[m_index++];
        return ans; 
    } 
private: 
    const BitArray<N>& m_other_ref;
    size_t m_index;
};

template<size_t N>
class Or 
{ 
public: 
    Or(const BitArray<N>& other_ ): m_other_ref(other_), m_index(0)  { } 
  
    int operator () (size_t arr_part)  
    { 
        return arr_part | m_other_ref[m_index++]; 
    } 
private: 
    const BitArray<N>& m_other_ref;
    size_t m_index;
};

template<size_t N>
class Xor 
{ 
public: 
    Xor(const BitArray<N>& other_ ): m_other_ref(other_), m_index(0)  { } 
  
    int operator () (size_t arr_part)  
    { 
        return arr_part ^ m_other_ref[m_index++]; 
    } 
private: 
    const BitArray<N>& m_other_ref;
    size_t m_index;
};


class FlipWord 
{ 
public: 
    size_t operator () (size_t arr_part)  
    { 
        
        return ~arr_part ; 
    } 
};





class CountByte
{
public:
    CountByte()
    {}
    unsigned char operator()(const unsigned char& sum_,unsigned char& byte_)
    {
        return g_byte_count_lut[byte_] + sum_;
    }
};


        
}//ilrd
#endif // __ILRD_94_BIT_ARRAY_FUNCTORS_HPP__ 