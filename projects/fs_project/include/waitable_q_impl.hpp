#ifndef __ILRD_RD94__WAITABLE_Q_IMPL_HPP__
#define __ILRD_RD94__WAITABLE_Q_IMPL_HPP__

#include <assert.h>//asert
#include "waitable_q.hpp"

namespace ilrd
{
template <typename T, typename CONTAINER>
void WaitableQueue<T,CONTAINER>::Push(const T& data_)
{
    boost::unique_lock<boost::mutex> guard((this->m_mutex));
    m_queue.push(data_);
    m_is_not_empty.notify_one();
}

template <typename T, typename CONTAINER>
void WaitableQueue<T,CONTAINER>::Pop(T *data_)
{
    assert(data_ != NULL);
    boost::unique_lock<boost::mutex> guard((this->m_mutex));
    while(m_queue.empty())
    {
        m_is_not_empty.wait(guard);
    }
    *data_ = m_queue.front();
    m_queue.pop();
}

template <typename T, typename CONTAINER>
bool WaitableQueue<T,CONTAINER>::Pop(T *data, boost::chrono::seconds timeout_)
{
    assert(data != NULL);
    boost::chrono::system_clock::time_point wakeup = boost::chrono::system_clock::now() + timeout_;
    boost::unique_lock<boost::mutex> guard((this->m_mutex));
    while(m_queue.empty())
    {
        if(boost::cv_status::timeout == m_is_not_empty.wait_until(guard, wakeup))
        {
            return false;
        }
    }
    *data = m_queue.front();
    m_queue.pop();
    return true;
}

template <typename T, typename CONTAINER>
bool WaitableQueue<T,CONTAINER>::IsEmpty() const noexcept
{
    boost::unique_lock<boost::mutex> guard((this->m_mutex));
    return m_queue.empty();
}


}


#endif //__ILRD_RD94__WAITABLE_Q_IMPL_HPP__