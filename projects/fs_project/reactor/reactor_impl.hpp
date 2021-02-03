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
    m_map[FdAndMode_ty(fd_,mode_)]= handler_;
}

void Reactor::Remove(int fd_, ModeType_ty mode_)
{
    if( 0 == m_map.erase(FdAndMode_ty(fd_,mode_)))
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
            (curr->second)();
       }
    }
    m_fd_handlers_to_invoke.clear();
}

void Reactor::Run()
{
    m_stop_flag.store(false);

    while(!(m_stop_flag.load()))
    {
        InvokeHandlers();
        m_listener->Do(&(m_map), &(m_fd_handlers_to_invoke));
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

    
    for(int i = 0; i< 3; ++i)
    {
        FD_ZERO(&(sets[i]));
    }

    m_timeout.tv_sec = 7;
    m_timeout.tv_usec = 0;

    int max_fd = 0;
    for(HandlerMap_ty::const_iterator it = map->begin(); it != map->end(); ++it)
    {
        int curr_fd = it->first.first;
        max_fd = std::max<int>(max_fd, curr_fd) ;

        FD_SET(curr_fd, &(sets[it->first.second]));
    }
    m_max_fd = max_fd + 1;
}

void Reactor::SelectListener::CreateList(int ready_fd, const HandlerMap_ty *map, pair_list *fd_handlers_to_invoke)
{
    for(map_iter it = map->begin(); it != map->end() && ready_fd >0 ; ++it)
    {
        if (FD_ISSET(it->first.first, &(sets[it->first.second])))
        {
            fd_handlers_to_invoke->push_front(it->first);
            --ready_fd;
        }
    }    
}


} // namespace ilrd





#endif //__RD94_REACTOR_IMPL_HPP__