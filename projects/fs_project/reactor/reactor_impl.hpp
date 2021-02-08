#ifndef __RD94_REACTOR_IMPL_HPP__
#define __RD94_REACTOR_IMPL_HPP__

#include "reactor.hpp"


namespace ilrd
{

Reactor::Reactor():
m_stop_flag(true),
m_listener(new SelectListener())
{}

void Reactor::Add(int fd_, ModeType_ty mode_, boost::function<void ()> handler_)
{
    m_map[FdAndMode_ty(fd_,mode_)] = handler_;
}

void Reactor::Remove(int fd_, ModeType_ty mode_)
{
    if(0 == m_map.erase(FdAndMode_ty(fd_,mode_)))
    {
        throw std::logic_error ("key does not exist") ;
    }
}

void Reactor::InvokeHandlers()
{
    list_iter it;
    for(it = m_fd_handlers_to_invoke.begin(); it != m_fd_handlers_to_invoke.end() &&  !m_stop_flag.load(); ++it)
    {
       map_iter curr = m_map.find(*it);
       if(curr != m_map.end())
       {
            curr->second();
       }
    }
    m_fd_handlers_to_invoke.clear();
}

void Reactor::Run()
{
    m_stop_flag.store(false);

    while(!(m_stop_flag.load()))
    {
        m_listener->Do(&(m_map), &(m_fd_handlers_to_invoke));
        InvokeHandlers();
    }
}

void Reactor::Stop()
{
    m_stop_flag.store(true);
}

void Reactor::SelectListener::Do(const HandlerMap_ty *map, pair_list *fd_handlers_to_invoke)
{
    SetArgs(map);
    int ready_fd = select(m_max_fd, &(sets[READ]), &(sets[WRITE]), &(sets[EXCEPTION]), &(m_timeout));
    if(FAIL == ready_fd)
    {
        throw SelectFailException();
    }
    if(0 <= ready_fd)
    {
        CreateList(ready_fd, map, fd_handlers_to_invoke);
    }
}

void Reactor::SelectListener::SetArgs(const HandlerMap_ty *map)
{

    FD_ZERO(&(sets[READ]));
    FD_ZERO(&(sets[WRITE]));
    FD_ZERO(&(sets[EXCEPTION]));  
    m_timeout.tv_sec = 7;
    m_timeout.tv_usec = 0;

    int max_fd = 0;
    for(HandlerMap_ty::const_iterator it = map->begin(); it != map->end(); ++it)
    {
        int curr_fd = it->first.first;
        max_fd = std::max<int>(max_fd, curr_fd) ;
        fd_set *cur_fd_set = &(sets[it->first.second]);
        FD_SET(curr_fd, cur_fd_set);
    }
    m_max_fd = max_fd + 1;
}

Reactor::FdListener::~FdListener()
{}

Reactor::SelectListener::~SelectListener()
{}

void Reactor::SelectListener::CreateList(int ready_fd, const HandlerMap_ty *map, pair_list *fd_handlers_to_invoke)
{
    for(map_iter it = map->begin(); it != map->end() && 0 < ready_fd ; ++it)
    {   
        FdAndMode_ty cur_pair = it->first;
        int cur_fd =cur_pair.first;
        fd_set *cur_fd_set = &(sets[cur_pair.second]);
        if (FD_ISSET(cur_fd, cur_fd_set))
        {
            fd_handlers_to_invoke->push_front(cur_pair);
            --ready_fd;
        }
    }    
}


} // namespace ilrd





#endif //__RD94_REACTOR_IMPL_HPP__