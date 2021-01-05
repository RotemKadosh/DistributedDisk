
#include <sys/shm.h> /*ftok, shmget, shmat, shmdt */
#include <stdio.h> /*printf*/

#define FAIL (-1)

int main() 
{ 
    int shm_id = 0;
    char *str = NULL;
    key_t key = ftok("txt", 65);
    if(FAIL == key)
    {
        perror("");
    }

    shm_id = shmget(key, 1024, 0666);
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
  
    printf("Data read from memory: %s\n", str); 

    if(FAIL == shmdt(str))
    {
        perror("shmdt failed: ");
        return FAIL;
    } 
     
    return 0; 
} 
