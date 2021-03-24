#include "master.hpp"
#include "singleton_impl.hpp"
#include "logger.hpp"
#include "framework.hpp"
#include "config_manager.hpp"

int main()
{
    using namespace ilrd;
    FrameWork *fw = Singleton<FrameWork>::GetInstance();
    (void)(fw);
    std::string mv_read_cmd("mv ./libreadcmd.so ");
    std::string mv_write_cmd("mv ./libwritecmd.so ");
    std::string monitor_dir = GET_CONFIG("PLUG_PATH");


    system((mv_read_cmd + monitor_dir).c_str());
    system((mv_write_cmd + monitor_dir).c_str());

    sleep(3);
    Master *master = Singleton<Master>::GetInstance();

    master->InitConnection("8");
    while (true)
    {
        
    }
    
    return 0;
}