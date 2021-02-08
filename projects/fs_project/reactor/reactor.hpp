#ifndef __RD94_REACTOR_HPP__
#define __RD94_REACTOR_HPP__

#include <boost/unordered_map.hpp> //map
#include <list>
#include <boost/atomic.hpp> // boost::atomic
#include <boost/noncopyable.hpp> // noncopyable
#include <boost/function.hpp>    // function
#include <boost/shared_ptr.hpp> // shared_ptr
//#include "utils.hpp"

namespace ilrd
{

class Reactor : private boost::noncopyable
{
public:
    typedef enum ModeType
    {
        READ = 0,
        WRITE = 1,
        EXCEPTION = 2
    } ModeType_ty;

    explicit Reactor();

    //~Reactor() = default

    void Add(int fd_, ModeType_ty mode_, boost::function<void ()> handler_); //overwrites, throws
    void Remove(int fd_, ModeType_ty mode_); //throw logical_error exception
    void Run(); //blocking, until all FD's are removed or Stop() called //throws
    void Stop(); // not thread safe, non reentrant

private:
    typedef std::pair<int , ModeType_ty> FdAndMode_ty;
    typedef boost::unordered_map<FdAndMode_ty, boost::function<void ()> > HandlerMap_ty;
    typedef HandlerMap_ty::const_iterator map_iter;
    typedef std::list<FdAndMode_ty> pair_list;
    typedef pair_list::const_iterator list_iter;

    class FdListener
    {
    public:
        virtual ~FdListener();
        virtual void Do(const HandlerMap_ty *map, pair_list *fd_handlers_to_invoke) = 0;
    
    };

    class SelectListener : public FdListener
    {
    public:

        virtual void Do(const HandlerMap_ty *map, pair_list *fd_handlers_to_invoke);
        virtual ~SelectListener();

    private:
        void SetArgs(const HandlerMap_ty *map);
        void CreateList(int ready_fd, const HandlerMap_ty *map, pair_list *fd_handlers_to_invoke);
        
        fd_set sets[3];
        int m_max_fd;
        struct timeval m_timeout;

        struct SelectFailException : public std::exception
        {
            const char * what () const throw ()
            {
                return "Select Fail";
            }
        };
    };

    
    pair_list m_fd_handlers_to_invoke;
    HandlerMap_ty m_map;

    boost::atomic <bool> m_stop_flag;
    boost::shared_ptr<FdListener> m_listener;
    
    static const int FAIL = -1;


    void InvokeHandlers();
};

} // namespace ilrd

#endif //__RD94_REACTOR_HPP__