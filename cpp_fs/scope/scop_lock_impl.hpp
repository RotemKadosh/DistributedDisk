#ifndef ILRD_94_SCOPE_LOCK_IMPL_HPP
#define ILRD_94_SCOPE_LOCK_IMPL_HPP
#include "scop_lock.hpp"
#include <boost/thread/mutex.hpp> //mutex lock, unlock

#include <iostream> //std::cerr
#include <pthread.h> //pthread_mutex_t , pthread_mutex_lock pthread_mutex_unlock

namespace ilrd
{
    
template<>
class ScopeLock<boost::mutex>
{
  public:
    explicit ScopeLock(boost::mutex& lock_);  //may throw
    ~ScopeLock() noexcept;                  

    private:
        boost::mutex& m_lock;  
};

ScopeLock<boost::mutex>::ScopeLock(boost::mutex& lock_):
m_lock(lock_)
{
    m_lock.lock();
}
ScopeLock<boost::mutex>::~ScopeLock() noexcept
{
    try
    {
        m_lock.unlock();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    } 
}

template<>
class ScopeLock<pthread_mutex_t>
{
  public:
    explicit ScopeLock(pthread_mutex_t& lock_);  //may throw
    ~ScopeLock() noexcept;                  

    private:
        pthread_mutex_t& m_lock;
        const static int SUCCESS = 0;  
};

ScopeLock<pthread_mutex_t>::ScopeLock(pthread_mutex_t& lock_):
m_lock(lock_)
{
    if(SUCCESS != pthread_mutex_lock(&m_lock))
    {
        throw std::exception();
    }
}

ScopeLock<pthread_mutex_t>::~ScopeLock() noexcept
{
    if(SUCCESS != pthread_mutex_unlock(&m_lock))
    {
       std::cerr << "unlock failed\n";
    }
}

}

#endif // ILRD_94_SCOPE_LOCK_IMPL_HPP