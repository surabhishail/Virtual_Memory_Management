#include "syscall.h"

int main()
{
	//int resultant_value;
	int write_value;
	//Taking 2 strings, string1 and string 2.
	// data type for the variable is char
	char* string1;
	//char* string2;
	//declare string and assigning the output to result and write

	string1 = "Write SYS CALL";
	 //resultant_value = ConsoleRead(string2,15);
	 write_value = ConsoleWrite(string1, 15);
	 ThreadExit(write_value);
	 return 1;
}
