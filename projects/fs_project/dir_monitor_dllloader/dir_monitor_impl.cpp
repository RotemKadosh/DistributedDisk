#include <iostream>//cout, cerr
#include <string>//string
#include <cassert>//assert
#include <fstream> //ofstream
#include <sys/inotify.h> //inotify_init
#include <dlfcn.h>//dlopen, RTLD_LAZY
#include "dir_monitor.hpp"
#include "dispatcher_callback_impl.hpp"
#include "logger.hpp"
#include "singleton.hpp"

namespace ilrd
{

//-------------------------------DirMonitor---------------------------------

DirMonitor::DirMonitor(const char *dir_path):
m_path(dir_path),
m_dispatcher(new Dispatcher<DirEvent_t>),
m_fd(inotify_init()),
m_flag(RUN)
{
    assert(NULL != m_path);
    check_add_inotify();
    try
    {
        m_mon_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&DirMonitor::Monitor, this)));
    }
    catch(const std::bad_alloc& e)
    {
        close(m_wfd);
        close(m_fd);
        std::cerr << e.what() << '\n';
        //LOG_INFO("plug & play ctor ");
        throw e;
    }
    catch(const boost::thread_resource_error& e)
    {
        close(m_wfd);
        close(m_fd);
        std::cerr << e.what() << '\n';
        throw e;
    }
}

void DirMonitor::check_add_inotify()
{
    if(-1 == m_fd)
    {
        LOG_ERROR(" DirMonitor::check_add_inotify 51 fail ");
        throw MonitorFailException();
    }
    m_wfd = inotify_add_watch(m_fd, m_path, IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_TO | IN_MOVED_FROM);
    if(-1 == m_wfd)
    {
        close(m_fd);
        LOG_ERROR(" DirMonitor::inotify_add_watch 57 fail ");
        perror("DirMonitor::inotify add watch");
        throw MonitorFailException();
    }
}

DirMonitor::~DirMonitor() noexcept
{
    StopMonitor();
}

void DirMonitor::CreateDir(const char *m_path)
{
    char path_name[FILENAME_MAX]  = {'\0'};
    strcat(path_name, m_path);
    strcat(path_name, "/dummy_die");
    bool ok = static_cast<bool>(std::ofstream(path_name));
    if(!ok)
    {
        LOG_ERROR(" DirMonitor::mkdir dir fail ");

    }
}

void DirMonitor::RemoveDir(const char *m_path)
{
    char path_name[FILENAME_MAX] = {'\0'};
    strcat(path_name, m_path);
    strcat(path_name, "/dummy_die");
    std::remove(path_name);
}

void DirMonitor::StopMonitor()
{
    if(STOP != m_flag.load())
    {
        m_flag.store(STOP);
        CreateDir(m_path);
        m_mon_thread->join();
        RemoveDir(m_path);
        close(m_wfd);
        close(m_fd);
    }
}


void DirMonitor::Subscribe(CallbackBase<DirEvent_t> *cb_) //may throw bad_alloc
{
    m_dispatcher->Subscribe(cb_);
}

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
void DirMonitor::Monitor() // may throw MonitorFailException
{
    while (STOP != m_flag)
    {
        char buf[EVENT_BUF_LEN]__attribute__ ((aligned(__alignof__(struct inotify_event))));
        memset(buf, 0, sizeof(buf));
        ssize_t len = read(m_fd, buf, sizeof(buf));
        if(0 >= len)
        {
            //std::cout<<" read 118 fail\n";
            throw MonitorFailException();
        }
        const struct inotify_event *event = NULL;
        for (char *ptr = buf; ptr < buf + len; ptr += EVENT_SIZE + event->len)
        {
            if(STOP != m_flag)
            {
                event = reinterpret_cast<const struct inotify_event *>(ptr);
                m_dispatcher->Broadcast(TranslateINotifyToEvent(event));
            }
            else
            {
                break;
            }
        }
    }
}

DirEvent_t DirMonitor::TranslateINotifyToEvent(const struct inotify_event *event)
{

    if (event->mask & IN_CREATE || event->mask & IN_MOVED_TO)
    {
        return DirEvent_t(event->name, event->len, ADD);
    }
    else if (event->mask & IN_DELETE || event->mask & IN_MOVED_FROM)
    {
        return DirEvent_t(event->name, event->len, REMOVE);
    }

    return DirEvent_t(event->name, event->len, MODIFIED);
}

//-----------------------------------DllLoader-----------------------------

DllLoader::DllLoader(DirMonitor *monitor_):
m_callback(new Callback<DirEvent_t,DllLoader>(this, &DllLoader::Notify, &DllLoader::DirMonitorDeath))
{
    try
    {
        monitor_->Subscribe(m_callback);
    }
    catch(const std::bad_alloc& e)
    {
        delete m_callback;
        m_callback = NULL;
        throw e;
    }   
}    

DllLoader::~DllLoader()
{
    delete m_callback;
    m_callback = NULL;
}

void DllLoader::DirMonitorDeath()
{
    std::cout << "DirMonitor died\n";
}

void DllLoader::Notify(DirEvent_t dir_event)
{
    if(ADD == dir_event.m_event_type)
    {
       void *handle = dlopen(dir_event.m_filename.c_str(), RTLD_LAZY);
       if(handle == NULL) 
       {
           LOG_ERROR(dir_event.m_filename.c_str());
           LOG_ERROR("dlopen fail:");
           LOG_ERROR(dlerror());
          
       }

    }

    else if(REMOVE == dir_event.m_event_type)
    {
        std::cout << dir_event.m_filename << " removed\n";
    }

    else if(MODIFIED == dir_event.m_event_type)
    {
        std::cout << dir_event.m_filename << " modified\n";
    }
    
}

DirEvent::DirEvent(const char *file_name, size_t len, DirEventType_t ev_type):
m_filename(std::string(file_name, len)),
m_event_type(ev_type)
{
    //EMPTY  
}

}//ilrd
