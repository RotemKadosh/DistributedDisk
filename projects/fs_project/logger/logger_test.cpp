#include "logger.hpp"
#include "logger_impl.hpp"

using namespace ilrd;

test_status_t LogSimpleTest()
{
    setenv("LOG_PATH", "log.txt", 2);
    Logger *log = LOGGER;
    LOG_ERROR("---------------------------");
    LOG_ERROR("test1");
    LOG_ERROR("---------------------------");
    log->Log(Logger::NONE, "this is NONE");
    log->Log(Logger::DEBUG, "this is DEBUG");
    log->Log(Logger::INFORMATION, "this is INFORMATION");
    log->Log(Logger::WARNING, "this is WARNING");
    log->Log(Logger::ERROR, "this is ERROR");
    sleep(1);
    return PASSED;
}

test_status_t LogTest()
{
    setenv("LOG_PATH", "log.txt", 2);
    Logger *log = LOGGER;
    LOG_ERROR("---------------------------");
    LOG_ERROR("test2");
    LOG_ERROR("---------------------------");
    log->Log(Logger::NONE, "this is NONE");
    log->Log(Logger::DEBUG, "this is DEBUG");
    log->Log(Logger::INFORMATION, "this is INFORMATION");
    log->Log(Logger::WARNING, "this is WARNING");
    log->Log(Logger::ERROR, "this is ERROR");

    LOG_ERROR("macro ERROR");
    LOG_WARNING("macro WARNING");
    LOG_INFO("macro INFO");
   
    sleep(1);

    return PASSED;
}

test_status_t LogLevelTest()
{
    setenv("LOG_PATH", "log.txt", 2);
    Logger *log = LOGGER;
    log->SetLevel(Logger::NONE);

    log->Log(Logger::NONE,"---------------------------");
    log->Log(Logger::NONE,"test3");
    log->Log(Logger::NONE,"---------------------------");
    log->Log(Logger::NONE, "this is NONE");
    log->Log(Logger::DEBUG, "this is DEBUG");
    log->Log(Logger::INFORMATION, "this is INFORMATION");
    log->Log(Logger::WARNING, "this is WARNING");
    log->Log(Logger::ERROR, "this is ERROR");

    LOG_ERROR("macro ERROR");
    LOG_WARNING("macro WARNING");
    LOG_INFO("macro INFO");
   
    sleep(1);

    return PASSED;
}

int main()
{
    RUNTEST(LogSimpleTest);
    RUNTEST(LogTest);
    RUNTEST(LogLevelTest);
    return PASSED;
}
