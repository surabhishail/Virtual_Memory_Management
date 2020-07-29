/* Test file that triggers All syscalls in an Exec'd process */

#include "syscall.h"

int main() {
	// Exec process that triggers exception

	//Exec("../test/testp5_2_Normal", 0, 0, 0);
	//Exec("../test/testp5_2_Write", 0, 0, 0);
	//Yield();
	SpaceId space_pid = Exec("../test/UserPrgm");

	// Exec'd process should have terminated,
	// but this process should still reach this point and exit normally
	Join(space_pid);
	//Yield();
	Exit(0);

}