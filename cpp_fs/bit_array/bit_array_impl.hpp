#ifndef __ILRD_94_BIT_ARRAY_IMPL_HPP__ 
#define __ILRD_94_BIT_ARRAY_IMPL_HPP__


#include <stdexcept> // std::out_of_range
#include <algorithm> //for_each
#include <numeric> //accumulate
#include "bit_array.hpp"
#include "bit_array_functors.hpp"

namespace ilrd
{
//--------------------proxy---------------------------
template<size_t N>
BitArray<N>::BitProxy::BitProxy(BitArray<N>& bit_arr_, size_t pos_):
m_bit_arr_ref(bit_arr_),
m_pos(pos_)
{}

template<size_t N>
typename BitArray<N>::BitProxy& BitArray<N>::BitProxy::operator=(bool val) noexcept
{
    m_bit_arr_ref.Set(m_pos, val);
    return *this;
}

template<size_t N>
BitArray<N>::BitProxy::operator bool() const noexcept
{
    return const_cast<const BitArray<N>&>(m_bit_arr_ref)[m_pos];
}

template<size_t N>
bool BitArray<N>::BitProxy::operator !() const noexcept
{
   return !(static_cast<bool>(*this));
}

template<size_t N>
typename BitArray<N>::BitProxy& BitArray<N>::BitProxy::Flip() noexcept
{
    m_bit_arr_ref.Flip(m_pos);
}

template<size_t N>
typename BitArray<N>::BitProxy& BitArray<N>::BitProxy::operator=(const BitProxy& other_)
{
    bool val = other_;
    *this = val;
    return *this;
}
//-----------------------bit array------------------------
template<size_t N>
BitArray<N>::BitArray():
m_bit_arr(std::vector<size_t>(((N - 1) / BITS_IN_WORD) + 1, 0))
{}

template<size_t N>
void BitArray<N>::CheckBoundaries(const size_t pos_)
{
    if(N <= pos_)
    {
        throw std::out_of_range("");
    }
}

template<size_t N>
size_t BitArray<N>::GetPosIdx(const size_t pos_)
{
    return pos_ / BITS_IN_WORD;
}

template<size_t N>
bool BitArray<N>::operator[](size_t pos_) const 
{
    CheckBoundaries(pos_);
    size_t part = m_bit_arr[GetPosIdx(pos_)];
    return (part & (1LU << (pos_ % BITS_IN_WORD)));
}

template<size_t N>
typename BitArray<N>::BitProxy BitArray<N>::operator[](size_t pos_)
{
    CheckBoundaries(pos_);
    return BitProxy(*this, pos_);
}

template<size_t N>
typename BitArray<N>::BitProxy BitArray<N>::Get(size_t pos_)
{
    CheckBoundaries(pos_);
    return BitProxy(*this, pos_);
} 

template<size_t N>
BitArray<N>& BitArray<N>::operator&=(const BitArray<N>& other_) noexcept
{
    std::transform(m_bit_arr.begin(), m_bit_arr.end(), other_.m_bit_arr.begin(),
                   m_bit_arr.begin(), std::bit_and<size_t>());
    return *this;
}

template<size_t N>
BitArray<N>& BitArray<N>::operator|=(const BitArray<N>& other_) noexcept
{
    std::transform(m_bit_arr.begin(), m_bit_arr.end(), other_.m_bit_arr.begin(),
                   m_bit_arr.begin(), std::bit_or<size_t>());
    return *this;
}

template<size_t N>
BitArray<N>& BitArray<N>::operator^=(const BitArray<N>& other_) noexcept
{
    std::transform(m_bit_arr.begin(), m_bit_arr.end(), other_.m_bit_arr.begin(),
                   m_bit_arr.begin(), std::bit_xor<size_t>());
    return *this;
}

template<size_t N>
bool BitArray<N>::operator==(const BitArray<N>& rhs_) const noexcept
{
    return (m_bit_arr == rhs_.m_bit_arr);
}

template<size_t N>
bool BitArray<N>::operator!=(const BitArray<N>& rhs_) const noexcept
{
     return !(m_bit_arr == rhs_.m_bit_arr);
}

template<size_t N>
BitArray<N>& BitArray<N>::Set() noexcept
{
    size_t mask =  0xffffffffffffffff ;
    std::fill(m_bit_arr.begin(), m_bit_arr.end(), mask);
    
    size_t shift = (BITS_IN_WORD - (N % BITS_IN_WORD)) % BITS_IN_WORD;
    
    m_bit_arr.back() >>= shift;
    return *this;
}

template<size_t N>
BitArray<N>& BitArray<N>::Set(size_t pos_, bool value_)
{
    CheckBoundaries(pos_);
    size_t size_t_value = value_;
    size_t bit_array_part =  m_bit_arr[GetPosIdx(pos_)];
    size_t mask = 0xffffffffffffffff ^ (1lu << (pos_ % BITS_IN_WORD));

    bit_array_part &= mask;
    size_t_value =  (size_t_value << (pos_ % BITS_IN_WORD));

    m_bit_arr[GetPosIdx(pos_)] = (bit_array_part | size_t_value);

    return *this;
}

template<size_t N>
BitArray<N>& BitArray<N>::Flip() noexcept
{
    std::transform(m_bit_arr.begin(), m_bit_arr.end(),m_bit_arr.begin(), FlipWord());
    size_t mask =  0xffffffffffffffff >> (BITS_IN_WORD - (N % BITS_IN_WORD)) % BITS_IN_WORD;
    size_t part = m_bit_arr[((N) / BITS_IN_WORD)];
    m_bit_arr[((N) / BITS_IN_WORD)] = part & mask;
    
    return *this;
}

template<size_t N>
BitArray<N>& BitArray<N>::Flip(size_t pos_) 
{
    CheckBoundaries(pos_);
    size_t bit_array_part =  m_bit_arr[GetPosIdx(pos_)];
    bit_array_part ^= (1LU << (pos_ % BITS_IN_WORD));
    m_bit_arr[GetPosIdx(pos_)] = bit_array_part;
    return *this;
}


static size_t CountSetBits(size_t sum, size_t word)
{
    unsigned char *byte = reinterpret_cast<unsigned char *>(&word);
    return std::accumulate(byte, byte + 8, 0UL, CountByte()) + sum;
}

template<size_t N>
size_t BitArray<N>::Count() const noexcept
{
    return accumulate(m_bit_arr.begin(), m_bit_arr.end(), 0UL, CountSetBits);
}

template<size_t N>
std::string BitArray<N>::ToString() const
{
    std::string str(N , '0');
    long i = 0;
    long j = 0;
    for(j = 0 ,i = N-1 ; i >= 0 ; --i, j++)
    {
        if((*this)[j])
        {
            str[i] = '1';
        }
    }
    //str[i] = '\0';
    return str;
}


}
#endif // __ILRD_94_BIT_ARRAY_IMPL_HPP__