#include "framework.hpp"
#include "factory_impl.hpp" //factory
#include "singleton_impl.hpp"// singleton::GetInstance
#include <iostream>

namespace ilrd
{

PlugPlay::PlugPlay(const char *dir_path): //may throw bed_alloc, 
m_dir_monitor(dir_path),
m_loader(&m_dir_monitor)
{

}


void FrameWork::RegisterProxy(int fd_, boost::shared_ptr<ProxyBase> proxy_)
{
    m_mediator.RegisterProxy(fd_, proxy_);
}

void FrameWork::RegisterCmd(std::string id, create_cmd_func_ty func)
{
    m_factory.Add(id, func);
}

FrameWork::FrameWork(): //may throw bed_alloc, bed_resource_error
m_log(Singleton<Logger>::GetInstance()),
m_factory(),
m_tp(new ThreadPool(2)),
m_mediator(m_tp, boost::bind(&FrameWork_factory_ty::Create, &m_factory, _1, _2)),
m_plug("/home/rotemkadosh27/git/projects/fs_project/master/monitor_dir")
{
      
}

FrameWork::~FrameWork()
{

}


}//ilrd

