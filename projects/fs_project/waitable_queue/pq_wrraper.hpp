#ifndef __ILRD_RD94__PQWRAPPER_Q_HPP
#define __ILRD_RD94__PQWRAPPER_Q_HPP

#include <queue>     //priotity_queue

#include "../utils/utils.hpp"

namespace ilrd
{
template <typename T,
          typename CONTAINER = std::vector<T>,
          typename COMPARE = std::less<typename CONTAINER::value_type> >

class PQWrapper : private std::priority_queue<T, CONTAINER, COMPARE>
{
public:
    typedef typename CONTAINER::const_reference const_value_ref;
    typedef std::priority_queue<T, CONTAINER, COMPARE> priority_queue;

    // generated ctor, dtor is good enough

    
    inline const_value_ref front() const;

    using std::priority_queue<T, CONTAINER, COMPARE>::empty;
    using std::priority_queue<T, CONTAINER, COMPARE>::pop;
    using std::priority_queue<T, CONTAINER, COMPARE>::push;
};

} // namespace ilrd

#endif //__ILRD_RD94__PQWRAPPER_Q_HPP