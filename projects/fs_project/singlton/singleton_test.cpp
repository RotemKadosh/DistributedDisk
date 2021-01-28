#include "singleton.hpp"
#include "singleton_impl.hpp"
#include <boost/thread.hpp>
#include "utils.hpp"

using namespace ilrd;
using namespace std;

class Obj
{
    public:
    int ma;

};



void thread1()
{
    Obj *ptr1 = Singleton<Obj>::GetInstance();
    ptr1->ma = 1;
}

void thread2()
{
    Obj *ptr1 = Singleton<Obj>::GetInstance();
    while (ptr1->ma != 1)
    {}
}


test_status_t SingleThreadTest()
{
    Obj *ptr1 = Singleton<Obj>::GetInstance();
    Obj *ptr2 = Singleton<Obj>::GetInstance();
    Obj *ptr3 = Singleton<Obj>::GetInstance();
    Obj *ptr4 = Singleton<Obj>::GetInstance();

    ptr1->ma = 10;
    REQUIRE(10 == ptr1->ma);
    REQUIRE(10 == ptr2->ma);
    REQUIRE(10 == ptr3->ma);
    REQUIRE(10 == ptr4->ma);

    return PASSED;

}
 

test_status_t MultiThreadTest()
{
    boost::thread_group tg;

    for(int i = 0; i < 1000 ; i++)
    {
        tg.create_thread(thread2);
    }
    tg.create_thread(thread1);
    tg.join_all();
    return PASSED;
}

int main()
{
    RUNTEST(SingleThreadTest);
    RUNTEST(MultiThreadTest);
    return PASSED;
}