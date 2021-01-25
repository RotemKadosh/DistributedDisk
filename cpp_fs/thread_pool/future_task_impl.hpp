#ifndef __RD94_FUTURE_TASK_IMPL_HPP__
#define __RD94_FUTURE_TASK_IMPL_HPP__

#include "future_task.hpp"

namespace ilrd
{
    FunctionTask::FunctionTask(boost::function<void ()> func_):
    m_func(func_)
    {}
    
    FunctionTask::~FunctionTask()
    {}

    void FunctionTask::Do()
    {
        m_func();
    }

    template <typename T>
    FutureTask<T>::FutureTask(boost::function<T ()> func_):
    m_promise(),
    m_future_obj(m_promise.get_future()),
    m_func(func_)
    {}

    template <typename T>
    void FutureTask<T>::Do()
    {
        T t = m_func();
        m_promise.set_value(t);
    }

    template <typename T>
    bool FutureTask<T>::IsReady() const
    {
        return m_future_obj.is_ready();
    }
    
    template <typename T>
    T FutureTask<T>::GetValue()
    {
       return m_future_obj.get();
    }
}

#endif // __RD94_FUTURE_TASK_IMPL_HPP__