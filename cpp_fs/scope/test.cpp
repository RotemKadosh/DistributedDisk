#include "scop_lock_impl.hpp"
#include <boost/thread.hpp> //
#include <boost/thread/scoped_thread.hpp>
#include <boost/chrono.hpp>
using namespace ilrd;

pthread_mutex_t pthread_mut;
boost::mutex boost_mut;

int Pcounter = 0;

void *PLoop(void *args)
{
    ScopeLock<pthread_mutex_t >ScopeLock(pthread_mut);
    Pcounter++;
    std::cout<< "pthread counter:" << Pcounter << "\n";
    (void)args;
    return NULL;
}   

int Bcounter = 0;

void *BLoop(void *args)
{
    ScopeLock<boost::mutex> ScopeLock(boost_mut);
    Bcounter++;
    std::cout<< "boost counter:" << Bcounter << "\n";
    (void)args;
    return NULL;
} 

void *MutMay(void *args)
{
    ScopeLock<boost::mutex >ScopeLock(boost_mut);
    std::cout << "may has the mutex\n";
    (void)args;
    return NULL;
}

void *MutMoshe(void *args)
{
    ScopeLock<boost::mutex >ScopeLock(boost_mut);
    std::cout << "moshe has the mutex\n";
    (void)args;
    return NULL;
}

void *MutRotem(void *args)
{
    ScopeLock<boost::mutex >ScopeLock(boost_mut);
    std::cout << "rotem has the mutex\n";
    (void)args;
    return NULL;
}

void *PMay(void * args)
{
    ScopeLock<pthread_mutex_t >ScopeLock(pthread_mut);
    std::cout << "may has the mutex\n";
    (void)args;
    return NULL;
}

void *PMoshe(void * args)
{
    ScopeLock<pthread_mutex_t >ScopeLock(pthread_mut);
    std::cout << "moshe has the mutex\n";
    (void)args;
    return NULL;
}

void *PRotem(void * args)
{
    ScopeLock<pthread_mutex_t >ScopeLock(pthread_mut);
    std::cout << "rotem has the mutex\n";
    (void)args;
    return NULL;
}

int main()
{ 
    

    pthread_t tid[100];
    pthread_mutex_init(&pthread_mut, NULL);

    for(int i = 0 ; i < 100 ; ++i)
    {
        pthread_create(&(tid[i]), NULL, PLoop, NULL);
    }
    sleep(2);
    for(int i = 0 ; i < 100 ; ++i)
    {
        pthread_join(tid[i], NULL);
    }
    for(int i = 0 ; i < 100 ; ++i)
    {
        pthread_create(&(tid[i]), NULL, BLoop, NULL);
    }
    sleep(2);
    for(int i = 0 ; i < 100 ; ++i)
    {
        pthread_join(tid[i], NULL);
    }

    return 0;
}