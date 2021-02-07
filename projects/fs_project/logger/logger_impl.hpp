#ifndef __RD94_LOGGER_IMPL_HPP__
#define __RD94_LOGGER_IMPL_HPP__


#include "logger.hpp"
#include "singleton.hpp"
#include "waitable_q_impl.hpp"
#include "singleton_impl.hpp"

namespace ilrd
{  
    
    Logger::MessageLevel_t Logger::GetLevel() const
    {
        return m_current_level.load();
    }

    void Logger::SetLevel(MessageLevel_t level_)
    {
        m_current_level.store(level_);
    }
    
    void Logger::ThreadLogger()
    {
        std::string message_to_write;
        while(!(m_stop_flag.load()))
        {
            m_log_fd << message_to_write <<std::endl;
            m_message_q.Pop(&message_to_write);
        }
    }

    void Logger::Log(MessageLevel_t level_, const std::string& message_)
    {
        if(level_ <= m_current_level.load())
        {
            m_message_q.Push(message_);
        }
    }

    Logger::Logger():
    m_current_level(DEBUG),
    m_stop_flag(false),
    m_log_fd(),
    m_logging_thread(boost::thread(boost::bind(&Logger::ThreadLogger, this)))
    {
        char *path = getenv("LOG_PATH");
        if(NULL == path)
        {
            m_logging_thread.interrupt();
            m_logging_thread.join();
            throw LogException();
        }
        m_log_fd.open(path);
    }

    Logger::~Logger() noexcept
    {
        m_stop_flag.store(true);
        m_message_q.Push("");
        m_logging_thread.join();
        m_log_fd.close();
    }



}// ilrd

#endif //__RD94_LOGGER_IMPL_HPP__