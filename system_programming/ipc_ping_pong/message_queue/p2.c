#include <stdio.h>/*printf*/
#include <sys/ipc.h> /*ftok*/
#include <sys/msg.h>/*msgget, msgsnd, msgctl*/
#include "message.h"

#define FAIL (-1)
#define SUCCESS (0)
int main() 
{ 

    message_t message = {0, {0}};
    int msgid = 0;
    key_t key = ftok("progfile", 65); 
    if(FAIL == key)
    {
        return FAIL;
    }
    msgid = msgget(key, 0666 ); 
    if(FAIL == msgid)
    {
        return FAIL;
    }
    if(FAIL == msgrcv(msgid, &message, sizeof(message), 1, 0))
    {
        return FAIL;
    }

    printf("Data Received is: %s\n",message.mesg_text);
  
    
  
    return SUCCESS; 
} 