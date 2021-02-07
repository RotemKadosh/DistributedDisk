#ifndef __RD94_DIR_MONITOR_HPP__
#define __RD94_DIR_MONITOR_HPP__

#include <string>  //std::string
#include <boost/thread.hpp> //thread
#include <boost/noncopyable.hpp> // noncopyable
#include "../dispatcher/dispatcher_callback.hpp"

namespace ilrd
{


typedef enum DirEventType
{

    ADD = 0,
    MODIFY = 1,
    REMOVE = 2

}DirEventType_t;

typedef struct DirEvent
{
    explicit DirEvent(const char *file_name, size_t len, DirEventType_t ev_type);
    std::string filename;
    DirEventType_t m_event_type;

} DirEvent_t;



class DirMonitor: private boost::noncopyable
{
public:

    explicit DirMonitor(const char *dir_path); // may throw thread_resource_error / MonitorFailException, bad_alloc
    ~DirMonitor() noexcept;
    void StopMonitor();
    void Subscribe(CallbackBase<DirEvent_t> *cb_);

private:

    boost::shared_ptr< Dispatcher<DirEvent_t> > m_dispatcher; 
    int m_fd;
    const char *m_path;
    boost::shared_ptr<boost::thread> m_thread_monitor;
    int m_watch_descriptor;
   
    typedef enum State
    {
        RUN = 0,
        STOP = 1
    } Mon_State_t;

    static const int FAIL = -1;
    boost::atomic<Mon_State_t> m_status;
    
    
    void Monitor(); // may throw MonitorFailException
    void AddWatch(); // may throw MonitorFailException
    void ReadEventsAndBroadcast();
    void BroadcastEventToSubs(DirEvent_t event);
   
    void RemoveDir(const char *m_path);
    void CreateDir(const char *m_path);
    
    struct MonitorFailException : public std::exception
    {
        const char * what () const throw ()
        {
            return "Monitor Fail";
        }
    }; 
};

class DllLoader: private boost::noncopyable
{
public: 
    explicit DllLoader(DirMonitor *monitor_);
    ~DllLoader();
private:
    void DirMonitorDeath();
    void Notify(DirEvent_t dir_event);
    Callback<DirEvent_t, DllLoader> *m_callback; 

};

} //ilrd


#endif //__RD94_DIR_MONITOR_HPP__