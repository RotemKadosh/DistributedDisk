
#include "thread_pool.hpp"
#include "future_task_impl.hpp"
#include "utils.hpp" // RUNTEST REQUIRE
#include <iostream>

using namespace ilrd;
using namespace std;


boost::atomic_int counter(0);

class AddTask : public ThreadPool::Task
{
    public:
    void Do()
    {
        ++counter;
    }
       
};

void func()
{
    ++counter;
}

int future_func()
{
    int counter = 0;
    ++counter;
    return counter;
}

test_status_t AddAndRunTest()
{
    ThreadPool tp(SCHED_RR,4,3);
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new AddTask() ), ThreadPool::LOW );
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new AddTask() ), ThreadPool::LOW );
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new AddTask() ) );
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new AddTask() ) );
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new AddTask() ), ThreadPool::HIGH );
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new AddTask() ), ThreadPool::HIGH );
    tp.Run();
    sleep(1);
    REQUIRE(counter == 6);
    return PASSED;
}
test_status_t PauseTest()
{
    ThreadPool tp(SCHED_RR,4,3);
    counter = 0;
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new AddTask() ), ThreadPool::LOW );
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new AddTask() ), ThreadPool::LOW );
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new AddTask() ) );
    tp.Run();
    sleep(1);
    tp.Pause();
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new AddTask() ) );
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new AddTask() ), ThreadPool::HIGH );
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new AddTask() ), ThreadPool::HIGH );
    REQUIRE(counter == 3);
    tp.Run();
    sleep(5);
    REQUIRE(counter == 6);
    return PASSED;
}
test_status_t ResizeTest()
{
    ThreadPool tp(SCHED_RR,4,3);
    counter = 0;
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new AddTask() ), ThreadPool::LOW );
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new AddTask() ), ThreadPool::LOW );
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new AddTask() ) );
    tp.Run();
    sleep(1);
    tp.Resize(0);
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new AddTask() ) );
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new AddTask() ), ThreadPool::HIGH );
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new AddTask() ), ThreadPool::HIGH );
    sleep(1);
    REQUIRE(counter == 3);
    tp.Resize(2);
    sleep(1);
    REQUIRE(counter == 6);
    return PASSED;
}
test_status_t ExistingFuncTest()
{
    ThreadPool tp(SCHED_RR,4,3);
    counter = 0;
    boost::function<void ()> func_ = func;
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new FunctionTask(func_)), ThreadPool::LOW );
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new FunctionTask(func_) ), ThreadPool::LOW );
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new FunctionTask(func_) ) );
    tp.Run();
    sleep(1);
    tp.Resize(0);
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new FunctionTask(func_) ) );
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new FunctionTask(func_) ), ThreadPool::HIGH );
    tp.Add(boost::shared_ptr< ThreadPool::Task >( new FunctionTask(func_) ), ThreadPool::HIGH );
    sleep(1);
    REQUIRE(counter == 3);
    tp.Resize(2);
    sleep(1);
    REQUIRE(counter == 6);
    return PASSED;
}

test_status_t FutureFuncTest()
{
    ThreadPool tp(SCHED_RR,4,3);
    counter = 0;
    boost::function<int ()> func_ = future_func;
    FutureTask<int> *f = new FutureTask<int>(func_);
    boost::shared_ptr< ThreadPool::Task > shrd (f);
    tp.Add(shrd, ThreadPool::LOW );
    REQUIRE(false == f->IsReady());
    tp.Run();
    sleep(1);
    REQUIRE(true == f->IsReady());
    REQUIRE(1 == f->GetValue());
    
    return PASSED;

}
int main()
{
  
    RUNTEST(AddAndRunTest);
    RUNTEST(PauseTest);
    RUNTEST(ResizeTest);
    RUNTEST(ExistingFuncTest);
    RUNTEST(FutureFuncTest);
    return 0;

}