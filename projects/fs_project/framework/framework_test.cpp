#include "framework.hpp"
#include "singleton_impl.hpp"
#include <iostream>
using namespace ilrd;
int main()
{
    FrameWork *fw = Singleton<FrameWork>::GetInstance();

    unsigned int sleep_time = 15;
    while(sleep_time != 0)
    {
        sleep_time = sleep(sleep_time);

    } 

    std::cout << "hello" << std::endl;


    return 0;
}
