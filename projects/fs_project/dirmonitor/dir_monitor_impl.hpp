#ifndef __RD94_DIR_MONITOR_IMPL_HPP__
#define __RD94_DIR_MONITOR_IMPL_HPP__

#include "dir_monitor.hpp"
#include <sys/inotify.h> //inotify_init
#include <assert.h> //assert
#include <iostream>// prints as stubs

namespace ilrd
{

/*---------------------DirMonitor-------------------*/

DirMonitor::DirMonitor(const char *dir_path):
m_dispatcher(new Dispatcher<DirEvent_t>::Dispatcher()),
m_fd(inotify_init()),
m_path(dir_path),
m_status(STOP)
{
    assert(NULL != m_path);
    
    if(FAIL == m_fd)
    {
        delete m_dispatcher;
        throw MonitorFailException();
    }
    try
    {
       m_thread_monitor = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&DirMonitor::Monitor, this)));
    }
    catch(const std::exception& e)
    {
        delete m_dispatcher;
        throw MonitorFailException();;
    }
    
}

DirMonitor::~DirMonitor() noexcept
{
    StopMonitor();
}

void DirMonitor::Subscribe(CallbackBase<DirEvent_t> *cb_)
{
    m_dispatcher->Subscribe(cb_);
}

void DirMonitor::AddWatch()
{
    m_watch_descriptor = inotify_add_watch(m_fd, m_path, IN_CREATE || IN_DELETE || IN_MODIFY);
    if(FAIL == m_watch_descriptor)
    {
        throw MonitorFailException();
    }
}

DirEventType_t GetEventType(uint32_t mask)
{
    if(mask & IN_CREATE)
    {
        return ADD;
    }
    if(mask & IN_MODIFY)
    {
        return MODIFY;
    }
    else
    {
        return REMOVE;
    }
}

void DirMonitor::BroadcastEventToSubs(DirEvent_t e)
{
    m_dispatcher->Broadcast(e);
}

void DirMonitor::ReadEventsAndBroadcast()
{

    char buf[4096] __attribute__ ((aligned(__alignof__(struct inotify_event))));
    const struct inotify_event *event;
    ssize_t len = read(m_fd, buf, sizeof(buf));
    if(FAIL == len)
    {
        throw MonitorFailException();
    }
    for (char *ptr = buf; ptr < buf + len; ptr += sizeof(struct inotify_event) + event->len) 
    {
        event = (struct inotify_event *) ptr;
        DirEventType_t type = GetEventType(event->mask);
        DirEvent_t e = {event->name, type};
        BroadcastEventToSubs(e);
    }

}

void DirMonitor::Monitor()
{

    AddWatch();
    m_status.store(RUN);

    DirEvent_t event;

    while (STOP != m_status.load())
    {
        ReadEventsAndBroadcast();
    }
}

void TouchDir(const char *m_path)
{
    char command[FILENAME_MAX ]  = "touch ";
    strcat(command, m_path);
    system(command);
}

void DirMonitor::StopMonitor()
{
    if(STOP != m_status.load())
    {
        m_status.store(STOP);
        TouchDir(m_path);
        m_thread_monitor->join();
        close(m_fd);
    }
}

/*---------------------DlLoader-------------------*/

DllLoader::DllLoader(DirMonitor *monitor_):
m_callback(new Callback<DirEvent_t,DllLoader>::Callback(this, &(DllLoader::Notify), &(DllLoader::DirMonitorDeath)))
{
    assert(NULL != monitor_);
    try
    {
        monitor_->Subscribe(m_callback);
    }
    catch(const std::bad_alloc& e)
    {
        delete m_callback;
        throw e;
    }
   
}

void DllLoader::Notify(DirEvent_t dir_event)
{
    std::cout<< "recieved event with type: " << dir_event.m_event_type<<"\n";
    std::cout<< "on file: " << dir_event.filename <<"\n";
}

void DllLoader::DirMonitorDeath()
{
    std::cout<< "DirMonitor Died\n";
}


} //ilrd


#endif //__RD94_DIR_MONITOR_IMPL_HPP__