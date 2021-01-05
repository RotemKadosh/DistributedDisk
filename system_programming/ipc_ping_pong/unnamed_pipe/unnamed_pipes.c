#include <unistd.h>/*read, write, pipe*/ 
#include <stdio.h>/*printf, perror*/
#include <sys/types.h>/*pid_t, fork*/

#define FAIL (-1)
#define SUCCESS (0)
void ClosePipes(int *ping, int *pong);
int main() 
{
   int i = 0;
   int parent_to_child[2], child_to_parent[2];
   char pipe1writemessage[5] = "ping";
   char pipe2writemessage[5] = "pong";
   char readmessage[5];
   int pid = 0;

   if (FAIL == pipe(parent_to_child) || FAIL == pipe(child_to_parent)) 
   {

      return FAIL;
   }
  
   pid = fork();
   if (0 != pid)
   { 

      for(i = 0; i < 5; ++i)
      {
         if(FAIL == write(parent_to_child[1], pipe1writemessage, sizeof(pipe1writemessage)))
         {
            perror("write error");
         }

         if(FAIL == read(child_to_parent[0], readmessage, sizeof(readmessage)))
         {
            perror("read error");
         }

         printf("In Parent: Reading from pipe 2 – %s \n", readmessage);
      } 
      ClosePipes(parent_to_child,child_to_parent);
   } 
   else 
   { 


      for(i = 0; i < 5; ++i)
      {
         if(FAIL == read(parent_to_child[0], readmessage, sizeof(readmessage)))
         {
            perror("read error");
         }
         printf("In Child: Reading from pipe 1 –  %s \n", readmessage);
         if(FAIL == write(child_to_parent[1], pipe2writemessage, sizeof(pipe2writemessage)))
         {
            perror("write error");
         }
      }

      ClosePipes(parent_to_child,child_to_parent);
   }
   
   return SUCCESS;
}

void ClosePipes(int *parent_to_child, int *child_to_parent)
{
    if(FAIL == close(child_to_parent[0]))
    {
        perror("close error: ");
    }
    if(FAIL == close(child_to_parent[1]))
    {
        perror("close error: ");
    }
        if(FAIL == close(parent_to_child[0]))
    {
        perror("close error: ");
    }
    if(FAIL == close(parent_to_child[1]))
    {
        perror("close error: ");
    }
}

