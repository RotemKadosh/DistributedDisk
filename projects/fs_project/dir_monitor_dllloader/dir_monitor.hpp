#ifndef __RD94_DIR_MONITOR_HPP__
#define __RD94_DIR_MONITOR_HPP__
#include <boost/thread/thread.hpp>// thread
#include <boost/atomic.hpp>// thread
#include <sys/inotify.h>  // struct inotify_event
#include <string>  //std::string
#include "dispatcher_callback.hpp"

namespace ilrd
{


typedef enum DirEventType
{
    ADD = 0,
    MODIFIED = 1,
    REMOVE = 2
}DirEventType_t;

typedef struct DirEvent
{
    DirEvent(const char *file_name, size_t len, DirEventType_t ev_type);
    std::string m_filename;
    DirEventType_t m_event_type;

}DirEvent_t;



class DirMonitor
{
public:

    explicit DirMonitor(const char *dir_path); // may throw MonitorFailException, bad_alloc, boost::thread_resource_error
    ~DirMonitor() noexcept;
    void StopMonitor();
    void Subscribe(CallbackBase<DirEvent_t> *cb_); //may throw bad_alloc

private:

    typedef enum state
    {
        RUN = 0,
        STOP = 1
    }state_t;

    void Monitor(); // may throw MonitorFailException
    static DirEvent_t TranslateINotifyToEvent(const struct inotify_event *event);
    void RemoveDir(const char *m_path);
    void CreateDir(const char *m_path);
    void check_add_inotify();


    const char *m_path;
    boost::shared_ptr<Dispatcher<DirEvent_t> > m_dispatcher; 
    int m_fd;
    int m_wfd;
    boost::shared_ptr<boost::thread> m_mon_thread;
    boost::atomic<state_t> m_flag;

    struct MonitorFailException : public std::exception
    {
        const char * what () const throw ()
        {
            return "Monitor Fail";
        }
    }; 
};


class DllLoader
{
public: 
    explicit DllLoader(DirMonitor *monitor_);
    ~DllLoader();
private:
    void DirMonitorDeath();
    void Notify(DirEvent dir_event);

    Callback<DirEvent_t, DllLoader> *m_callback; 

};

} //ilrd

#endif // __RD94_DIR_MONITOR_HPP__