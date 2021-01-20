#ifndef __ILRD_94_BIT_ARRAY_IMPL_HPP__ 
#define __ILRD_94_BIT_ARRAY_IMPL_HPP__

#include <string> //std::string
#include <stdexcept> // std::out_of_range
#include <algorithm> //fill, transform
#include <numeric>//accumulate
#include <functional>//bit_and, bit_or, bit_xor
#include "bit_array.hpp"

namespace ilrd
{

template<size_t N>
void BitArray<N>::CheckBoundaries(const size_t pos_)
{
    if(N <= pos_)
    {
        throw std::out_of_range("");
    }
}

template<size_t N>
size_t BitArray<N>::GetVectorIdx(const size_t pos_)
{
    return pos_ / BITS_IN_SIZE_T;
}

template<size_t N>
BitArray<N>::BitProxy::BitProxy(BitArray<N>& bit_arr_, size_t pos_):
m_bitsarray_ref(bit_arr_),
m_pos(pos_)
{
    //EMPTY
}

template<size_t N>
typename BitArray<N>::BitProxy& BitArray<N>::BitProxy::operator=(bool other_) noexcept
{
    m_bitsarray_ref.Set(m_pos, other_);
    return *this;
}

template<size_t N>
typename BitArray<N>::BitProxy& BitArray<N>::BitProxy::operator=(const BitProxy& other_) noexcept
{
    bool val = static_cast<bool>(other_);
    *this = val;
    return *this; 
}

template<size_t N>
BitArray<N>::BitProxy::operator bool() const noexcept
{
    return const_cast<const BitArray<N>&>(m_bitsarray_ref)[m_pos];
}

template<size_t N>
bool BitArray<N>::BitProxy::operator!() const noexcept
{
    return !(static_cast<bool>(*this));
}

template<size_t N>
typename BitArray<N>::BitProxy& BitArray<N>::BitProxy::Flip() noexcept
{
    m_bitsarray_ref.Flip(m_pos);
    return *this;
}

template<size_t N>
BitArray<N>::BitArray():
m_bitarray(std::vector<size_t>(((N - 1) / BITS_IN_SIZE_T) + 1, 0))
{
    //EMPTY   
}

template<size_t N>
bool BitArray<N>::operator[](size_t pos_) const //throws std::out_of_range
{
    CheckBoundaries(pos_);
    size_t bit_array_part =  m_bitarray[GetVectorIdx(pos_)];
    return (bit_array_part & (1lu << (pos_ % BITS_IN_SIZE_T)));
}

template<size_t N>
typename BitArray<N>::BitProxy BitArray<N>::operator[](size_t pos_)//throws std::out_of_range
{
    CheckBoundaries(pos_);
    return BitProxy(*this, pos_);
}

template<size_t N>
typename BitArray<N>::BitProxy BitArray<N>::Get(size_t pos_)//throws std::out_of_range
{
    CheckBoundaries(pos_);
    return BitProxy(*this, pos_);
} //throws std::out_of_range

 
template<size_t N>
BitArray<N>& BitArray<N>::operator&=(const BitArray<N>& other_) noexcept
{
    
   std::transform(m_bitarray.begin(), m_bitarray.end(), other_.m_bitarray.begin(),
                                     m_bitarray.begin(), std::bit_and<size_t>());
    return *this;
}

template<size_t N>
BitArray<N>& BitArray<N>::operator|=(const BitArray<N>& other_) noexcept
{
   std::transform(m_bitarray.begin(), m_bitarray.end(), other_.m_bitarray.begin(),
                                        m_bitarray.begin(), std::bit_or<size_t>());
    return *this;
}

template<size_t N>
BitArray<N>& BitArray<N>::operator^=(const BitArray<N>& other_) noexcept
{
    std::transform(m_bitarray.begin(), m_bitarray.end(), other_.m_bitarray.begin(),
                                        m_bitarray.begin(), std::bit_xor<size_t>());
    return *this;
}

template<size_t N>
bool BitArray<N>::operator==(const BitArray<N>& rhs_) const noexcept
{
    return (m_bitarray == rhs_.m_bitarray);
}

template<size_t N>
bool BitArray<N>::operator!=(const BitArray<N>& rhs_) const noexcept
{
     return m_bitarray != rhs_.m_bitarray;
}

template<size_t N>
BitArray<N>& BitArray<N>::Set() noexcept
{
    size_t all_on = ALL_ON;
    std::fill(m_bitarray.begin(), m_bitarray.end(), all_on);

    size_t shift = (BITS_IN_SIZE_T - (N % BITS_IN_SIZE_T)) % BITS_IN_SIZE_T;

    m_bitarray.back() >>= shift;

    return *this;
}

template<size_t N>
BitArray<N>& BitArray<N>::Set(size_t pos_, bool value_)
{
    CheckBoundaries(pos_);
    size_t size_t_value = static_cast<bool>(value_);

    size_t mask = ALL_ON ^ (1lu << (pos_ % BITS_IN_SIZE_T));
    m_bitarray[GetVectorIdx(pos_)] &= mask;

    size_t val =  (size_t_value << (pos_ % BITS_IN_SIZE_T));
    m_bitarray[GetVectorIdx(pos_)] |= val;
    return *this;
}

class CountByte
{
public:
    CountByte(unsigned char *table_);
    unsigned char operator()(const unsigned char& sum_, unsigned char &arr_num);

private:
    unsigned char *m_table;
};

CountByte::CountByte(unsigned char *table_) : m_table(table_)
{
    //EMPTY
}

unsigned char CountByte::operator()(const unsigned char& sum_, unsigned char &arr_num)
{
    return m_table[arr_num] + sum_;
}

static size_t CountSetBits(size_t sum, size_t arr_num)
{
    static unsigned char table[256] = 
    {
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
    unsigned char *byte = reinterpret_cast<unsigned char *>(&arr_num);
    return std::accumulate(byte, byte + 8, 0UL, CountByte(table)) + sum;
}

template<size_t N>
size_t BitArray<N>::Count() const noexcept
{
    return std::accumulate(m_bitarray.begin(), m_bitarray.end(), 0UL, CountSetBits);
}

class FlipWord 
{ 
public: 
    size_t operator()(size_t arr_num); 

};

size_t FlipWord::operator()(size_t arr_num)  
{ 
    return ~arr_num;  
} 

template<size_t N>
BitArray<N>& BitArray<N>::Flip() noexcept
{
    std::transform(m_bitarray.begin(), m_bitarray.end(),m_bitarray.begin(), FlipWord());

    size_t mask =  ALL_ON >> (BITS_IN_SIZE_T - (N % BITS_IN_SIZE_T)) % BITS_IN_SIZE_T;
    m_bitarray.back() &= mask;

    return *this;
}

template<size_t N>
BitArray<N>& BitArray<N>::Flip(size_t pos_) //throws std::out_of_range
{
    CheckBoundaries(pos_);
    m_bitarray[GetVectorIdx(pos_)] ^= (1lu << (pos_ % BITS_IN_SIZE_T));

    return *this;
}

template<size_t N>
std::string BitArray<N>::ToString() const
{
    std::string str(N, '0');
    for(ssize_t i = N - 1, j = 0; i >= 0; --i, ++j)
    {
        str[i] = (1 & (*this)[j]) + '0'; 
    }
    return str;
}





} // ilrd
#endif // __ILRD_94_BIT_ARRAY_IMPL_HPP__