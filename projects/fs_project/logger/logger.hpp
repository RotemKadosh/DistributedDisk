#ifndef __RD94_LOGGER_HPP__
#define __RD94_LOGGER_HPP__

#include <string>//std::string
#include <fstream>//std::ofstream
#include <boost/atomic.hpp>   //boost::atomic
#include <boost/thread/mutex.hpp> //mutex
#include <boost/thread.hpp>
#include "waitable_queue.hpp" //WaitableQueue
#include "singleton.hpp" //Singleton
#include "singleton_impl.hpp" //Singleton

namespace ilrd
{ 


#define LOGGER (Singleton<Logger>::GetInstance())
#define LOG_ERROR(message_) ((LOGGER)->Log(Logger::ERROR, message_))
#define LOG_WARNING(message_) ((LOGGER)->Log(Logger::WARNING, message_))
#define LOG_INFO(message_) ((LOGGER)->Log(Logger::INFORMATION, message_))
   

class Logger
{
public:
    //envarment variable LOG_PATH is revserved for log file path may throw LogException

    typedef enum MessageLevel
    {
        NONE = 0,
        ERROR,
        WARNING,
        INFORMATION,
        DEBUG
    } MessageLevel_t;

    MessageLevel_t GetLevel() const;
    void SetLevel(MessageLevel_t level_);

    void Log(MessageLevel_t level_, const std::string& message_); // may throw Bad_alloc

    struct LogException : public std::runtime_error
    {
        LogException(): std::runtime_error(""){}
        const char * what () const throw ()
        {
            return "Log Fail";
        }
    };

private:
    friend class Singleton<Logger>;
    explicit Logger(); //envarment variable LOG_PATH is revserved for log file path may throw LogException
    ~Logger() noexcept;
    void ThreadLogger();
    std::string GetLoggerPath();

    WaitableQueue< std::string > m_message_q;
    boost::atomic< MessageLevel_t > m_current_level;
    boost::atomic< bool > m_stop_flag;
    std::ofstream m_log_fd;
    /* boost::mutex::mutex m_message_lock;*/
    boost::thread m_logging_thread;


};

}// ilrd

#endif //__RD94_LOGGER_HPP__