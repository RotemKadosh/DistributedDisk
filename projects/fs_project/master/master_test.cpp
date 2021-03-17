#include "master.hpp"
#include "singleton_impl.hpp"
#include "logger.hpp"
#include "framework.hpp"

int main()
{
    using namespace ilrd;
    //setenv("LOG_PATH", "log.txt", 2);
    LOG_ERROR("START");
    FrameWork *fw = Singleton<FrameWork>::GetInstance();
    LOG_ERROR("FRAME");
    system("mv ./libreadcmd.so ./monitor_dir/");
    system("mv ./libwritecmd.so ./monitor_dir/");
    LOG_ERROR("mv");
    sleep(3);
    Master *master = Singleton<Master>::GetInstance();
    LOG_ERROR("master");
    master->InitConnection("4");
    while (true)
    {
        
    }
    
    return 0;
}