#include <boost/thread/thread.hpp> // thread_group.create/ join all
#include "waitable_q.hpp"
#include "waitable_q_impl.hpp"
#include "pq_wrraper.hpp"
#include "pq_wrapper_impl.hpp"


using namespace ilrd;
test_status_t SingleThreadTest()
{
    int a = 7;
    int ans = 0;
    WaitableQueue< int, std::queue<int> > queue; 
    REQUIRE(true == queue.IsEmpty());
    queue.Push(a);
    REQUIRE(false == queue.IsEmpty());
    queue.Pop(&ans);
    REQUIRE(ans == a);
    REQUIRE(false == queue.Pop(&ans,boost::chrono::seconds(3)));
    return PASSED;
}

WaitableQueue< int, std::queue<int> > g_queue;
int ans = 0;
void reader();
void writer();
void block_reader();

test_status_t MultyThreadTest()
{
    boost::thread_group producer_threads, consumer_threads;
    
    consumer_threads.create_thread(reader);
    producer_threads.create_thread(writer);
    consumer_threads.join_all();
    REQUIRE(ans == 1);
    ans = 0;
    consumer_threads.create_thread(reader);
    consumer_threads.join_all();
    REQUIRE(ans == 1);
    ans = 0;
    
    consumer_threads.create_thread(reader);
    consumer_threads.join_all();
    REQUIRE(ans == 1);
    ans = 0;
    
    consumer_threads.create_thread(reader);
    consumer_threads.join_all();

    REQUIRE(ans == 0);
    consumer_threads.join_all();


    consumer_threads.create_thread(block_reader);
    sleep(5);
    producer_threads.create_thread(writer);
    consumer_threads.join_all();
    REQUIRE(ans == 1);
    ans = 0;
    consumer_threads.create_thread(block_reader);
    consumer_threads.join_all();
    REQUIRE(ans == 1);
    ans = 0;
    consumer_threads.create_thread(block_reader);
    consumer_threads.join_all();
    REQUIRE(ans == 1);
    ans = 0;
    consumer_threads.create_thread(reader);
    consumer_threads.join_all();

    REQUIRE(ans == 0);
    return PASSED;
}
void writer()
{
    int a = 1;
    g_queue.Push(a);
    g_queue.Push(a);
    g_queue.Push(a);
}
void reader()
{
    g_queue.Pop(&ans, boost::chrono::seconds(1));
}

void block_reader()
{
    g_queue.Pop(&ans);
}
test_status_t PqSingleThreadTest()
{
    int a = 7;
    int b = 3;
    int ans = 0;
    WaitableQueue< int, ilrd::PQWrapper<int> > queue; 
    REQUIRE(true == queue.IsEmpty());
    queue.Push(a);
    queue.Push(b);
    REQUIRE(false == queue.IsEmpty());
    queue.Pop(&ans);
    REQUIRE(ans == a);
    REQUIRE(true == queue.Pop(&ans,boost::chrono::seconds(3)));
    REQUIRE(ans == b);
    REQUIRE(false == queue.Pop(&ans,boost::chrono::seconds(3)));
    
    queue.Push(b);
    queue.Push(a);
    queue.Pop(&ans);
    REQUIRE(ans == a);
    REQUIRE(true == queue.Pop(&ans, boost::chrono::seconds(3)));
    REQUIRE(ans == b);
    REQUIRE(false == queue.Pop(&ans, boost::chrono::seconds(3)));

    

    return PASSED;
}

int main()
{
    RUNTEST(SingleThreadTest);
    RUNTEST(MultyThreadTest);
    RUNTEST(PqSingleThreadTest);
    return PASSED;
}