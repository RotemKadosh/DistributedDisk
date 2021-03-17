#ifndef __RD94_SINGLETON_IMPL_HPP__
#define __RD94_SINGLETON_IMPL_HPP__

#include <boost/thread/lock_guard.hpp>//lock_guard

#include "singleton.hpp"

namespace ilrd
{
template<typename T>
T* Singleton<T>::GetInstance()
{
    static boost::mutex m_mutex; 
    
    if(NULL == m_instance.load(boost::memory_order_acquire))
    {
        boost::lock_guard<boost::mutex> lock(m_mutex);
        
        if(NULL == m_instance.load(boost::memory_order_relaxed)) 
        {
            T *tmp = new T();
            m_instance.store(tmp, boost::memory_order_release);
            atexit(ExitFunc);
        }
    }
    return m_instance;
}

template<typename T>
void Singleton<T>::ExitFunc()
{
    delete Singleton<T>::m_instance;
}


}// ilrd

#endif // __RD94_SINGLETON_IMPL_HPP__