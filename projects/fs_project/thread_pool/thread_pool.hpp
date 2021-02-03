#ifndef __RD94_THREAD_POOL_HPP__
#define __RD94_THREAD_POOL_HPP__

#include <boost/thread/mutex.hpp> // mutex 
#include <boost/unordered_map.hpp> // unordered map
#include <boost/thread/condition.hpp> // condition_variable
#include <boost/thread/thread.hpp>//boost::thread::hardware_concurrency
#include <boost/shared_ptr.hpp>//boost::shared_ptr
#include "../waitable_queue/waitable_q_impl.hpp" //WaitableQueue
#include "../waitable_queue/pq_wrapper_impl.hpp" //PQWrapper
#include <boost/atomic.hpp>
#include "utils.hpp"

namespace ilrd
{
 
class ThreadPool : private boost::noncopyable
{
public:
    class Task  : private boost::noncopyable
    {
    public:
        //generated OK
        virtual ~Task()
        {}
        virtual void Do() = 0;
    };
    enum Priority
    {
        LOW = 1,
        NORMAL = 2,
        HIGH = 3,
        CRITICAL = 4 //user dont allow to use it
    };

    explicit ThreadPool(int policy_, size_t thread_count_ = boost::thread::hardware_concurrency(), int niceness_ = 0); //throws bad_alloc
    ~ThreadPool() noexcept;
    void Add(boost::shared_ptr<Task> new_task_, Priority priority_ = NORMAL);//throws bad_alloc
    void Run();
    void Pause();
    void Stop(); //destroys thread pool gracefully- task should have interruption piont in order to be stoped.
    void Resize(size_t new_thread_count_); // throws bad_alloc

private:

    typedef boost::unordered_map<boost::thread::id, boost::shared_ptr<boost::thread> > ThreadMap_t;
    typedef WaitableQueue<std::pair<Priority, boost::shared_ptr<Task> >, PQWrapper<std::pair<Priority, boost::shared_ptr<Task> > > > TaskQueue_t;
    typedef boost::unordered::unordered_map<boost::thread::id, boost::shared_ptr<boost::thread> >::iterator iter;
    typedef std::pair<Priority, boost::shared_ptr<Task> > Pair;
    enum State
    {
        PAUSE = 1,
        RUN = 2,
        STOP = 3
    };

    friend class SleepingPeel;

    void RunThread();
    void AngleOfDeathThread();
    void InsertPoisonApples(size_t poison_apple_to_insert);
    void AddThreads(size_t thread_count_);
    void InsertSleepingPeels(size_t count_of_pills);

    TaskQueue_t m_tasks;
    ThreadMap_t m_threads;
    WaitableQueue<boost::thread::id> m_ready_to_die;
    boost::thread m_angle_of_death;
    int m_niceness;
    int m_policy;
    boost::atomic<State> m_flag;
    boost::mutex m_mutex;
    boost::condition_variable m_paused;
    
    
};




} // ilrd

#endif // __RD94_THREAD_POOL_HPP__s