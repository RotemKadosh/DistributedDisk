#include <stdio.h>/*printf*/
#include <sys/ipc.h> /*ftok*/
#include <sys/msg.h>/*msgget, msgsnd, msgctl*/
#include <string.h> /*strcpy*/
#include "message.h" /*message_t*/


#define FAIL (-1)
#define SUCCESS (0)
 
int main() 
{ 
    int msgid = 0;
    message_t message1 = {0, {0}};
    message_t message2 = {0, {0}};
    message_t message3 = {0, {0}};
    // ftok to generate unique key 
    key_t key = ftok("progfile", 65); 
    if (FAIL == key)
    {
        return FAIL;
    }
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (FAIL == msgid)
    {
        return FAIL;
    }

    strcpy(message1.mesg_text, "p1 send a message\n");
    strcpy(message2.mesg_text, "p1 send a message\n");
    strcpy(message3.mesg_text, "p1 send a message\n");
    
    message1.mesg_type = 1;
    message2.mesg_type = 2; 
    message3.mesg_type = 3;  
    
    if (FAIL == msgsnd(msgid, &message1, sizeof(message1), 0) || 
        FAIL == msgsnd(msgid, &message2, sizeof(message2), 0) ||
        FAIL == msgsnd(msgid, &message3, sizeof(message3), 0))
    {
        return FAIL;
    }
    printf( "p1 has send\n");
   
    return 0; 
} 
