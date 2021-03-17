#ifndef __RD94_DISPATCHER_CALLBACK_IMPL_HPP__
#define __RD94_DISPATCHER_CALLBACK_IMPL_HPP__

#include <assert.h>  //assert
#include "dispatcher_callback.hpp"

namespace ilrd
{

// ------------CallbackBase-------------------------- 
template<typename EVENT>
CallbackBase<EVENT>::CallbackBase():m_dispatcher(NULL)
{}   

template<typename EVENT>
void CallbackBase<EVENT>::CallBackDeath() 
{
    if(NULL != m_dispatcher)
    {
        m_dispatcher->UnSubscribe(this);
    }
}

template<typename EVENT>
void CallbackBase<EVENT>::SetDispatcher(Dispatcher<EVENT> *dispatcher_)
{
    m_dispatcher = dispatcher_;
}

template<typename EVENT>
CallbackBase<EVENT>::~CallbackBase() noexcept 
{
    CallBackDeath();
}

// ------------Callback------------------------------

template<typename EVENT, typename OBSERVER>
Callback<EVENT,OBSERVER>::Callback(OBSERVER *observer_, 
    notify_func notify_func_, notify_death_func dispatcher_death_func_):
m_observer(observer_),
m_notify_func(notify_func_),
m_dispatcher_death_func(dispatcher_death_func_)
{
    assert(NULL != m_observer);
    assert(NULL != m_notify_func);
    assert(NULL != m_dispatcher_death_func);
}

template<typename EVENT, typename OBSERVER>
void Callback<EVENT,OBSERVER>::Notify(EVENT event_)
{
    (m_observer->*m_notify_func)( event_);
}

template<typename EVENT, typename OBSERVER>
void Callback<EVENT,OBSERVER>::DispatcherDeath()
{
    (m_observer->*m_dispatcher_death_func)();
}

template<typename EVENT, typename OBSERVER>
Callback<EVENT,OBSERVER>::~Callback() noexcept
{
    //empty
}

// ------------Dispatcher------------------------------

template<typename EVENT>
void Dispatcher<EVENT>::Subscribe(CallbackBase<EVENT> *callback_)
{
    assert(NULL != callback_);

    m_subscribers.push_front(callback_);
    callback_->SetDispatcher(this);
}

template<typename EVENT>
void Dispatcher<EVENT>::Broadcast(EVENT event_)
{
    for(iter it = m_subscribers.begin();it != m_subscribers.end(); ++it)
    {
        (*it)->Notify(event_);
    }
}


template<typename EVENT>
void Dispatcher<EVENT>::UnSubscribe(CallbackBase<EVENT> *callback_)
{
    assert(NULL != callback_);
    
    m_subscribers.remove(callback_);
    
}


template<typename EVENT>
Dispatcher<EVENT>::~Dispatcher() noexcept
{
    for(iter it = m_subscribers.begin(); it != m_subscribers.end(); ++it)
    {
        (*it)->DispatcherDeath();
        (*it)->SetDispatcher(NULL);
    }
}

template<typename EVENT>
Dispatcher<EVENT>::Dispatcher()
{
    //empty
}

}// ilrd

#endif // __RD94_DISPATCHER_CALLBACK_IMPL_HPP__