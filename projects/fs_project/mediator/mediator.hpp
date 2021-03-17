#ifndef __RD94_MEDIATOR_HPP__
#define __RD94_MEDIATOR_HPP__

#include <string> //string
#include <vector> //vector
#include <boost/unordered_map.hpp> // unordered_map
#include <boost/thread/thread.hpp> //thread
#include <boost/function.hpp> //function
#include <boost/shared_ptr.hpp> //shared_ptr

#include "utils.hpp" //utils
#include "reactor.hpp" //reactor
#include "thread_pool.hpp" //thread_pool
#include "future_task.hpp" //task


namespace ilrd
{

typedef int fd_ty;

class Command
{
public:
    Command(boost::shared_ptr<std::vector<char> > data_);
    virtual ~Command() noexcept;
    virtual void Run() = 0;

protected:
    boost::shared_ptr<std::vector<char> > m_data;
};



struct CmdData
{
    CmdData(std::string& id_, std::vector<char>& args_);
    // ~CmdData = default;
    std::string m_key;
    std::vector<char> m_args;
};

class ProxyBase
{
public:
    
    virtual ~ProxyBase() noexcept = 0;
    virtual boost::shared_ptr<CmdData> ParseData(fd_ty fd_) = 0;
};

class Mediator : private boost::noncopyable
{
public:
    typedef boost::shared_ptr<Command>  cmd_ptr;
    typedef boost::function<cmd_ptr (std::string, std::vector<char>)> factory_create_func_ty;
    typedef boost::shared_ptr<ThreadPool> thread_pool_ptr;
    typedef boost::shared_ptr<ProxyBase> proxy_ptr;

    explicit Mediator(thread_pool_ptr thread_pool_, factory_create_func_ty create_func_); //throws bad_alloc, 
    ~Mediator() noexcept;
    void RegisterProxy(fd_ty fd_, proxy_ptr proxy_); //throws bad_alloc, overwrites
    void UnRegisterProxy(fd_ty fd_); //throws logic_error 

private:

    class CmdFactoryTask : public ThreadPool::Task
    {
    public:    
        CmdFactoryTask(boost::shared_ptr<CmdData> data_, factory_create_func_ty create_func_);
        virtual ~CmdFactoryTask() noexcept;
        virtual void Do();

    private:
        boost::shared_ptr<CmdData> m_data;
        factory_create_func_ty m_create_func_; 
    };

    typedef boost::unordered_map< fd_ty , proxy_ptr > ProxyMap_ty;

    void AddFdAndProxy(fd_ty fd_, proxy_ptr proxy_);

    void ReactorHandler(fd_ty fd_);

    boost::shared_ptr<CmdData> GetCmdData(fd_ty fd_);
    boost::shared_ptr<ThreadPool::Task> GetTask(boost::shared_ptr<CmdData> data_);
    
    ProxyMap_ty m_proxy_map;
    thread_pool_ptr m_thread_pool;
    factory_create_func_ty m_create_func;
    Reactor m_reactor;
    boost::thread m_reactor_thread; 

};

}// ilrd

#endif // __RD94_MEDIATOR_HPP__