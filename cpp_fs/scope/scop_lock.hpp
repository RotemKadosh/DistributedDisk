#ifndef ILRD_94_SCOPE_LOCK_HPP
#define ILRD_94_SCOPE_LOCK_HPP

#if cplusplus < 201104
#define noexcept throw()
#endif //#if cplusplus<201104

#include <boost/core/noncopyable.hpp>   //noncopyable

namespace ilrd
{

template <typename T>
class ScopeLock : private boost::noncopyable
{
public:
    explicit ScopeLock(T &lock_);  //may throw
    ~ScopeLock() noexcept;                  

private:
    T& m_lock;
};

}//irld




#endif // ILRD_94_SCOPE_LOCK_HPP


//#define UNUSED(arg) {if(&arg) /* null */}; if someone wants
