#include "mediator.hpp"
#include <iostream>
namespace ilrd
{

//---------------command-----------------
Command::Command(boost::shared_ptr<std::vector<char> > data_):
m_data(data_)
{}

//------------------cmd_data-----------------------
CmdData::CmdData(std::string& id_, std::vector<char>& args_):
m_key(id_),
m_args(args_)
{}

ProxyBase::~ProxyBase() noexcept
{
    //EMPTY
}

Command::~Command() noexcept
{
    //EMPTY
}

//------------------mediator------------------------
Mediator::Mediator(thread_pool_ptr thread_pool_, factory_create_func_ty create_func_):
m_thread_pool(thread_pool_),
m_create_func(create_func_),
m_reactor_thread(boost::thread(boost::bind(&Reactor::Run, &m_reactor)))
{
    m_thread_pool->Run();
}

Mediator::~Mediator() noexcept
{
    m_thread_pool->Stop();
    m_reactor.Stop();
    m_reactor_thread.join();
}

void Mediator::RegisterProxy(fd_ty fd_, proxy_ptr proxy_)
{
    m_proxy_map[fd_] = proxy_;
    m_reactor.Add(fd_, Reactor::READ, boost::bind(&Mediator::ReactorHandler, this, fd_));
}

void Mediator::ReactorHandler(fd_ty fd_)
{
   boost::shared_ptr<CmdData> cmd_ = GetCmdData(fd_);
   boost::shared_ptr<ThreadPool::Task> task_ = GetTask(cmd_);
   m_thread_pool->Add( task_ );
}
void Mediator::UnRegisterProxy(fd_ty fd_)
{
    m_proxy_map.erase(fd_);
    m_reactor.Remove(fd_, Reactor::READ);
} 

boost::shared_ptr<CmdData> Mediator::GetCmdData(fd_ty fd_)
{
    proxy_ptr prox = m_proxy_map[fd_];
    return prox->ParseData(fd_);
    
}
boost::shared_ptr<ThreadPool::Task> Mediator::GetTask(boost::shared_ptr<CmdData> data_)
{
   return boost::make_shared<CmdFactoryTask>(CmdFactoryTask(data_, m_create_func));
}
//-------------------CmdFactoryTask---------------------------
Mediator::CmdFactoryTask::CmdFactoryTask(boost::shared_ptr<CmdData> data_, factory_create_func_ty create_func_):
m_data(data_),
m_create_func_(create_func_)
{}
void Mediator::CmdFactoryTask::Do()
{
    m_create_func_(m_data->m_key, m_data->m_args)->Run();
}
Mediator::CmdFactoryTask::~CmdFactoryTask() noexcept
{}
}