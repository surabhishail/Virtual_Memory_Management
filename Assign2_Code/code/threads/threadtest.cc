#include "kernel.h"
#include "main.h"
#include "thread.h"

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
        printf("*** thread %d looped %d times\n", which, num);
        kernel->currentThread->Yield();
    }
}
void 
CustCount(int len)
{
    for(int i=0;i<len;i++)
    {
        printf("I am customer %d\n",i);
    }
}

void
ThreadTest()
{
//    Thread *t = new Thread("forked thread");
//    t->Fork((VoidFunctionPtr) SimpleThread, (void *) 1);//SS For Nachos we pass like this with function name and value 
    
//    SimpleThread(0);
      int peak_cust = (rand()%((10-5)+5));
      CustCount(peak_cust);

}
