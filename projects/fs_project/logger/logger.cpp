#include <boost/filesystem.hpp>// 
#include "../waitable_queue/waitable_queue_impl.hpp" //WaitableQueue
#include "logger.hpp"
#include "config_manager.hpp"


namespace ilrd
{

Logger::Logger():
m_message_q(),
m_current_level(DEBUG),
m_stop_flag(false),
m_log_fd(GetLoggerPath().c_str()),
m_logging_thread(boost::thread(boost::bind(&Logger::ThreadLogger, this)))
{
}


std::string Logger::GetLoggerPath()
{
    const char *log_path = GET_CONFIG("LOG_PATH").c_str();

    if(NULL == log_path)
    {
        char buf[FILENAME_MAX];
        std::string path(getcwd(buf, sizeof(buf)));
        path += "/log";
        boost::filesystem::create_directory(path);
        path += "/log_file";
        return path;
    }

    return log_path;
}

Logger::~Logger() noexcept
{
    m_stop_flag.store(true);
    m_message_q.Push("stop");
    m_logging_thread.join();
    m_log_fd.close();
}

Logger::MessageLevel_t Logger::GetLevel() const
{
    return m_current_level.load();
}

void Logger::SetLevel(MessageLevel_t level_)
{
    m_current_level.store(level_);
}

void Logger::Log(MessageLevel_t level_, const std::string& message_)
{
    if(m_current_level.load() >= level_)
    {
        m_message_q.Push(message_);
    }
}

void Logger::ThreadLogger()
{
    std::string holder;
    while (true != m_stop_flag.load())
    {
        m_log_fd << holder << std::endl;
        m_message_q.Pop(&holder);
    }
}


} // namespace ilrd



