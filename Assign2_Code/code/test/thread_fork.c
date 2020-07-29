/* This file is the file for implementation of Fork and yield System calls.
     We have all the code inside main function and all the execution code 
     starts from this file in this function related to Fork and yield system calls */



#include "syscall.h"

void forkTest(int a) 
{ char *c = "ForkTesting"; 
  ConsoleWrite(c, 12); 
  ThreadExit(0); 
}  

int main()
{
    int i = 0; 
    char *b = "ForkCalled"; 
    ConsoleWrite(b, 11); 
    i = ThreadFork(forkTest); 
    ThreadExit(i);
}	