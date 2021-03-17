#include <iostream>
#include "/home/rotemkadosh27/git/projects/fs_project/singlton/singleton.hpp"

#include "/home/rotemkadosh27/git/projects/fs_project/logger/logger.hpp"
#include "/home/rotemkadosh27/git/projects/fs_project/logger/logger_impl.hpp"
#include "/home/rotemkadosh27/git/projects/fs_project/waitable_queue/waitable_q.hpp"
#include "/home/rotemkadosh27/git/projects/fs_project/waitable_queue/waitable_q_impl.hpp"
using namespace ilrd;

extern "C" void print() 
{
    std::cout << "address of logger in other : " << Singleton<Logger>::GetInstance() << std::endl;
}