#include "reactor.hpp"
#include "reactor_impl.hpp"
#include <sys/stat.h> //open
#include <fcntl.h>
#include "../utils/utils.hpp"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

using namespace ilrd;

void writeF()
{
    static int times = 0;
    std::cout<< "this is write handler: "<< times <<"\n";
    times++;
    sleep(1);
}
void readF()
{
    static int times = 0;
    std::cout<< "this is read handler: "<< times <<"\n";
    times++;
    sleep(1);
}
void stopF( Reactor *r)
{
    r->Stop();
}
void stopT( Reactor *r)
{
    sleep(5);
    r->Stop();
}
void removeF(Reactor *r, int fd1, int fd2, Reactor::ModeType_ty type)
{
    r->Remove(fd1, type);
    r->Remove(fd2, type);
}

test_status_t ReactorAddRun()
{
    Reactor r;
    int fd1 = open("a", O_TMPFILE | O_RDWR);
    int fd2 = open("b", O_TMPFILE | O_RDWR);
    int fd3 = open("c", O_TMPFILE | O_WRONLY);
    int fd4 = open("d", O_TMPFILE | O_WRONLY);

    r.Add(STDIN_FILENO, Reactor::READ, boost::bind(&stopF,&r));

    r.Add(fd1,Reactor::READ, &readF);
    r.Add(fd2,Reactor::READ, &readF);
    r.Add(fd3,Reactor::WRITE, &writeF);
    r.Add(fd4,Reactor::WRITE, &writeF);

    std::cout<< "press Enter to STOP\n";
    
    r.Run();

    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);
    remove("a");
    remove("b");
    remove("c"); 
    remove("d");   
    return PASSED;
}

test_status_t ReactorAddRemoveOnGo()
{
    Reactor r;
  
    int fd3 = open("c", O_CREAT);
    int fd4 = open("d", O_CREAT);


    r.Add(fd3,Reactor::READ, &readF);
    r.Add(fd4,Reactor::READ, boost::bind(&removeF,&r ,fd3,fd4, Reactor::READ));

    //r.Add(STDIN_FILENO, Reactor::READ, boost::bind(&stopF,&r));
    boost::thread t (boost::bind(&stopT,&r) );
    r.Run();
    t.join();

    close(fd3);
    close(fd4);

    remove("c"); 
    remove("d");   
    return PASSED;
}

test_status_t ReactorDontWake()
{
    Reactor r;
    int fd1 = open("a", O_CREAT | O_WRONLY);
    int fd2 = open("b", O_CREAT | O_WRONLY);
    int fd3 = open("c", O_CREAT | O_RDONLY);
    int fd4 = open("d", O_CREAT | O_RDONLY);


    //r.Add(STDIN_FILENO, Reactor::READ, boost::bind(&stopF,&r));
    r.Add(fd1, Reactor::READ, &readF);
    r.Add(fd2, Reactor::READ, &readF);
    r.Add(fd3, Reactor::WRITE, &writeF);
    r.Add(fd4, Reactor::WRITE, boost::bind(&removeF,&r ,fd3, fd4,Reactor::WRITE));


    boost::thread t (boost::bind(&stopT,&r) );

    r.Run();

    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);
    remove("a");
    remove("b");
    remove("c"); 
    remove("d");   
    return PASSED;
}


int main()
{
    RUNTEST(ReactorAddRun);
    RUNTEST(ReactorAddRemoveOnGo);
    RUNTEST(ReactorDontWake);
    return PASSED;
}

