#include <fcntl.h>/*open, close, O_WRONLY, O_RDONLY*/ 
#include <sys/stat.h> /*mkfifo*/
#include <unistd.h> /*read, write*/
#include <stdio.h> /*perror, printf*/

#define FAIL (-1)
#define SUCCESS (0)


int main()  
{
    /*errors and release and headers*/
    int read_fd = 0;
    char message[5] = {0};
    int write_fd = 0;
    int i = 0;
    
    if(FAIL == mkfifo("/home/rotemkadosh27/git/system_programming/ipc_ping_pong/named_pipe/myfifo1", 0666 ))
    {
        return FAIL;
    }
   
    sleep(5);
    read_fd =  open("/home/rotemkadosh27/git/system_programming/ipc_ping_pong/named_pipe/myfifo2", O_RDONLY | O_NONBLOCK);
    if (FAIL == read_fd)
    {
        perror("open failed");
        return FAIL;
    }
    write_fd = open("/home/rotemkadosh27/git/system_programming/ipc_ping_pong/named_pipe/myfifo1", O_WRONLY );
    if (FAIL == write_fd) 
    {
        close(read_fd);
        perror("open failed"); 
        return FAIL;
    } 
   
    
    for(i = 0; i < 5; ++i)
    {
        if(FAIL == write(write_fd, "ping", 5))
        {
            perror("write failed");
            break;
        }
        if(FAIL == read(read_fd, message, 5))
        {
            perror("read failed");
            break;
        }
        printf("first- read: %s",message);
    }

    close(write_fd);
    close(read_fd);

    unlink("/home/rotemkadosh27/git/system_programming/ipc_ping_pong/named_pipe/myfifo2");
    
    return SUCCESS;
}