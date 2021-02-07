#ifndef WFSQ.HPP
#define WFSQ.HPP

#include <iosfwd> //size_t
#include <boost/thread/mutex.hpp> //m_mutext
#include <boost/thread/condition_variable.hpp> // condition var
#include <boost/chrono.hpp> // chrono secondes
#include <queue> // std::queue

#include <boost/scoped_lock.hpp> //lock_guard

template<typename T>
class Wfsq
{
public:
    explicit Wfsq(size_t capacity = 100);
    //~Wfsq() = default;
    void Pop(T* out_param);
    bool Pop(T* out_param, boost::chrono::seconds timeout); // true if poped and false if time out
    void Push(const T& data); // may throw bed_alloc
    bool Push(const T& data, boost::chrono::seconds timeout);// may throw bed_alloc
    bool IsEmpty() const;
    size_t Size() const;
    size_t Capacity() const;

private:
    const size_t m_capacity;
    size_t m_size;

    std::queue<T> m_queue;
    mutable boost::mutex m_mutex;
    boost::condition_variable m_not_empty;
    boost::condition_variable m_not_full;
};



template <typename T>
Wfsq<T>::Wfsq(size_t capacity):
m_capacity(capacity),
m_size(0)
{}


template <typename T>
void Wfsq<T>::Pop(T* out_param)
{   

    // aquire lock
    // wait on m_not empty
    //front to output
    // Pop
    boost::mutex::scoped_lock<boost::mutex> lock(m_mutex);
    while (m_queue.empty())
    {
        m_not_empty.wait(lock);
    }
    *out_param = m_queue.front();
    m_queue.pop();
    m_not_full.notify_one();

}

template <typename T>
bool Wfsq<T>::Pop(T* out_param, boost::chrono::seconds timeout)
{
    //aquire lock
    //calc timnout from now
    //wait m_not empty until timeout
    //if wake by time out
        //return false
    //else
        //front to output
        //pop
        //return true


    boost::chrono::system_clock time_to_wake (boost::chrono::system_clock::now() + timeout);
    boost::mutex::scoped_lock<boost::mutex> lock(m_mutex);
    if (cv_status::timeout == m_not_empty.wait_until(lock, time_to_wake))
    {
        return false;
    }
    *out_param = m_queue.front();
    m_queue.pop();
    m_not_full.notify_one();
    
}


template <typename T>
void Wfsq<T>::Push(const T& data)
{   
    {
        boost::mutex::scoped_lock<boost::mutex> lock(m_mutex);
        while (m_queue.size() == m_capacity)
        {
            m_not_full.wait(lock);
        }
        m_queue.push(data);
    }
    m_not_empty.notify_one();
}

template <typename T>
bool Wfsq<T>::Push(const T& data, boost::chrono::seconds timeout)
{
    //aquire lock
    //calc timnout from now
    //wait on m_not_full until timeout
    //if wake by time out
        //return false
    //else
        //front to output
        //push

    boost::chrono::system_clock time_to_wake (boost::chrono::system_clock::now() + timeout);
    {
        boost::mutex::scoped_lock<boost::mutex> lock(m_mutex);
        if (cv_status::timeout == m_not_empty.wait_until(lock, time_to_wake))
        {
            return false;
        }
        m_queue.push(data);
    }
    m_not_empty.notify_one();

}

template <typename T>
bool Wfsq<T>::IsEmpty()
{
    boost::mutex::scoped_lock<boost::mutex> lock(m_mutex);
    return m_queue.empty()
}
template <typename T>
size_t Wfsq<T>::Size()
{
    boost::mutex::scoped_lock<boost::mutex> lock(m_mutex);
    return m_queue.size()
}
template <typename T>
size_t Wfsq<T>::Capacity() 
{
    return m_capacity;
}

#endif // WFSQ.HPP