#ifndef __RD94_SINGLETON_HPP__
#define __RD94_SINGLETON_HPP__

#include <boost/atomic.hpp>   //boost::atomic
#include <boost/thread/mutex.hpp> //mutex


namespace ilrd
{


template<typename T>
class Singleton: private boost::noncopyable
{
public:
    
    static T* GetInstance();

private:
    Singleton();
    static void ExitFunc();
    static boost::atomic<T *> m_instance;
    

};

template<typename T>
boost::atomic<T*> Singleton<T>::m_instance;


}// ilrd

#endif // __RD94_SINGLETON_HPP__