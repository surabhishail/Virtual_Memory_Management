/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"




void SysHalt()
{
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

int SysRead (int buffer,int size)
{
   // to read  something
	char* a = "Hi Console Read Here";
    int b = (int)(*a);
	for(int i = 0; i<size; i++){
		
		if(kernel->machine->WriteMem(buffer,1,b)){
            buffer++;
        }
		a++;
		b++;
	}
	return 1;
}

int SysWrite (int buffer,int size)
{
    // to Write something inside this function
    for(int i=0;i<size;i++)
    {
    	int b;

    	if(!kernel->machine->ReadMem(buffer,1,&b))
    	{
			continue;
    	}
		char c = (char)b;
    	cout<<c;
    	
    	buffer++;
    }
    return 1;
}
 






#endif /* ! __USERPROG_KSYSCALL_H__ */
