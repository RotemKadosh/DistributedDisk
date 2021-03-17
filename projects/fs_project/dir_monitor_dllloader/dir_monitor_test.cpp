#include "dir_monitor_impl.hpp"

using namespace ilrd;

test_status_t ConnectionTest()
{

    DirMonitor mon ("new_dir");
    DllLoader dll(&mon);
    sleep(3);
    std::ofstream a("new_dir/a");
    std::ofstream b("new_dir/b");
    std::ofstream c("new_dir/c");
    std::ofstream d("new_dir/d");
    
    std::remove("new_dir/a");
    std::remove("new_dir/b");
    std::remove("new_dir/c");
    std::remove("new_dir/d");
    sleep(1);
    return PASSED;

}


int main()
{
    RUNTEST(ConnectionTest);
    return PASSED;
}