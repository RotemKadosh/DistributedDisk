#include "reactor.hpp"
#include "reactor_impl.hpp"
#include <sys/stat.h> 
#include <fcntl.h>
#include "../utils/utils.hpp"
#include <iostream>
#include <boost/bind.hpp>
using namespace ilrd;

void writeF()
{
    static int times = 0;
    std::cout<< "this is write handler: "<< times <<"\n";
    times++;
}
void readF()
{
    static int times = 0;
    std::cout<< "this is read handler: "<< times <<"\n";
    times++;
}
void stopF( Reactor *r)
{
    r->Stop();
}

test_status_t ReactorAddRun()
{
    Reactor r;
    int fd1 = open("a", O_CREAT);
    int fd2 = open("b", O_CREAT);
    int fd3 = open("c", O_CREAT);
    int fd4 = open("d", O_CREAT);


    r.Add(fd1,Reactor::READ, &readF);
    r.Add(fd2,Reactor::READ, &readF);
    r.Add(fd3,Reactor::WRITE, &writeF);
    r.Add(fd4,Reactor::WRITE, &writeF);

    r.Add(STDIN_FILENO, Reactor::READ, boost::bind(&stopF,&r));

    r.Run();

    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);


    return PASSED;
}

int main()
{
    RUNTEST(ReactorAddRun);
    return PASSED;
}

