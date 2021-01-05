#include <string.h> /*strcpy*/
#include <sys/shm.h> /*ftok, shmget, shmat, shmdt */
#include <stdio.h> /*printf*/

#define FAIL (-1)

int main() 
{ 
    const char *msg = "p1 send is regards";
    char *str = NULL;
    int shm_id = 0;
    key_t key = ftok("txt", 65);
    if(FAIL == key)
    {
        perror("");
    }
  
    shm_id = shmget(key, 1024, 0666|IPC_CREAT);
    if(FAIL == shm_id)
    {
        perror("shmget failed: ");
        return FAIL;
    }
  
    str = (char*) shmat(shm_id, NULL, 0);
    if((void *)FAIL == str)
    {
        perror("shmat failed: ");
        return FAIL;
    }
    strcpy(str, msg);
    if(FAIL == shmdt(str))
    {
        perror("shmdt failed: ");
        return FAIL;
    } 
  
    return 0; 
} 
