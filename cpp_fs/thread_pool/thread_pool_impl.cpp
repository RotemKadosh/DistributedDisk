#include <utility>//std::pair
#include <boost/thread/mutex.hpp> // boost::unique_lock 
#include <pthread.h>//set_schedparam
#include <functional>//bind
#include <stdio.h>//perror
#include <sys/resource.h>//set priority
#include "thread_pool.hpp"
namespace ilrd
{

/*----------------------special Tasks ----------------------*/

/*-------------poison apple Task ----------------------*/
class PoisonApple : public ThreadPool::Task
{
public:
    explicit PoisonApple(WaitableQueue<boost::thread::id>& ready_to_die_);
    ~PoisonApple()
    {}

    virtual void Do();
  
private:
    WaitableQueue<boost::thread::id>& m_ready_to_die;
};

PoisonApple::PoisonApple(WaitableQueue<boost::thread::id>& ready_to_die_): 
m_ready_to_die(ready_to_die_)
{}

void PoisonApple::Do()
{
    m_ready_to_die.Push(boost::this_thread::get_id());
    while(true)
    {
        boost::this_thread::sleep_for(boost::chrono::seconds(60)); 
    } 
} 

/*------------ SleepingPeel Task ----------------------*/
class SleepingPeel : public ThreadPool::Task
{
public:
    SleepingPeel(ThreadPool& tp_);
    ~SleepingPeel();
    virtual void Do();
private:
    ThreadPool& m_my_tp;
};

SleepingPeel::SleepingPeel(ThreadPool& tp_):
m_my_tp(tp_)
{}

void SleepingPeel::Do()
{       
    while(ThreadPool::PAUSE == m_my_tp.m_flag)
    {
        boost::unique_lock<boost::mutex> lock(m_my_tp.m_mutex);
        m_my_tp.m_paused.wait(lock);
    }
}

SleepingPeel::~SleepingPeel()
{}

//-------------------------public thread pool--------------------
ThreadPool::ThreadPool(int policy_, size_t thread_count_, int niceness_)://throws bad_alloc
m_angle_of_death(boost::thread(boost::bind(&ThreadPool::AngleOfDeathThread, this))),
m_niceness(niceness_),
m_policy(policy_),
m_flag(PAUSE)
{
    static const int FAIL = -1;
    if(FAIL == setpriority(PRIO_PROCESS, getpid(), niceness_))
    {
        perror("setpriority:");
    }

    InsertSleepingPeels(thread_count_);
    AddThreads(thread_count_);
}

ThreadPool::~ThreadPool() noexcept
{
    if(STOP != m_flag)
    {
        Stop();
    }
}

void ThreadPool::Add(boost::shared_ptr<Task> new_task_, Priority priority_)//throws bad_alloc
{
    Pair pair(priority_, new_task_);
    m_tasks.Push(pair);
}

void ThreadPool::Run()
{   
    m_flag = RUN;
    m_paused.notify_all();
}

void ThreadPool::Pause()
{
    m_flag = PAUSE;
    InsertSleepingPeels(m_threads.size());
}

void ThreadPool::Stop() //destroys thread pool gracefully
{

    m_flag = STOP;
    m_angle_of_death.interrupt();
    m_angle_of_death.join();
    
    for(iter it = m_threads.begin(); it != m_threads.end(); ++it)
    {
        it->second->interrupt();
    }

    for(iter it = m_threads.begin(); it != m_threads.end(); ++it)
    {
        it->second->join();
    }  
}

//-------------------------private thread pool--------------------
void ThreadPool::AngleOfDeathThread()
{
    while(true)
    {
        boost::this_thread::interruption_point();
        boost::thread::id id_to_kill;
        m_ready_to_die.Pop(&id_to_kill);
        iter it = m_threads.find(id_to_kill);
        it->second->interrupt();
        it->second->join();
        m_threads.erase(it);
    }
}

void ThreadPool::RunThread()
{
    while(true)
    {
        Pair pair;
        m_tasks.Pop(&pair);
        boost::this_thread::interruption_point();
        pair.second->Do();  
    }
}

void ThreadPool::Resize(size_t new_thread_count_)// throws bad_alloc
{
    size_t len = m_threads.size();
    if(new_thread_count_ < len)
    {
        InsertPoisonApples(len - new_thread_count_);  
    }
    else
    {
        AddThreads(new_thread_count_ - len);
    }  
} 

void ThreadPool::InsertPoisonApples(size_t poison_apple_to_insert)
{   
    while(0 < poison_apple_to_insert--)
    {
        Add(boost::shared_ptr<Task>(new PoisonApple(m_ready_to_die)), CRITICAL);
    }
}

static void SetAttr(boost::thread::attributes *attrs, int policy_)
{   
    static const int FAIL = -1;
    if(FAIL == pthread_attr_setschedpolicy(attrs->native_handle(), policy_))
    {
        perror("pthread_attr_setschedpolicy:");    
    }
    if(FAIL == pthread_attr_setinheritsched(attrs->native_handle(),PTHREAD_INHERIT_SCHED))
    {
        perror("pthread_attr_setinheritsched:");
    }
}

void ThreadPool::AddThreads(size_t thread_count_)
{       
    boost::thread::attributes attrs;
    SetAttr(&attrs, m_policy);
    for(size_t i = 0; i < thread_count_; ++i)
    {   
        boost::shared_ptr<boost::thread> new_thread(new boost::thread(attrs, boost::bind(&ThreadPool::RunThread, this))); 
        m_threads[new_thread->get_id()] = new_thread;
    }
}




void ThreadPool::InsertSleepingPeels(size_t count_of_pills)
{
    while(0 < count_of_pills--)
    {
        Add(boost::shared_ptr<Task>(new SleepingPeel(*this)), CRITICAL);
    }
}


} // ilrd





