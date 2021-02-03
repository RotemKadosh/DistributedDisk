#ifndef __RD94_DIR_MONITOR_IMPL_HPP__
#define __RD94_DIR_MONITOR_IMPL_HPP__

#include "dir_monitor.hpp"
#include "../dispatcher/dispatcher_callback_impl.hpp" //distpatcher 
#include <sys/inotify.h> //inotify_init
#include <assert.h> //assert
#include <iostream>// prints as stubs
#include <fstream> //ofstream

namespace ilrd
{

/*---------------------DirMonitor-------------------*/

DirMonitor::DirMonitor(const char *dir_path):
m_dispatcher(new Dispatcher<DirEvent_t>()),
m_fd(inotify_init()),
m_path(dir_path),
m_status(STOP)
{
    assert(NULL != m_path);
    
    if(FAIL == m_fd)
    {
        throw MonitorFailException();
    }
    try
    {
       m_thread_monitor = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&DirMonitor::Monitor, this)));
    }
    catch(const std::bad_alloc& e)
    {
        close(m_fd);
        throw e;
    }
    catch(const boost::thread_resource_error& e)
    {
        close(m_fd);
        throw e;
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
    m_watch_descriptor = inotify_add_watch(m_fd, m_path, IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_TO | IN_MOVED_FROM);
    if(FAIL == m_watch_descriptor)
    {
        throw MonitorFailException();
    }
}

DirEventType_t GetEventType(uint32_t mask)
{
    if(mask & IN_CREATE || mask & IN_MOVED_TO )
    {
        return ADD;
    }
    if(mask & IN_MODIFY)
    {
        return MODIFY;
    }
    if(mask & IN_DELETE || mask & IN_MOVED_FROM)
    {
        return REMOVE;
    }
    return ADD;
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
        m_dispatcher->Broadcast(e);
    }

}

void DirMonitor::Monitor()
{

    AddWatch();
    m_status.store(RUN);


    while (STOP != m_status.load())
    {
        ReadEventsAndBroadcast();
    }
}

void DirMonitor::CreateDir(const char *m_path)
{
    char path_name[FILENAME_MAX]  = {'\0'};
    strcat(path_name, m_path);
    strcat(path_name, "/dummy_die");
    bool ok = static_cast<bool>(std::ofstream(path_name));
    if(!ok)
    {
        std::cerr<<" mkdir dir fail\n";
    }
}

void DirMonitor::RemoveDir(const char *m_path)
{
    char path_name[FILENAME_MAX]  = {'\0'};
    strcat(path_name, m_path);
    strcat(path_name, "/dummy_die");
    std::remove(path_name);
}

void DirMonitor::StopMonitor()
{
    if(STOP != m_status.load())
    {
        m_status.store(STOP);
        CreateDir(m_path);
        m_thread_monitor->join();
        close(m_fd);
        close(m_watch_descriptor);
        RemoveDir(m_path);
    }
}

/*---------------------DlLoader-------------------*/

DllLoader::DllLoader(DirMonitor *monitor_):
m_callback(new Callback<DirEvent_t,DllLoader>(this, &DllLoader::Notify, &DllLoader::DirMonitorDeath))
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
    static const char * lut[] = {"ADD", "MODIFY", "REMOVE"};
    std::cout<< "on file: " << dir_event.filename <<"\n";
    std::cout<< "recieved event with type: " << lut[dir_event.m_event_type] << "\n";
}

void DllLoader::DirMonitorDeath()
{
    std::cout<< "DirMonitor Died\n";
}

DllLoader::~DllLoader()
{
    delete m_callback;
}
} //ilrd


#endif //__RD94_DIR_MONITOR_IMPL_HPP__