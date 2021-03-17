#include <iostream> //vector
#include <sys/stat.h> 
#include <fcntl.h>
#include <vector> //vector
#include "thread_pool.hpp" //thread_pool
#include "factory_impl.hpp" //factory
#include "reactor_impl.hpp"
#include "mediator.hpp"
#include <boost/make_shared.hpp> 


using namespace ilrd;

class ProxyTest : public ProxyBase
{
 public:
    virtual ~ProxyTest() noexcept;
    virtual boost::shared_ptr<CmdData> ParseData(fd_ty fd_);
};

ProxyTest::~ProxyTest() noexcept
{
    //EMPTY   
}

boost::shared_ptr<CmdData> ProxyTest::ParseData(fd_ty fd_)
{
    std::string id("CommandTest");
    std::vector<char> arg;
    arg.push_back(3);

    return boost::shared_ptr<CmdData>(new CmdData(id, arg));
}


class CommandTest : public Command
{
public:
    virtual ~CommandTest() noexcept;
    virtual void Run();
};

CommandTest::~CommandTest() noexcept
{

}

void CommandTest::Run()
{
     std::cout<< "CommandTest Run func\n";
}


test_status_t test()
{
    int fd1 = open("a", O_CREAT);
    {
        boost::shared_ptr<ThreadPool> tp(new ThreadPool(2));
        Factory<Command, std::vector<char>, std::string> factory;
        Mediator med(tp, boost::bind(&Factory<Command, std::vector<char>, std::string>::Create, &factory, _1, _2));
        med.RegisterProxy(fd1, boost::shared_ptr<ProxyBase>(new ProxyTest()));

        write(fd1, "test", 5);
        
        sleep(10);
    }

    close(fd1);
    remove("a"); 
    return PASSED;
}

int main()
{
    RUNTEST(test);
   return 0; 
}