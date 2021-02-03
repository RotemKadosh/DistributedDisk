#ifndef __ILRD_RD94__PQWRAPPER_IMPL_HPP
#define __ILRD_RD94__PQWRAPPER_IMPL_HPP

#include "pq_wrraper.hpp"

namespace ilrd
{
template <typename T,typename CONTAINER,typename COMPARE>
typename PQWrapper<T, CONTAINER, COMPARE >::const_value_ref PQWrapper<T, CONTAINER, COMPARE >::front() const
{
   return this->top();
} 
}
#endif //__ILRD_RD94__PQWRAPPER_IMPL_HPP