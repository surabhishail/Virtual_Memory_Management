/* This file is the file for implementation of Read and Write System calls.
     We have all the code inside main function and all the execution code 
     starts from this file in this function related to read and write system calls */
	 
#include "syscall.h"

int main()
{
	int resultant_value;
	int write_value;
	//Taking 2 strings, string1 and string 2.
	// data type for the variable is char
	char* string1;
	char* string2;
	//declare string and assigning the output to result and write

	string1 = "hey there";
	 resultant_value = ThreadRead(string2,3);
	 write_value = ThreadWrite(string1, 3);
	 // Halt the system calls via invoking the Halt()

	 Halt();
	 //write = Write(string1,5,0);
	 //ThreadYield(12);
	 //ThreadFork(11);
	 //ThreadExit(14);
	 return 1;
}
