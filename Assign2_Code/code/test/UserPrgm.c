#include "syscall.h"
int main()
{
	//printf("Executing UserProgram for Execution Testing\n");
	int write_value;
	char* string1;
	string1 = "Write SYS CALL";
	 write_value = ConsoleWrite(string1, 15);
	 ThreadExit(write_value);
	 return 1;
	
}