#include "dir_monitor_impl.hpp"

using namespace ilrd;

test_status_t ConnectionTest()
{
    DirMonitor mon ("new_dir");
    DllLoader dll(&mon);
    sleep(10);
    return PASSED;
}


int main()
{
    RUNTEST(ConnectionTest);
    return PASSED;
}