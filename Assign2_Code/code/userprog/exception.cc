// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------

void fn(int x)
{
	std::cout << "Fork output";
}

void ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);

	DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

	switch (which)
	{
	case SyscallException:
		switch (type)
		{
		case SC_Halt:
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

			SysHalt();

			ASSERTNOTREACHED();
			break;
		
		case SC_Exit:
		{
			std::cout<<"\n After Matrix Multiplication Value is"<<kernel->machine->ReadRegister(4);
			exit((int)kernel->machine->ReadRegister(4));//will exit the whole current process
			return;
 
		}
    

		case SC_Add:
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

			/* Process SysAdd Systemcall*/
			int result;
			result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
							/* int op2 */ (int)kernel->machine->ReadRegister(5));

			DEBUG(dbgSys, "Add returning with " << result << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);

			/* Modify return point */
			{
				/* set previous programm counter (debugging only)*/
				kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

				/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
				kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

				/* set next programm counter for brach execution */
				kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
			}

			return;

			ASSERTNOTREACHED();

			break;

		case SC_ConsoleRead:
		{
			std::cout << "----------ConsoleRead Case in exception.cc----------\n"
					  << endl;
			int a = (int)kernel->machine->ReadRegister(4);
			int b = (int)kernel->machine->ReadRegister(5);
			int result = SysRead((int)a, (int)b);

			kernel->machine->WriteRegister(2, (int)result);
			{
				/* set previous programm counter (debugging only)*/
				kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

				/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
				kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

				/* set next programm counter for brach execution */
				kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
			}

			return;

			ASSERTNOTREACHED();
			break;
		}
			// case for Write System call
		case SC_ConsoleWrite:
		{

			std::cout << "----------ConsoleWrite  call in exception.cc--------\n"
					  << endl;
			int a = (int)kernel->machine->ReadRegister(4);
			int b = (int)kernel->machine->ReadRegister(5);
			int result = SysWrite((int)a, (int)b);

			kernel->machine->WriteRegister(2, (int)result);
			{
				/* set previous programm counter (debugging only)*/
				kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

				/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
				kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

				/* set next programm counter for brach execution */
				kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
			}
			return;

			ASSERTNOTREACHED();
			break;
		}

		case SC_ThreadFork: // case for Fork System call
		{
			std::cout << "---------ThreadFork  call in exception.cc---------\n"
					  << endl;

			Thread *t = new Thread("t1");
			t->Fork((VoidFunctionPtr)fn, (void *)0);

			{
				/* set previous programm counter (debugging only)*/
				kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

				/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
				kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

				/* set next programm counter for brach execution */
				kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
			}
			return;

			ASSERTNOTREACHED();
			break;
		}

		case SC_Exec:
		{
			std::cout << "---------Exec call in exception.cc---------\n"
					  << endl;
			std::cout << "Program To Execute is\n"
					  << kernel->machine->ReadRegister(4);

			// We are in the kernel space, we have to copy the name of the file
			// by translating using ReadMem
			char filename[100];
			int i = 0;
			int memval;

			int vaddr = (int)kernel->machine->ReadRegister(4);
			kernel->machine->ReadMem(vaddr, 1, &memval);
			while ((*(char *)&memval) != '\0')
			{
				filename[i] = (char)memval;
				++i;
				vaddr++;
				kernel->machine->ReadMem(vaddr, 1, &memval);
			}
			filename[i] = (*(char *)&memval);
			//StartProcess(filename);

			//filename[i]  = (char)memval;

			// The above is a direct copy of StartProcess, I didn't want to change
			// its scope so it has been included here
			OpenFile *executable = kernel->fileSystem->Open(filename);
			AddrSpace *space;
			//space->Load(filename);

			if (executable == NULL)
			{
				printf("Unable to open file %s\n", filename);
				return;
			}
			space = new AddrSpace(executable);
			kernel->currentThread->space = space;

			delete executable; // close file

			space->InitRegisters(); // set the initial register values
			space->RestoreState();  // load page table register

			kernel->machine->Run(); // jump to the user progam
			ASSERT(FALSE);			// machine->Run never returns;
									// the address space exits
									// by doing the syscall "exit"
			return;

			ASSERTNOTREACHED();
			break;
		}

			//Adding new case to handle Exit of current thread
		case SC_ThreadExit:
		{
			std::cout << "\n---------ThreadExit  call in exception.cc---------\n"
					  << endl;
			std::cout << "Exit called for the current thread :" << kernel->currentThread->getName() << "with ThreadId " << kernel->currentThread->thread_id << "   "<<kernel->machine->ReadRegister(4) << endl;
			 {
				/* set previous programm counter (debugging only)*/
				kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

				/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
				kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
				
				/* set next programm counter for brach execution */
				kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
			 }	
			kernel->currentThread->Finish();
			return;
		    ASSERTNOTREACHED();

			break;
		}


		//Default cases
		default:
			cerr << "Unexpected system call " << type << "\n";
			std::cout<<"-----------------------------------------------------------------------------------"<<endl;
	        std::cout<<"exiting the current thread : "<< kernel->currentThread->thread_id<<endl;
			std::cout<<"-----------------------------------------------------------------------------------"<<endl;
			kernel->currentThread->Finish();
			
			break;
		}
		break;

		//Adding new case to handle Page fault Exception
	case PageFaultException: // case for handling the page fault exceptions
	{
		cout<<"---------------PageFaultExecpection in Exeception.cc -------------\n";
		unsigned virtualPage = (unsigned)(kernel->machine->ReadRegister(BadVAddrReg) / PageSize);


		//Contents of the Needed Page
		char *vmContents = new char[PageSize];

		//Current Process Location in Page Table
		int newPageLocation = kernel->threadLocations[kernel->currentThread];

		//Read function for getting the Needed Page
		kernel->swapFile->ReadAt(vmContents, PageSize, newPageLocation + virtualPage * PageSize);

		//Virtual Address
		int virtualAddress = kernel->threadLocations[kernel->currentThread] + (virtualPage * PageSize);


		//Empty space finder in the RAM
		int availableFrame = kernel->machine->memoryStatus->FindAndSet();
        cout<< "------------Available Frames in RAM--------- ="<<availableFrame<<endl;

		if (availableFrame == -1)
		{
			cout<<"Swapping Pages"<<endl;
			int randomPage = rand() % NumPhysPages;
			cout<<"Random Page Selected for replacement is ="<<randomPage;
			int previousVirtualPage = kernel->machine->pageTable[randomPage].virtualPage;
			Thread *prevThreadid = kernel->machine->pageTable[randomPage].threadId;
			cout<<"Swap out from Virtual Page:"<<previousVirtualPage<<"Thread id :"<<(int)prevThreadid<<endl;


			char *pageContents = new char[PageSize];
			bcopy(&(kernel->machine->mainMemory[kernel->machine->pageTable[randomPage].physicalPage * PageSize]), pageContents, PageSize);
			kernel->swapFile->WriteAt(pageContents, PageSize, kernel->threadLocations[prevThreadid] + (previousVirtualPage * PageSize));

			kernel->machine->pageTable[randomPage].virtualPage = virtualPage;
			kernel->machine->pageTable[randomPage].valid = TRUE;
			kernel->machine->pageTable[randomPage].threadId = kernel->currentThread;
			cout<<"Swap in at Virtual Page:"<<virtualPage<<"Thread id :"<<(int)kernel->currentThread<<endl;


			bzero(&(kernel->machine->mainMemory[randomPage * PageSize]), PageSize);
			bcopy(vmContents, &(kernel->machine->mainMemory[randomPage * PageSize]), PageSize);

			delete pageContents;
		}
		else
		{
			kernel->machine->pageTable[availableFrame].threadId = kernel->currentThread;
			kernel->machine->pageTable[availableFrame].virtualPage = virtualPage;
			kernel->machine->pageTable[availableFrame].valid = TRUE;
			kernel->machine->pageTable[availableFrame].physicalPage = availableFrame;
			bzero(&(kernel->machine->mainMemory[availableFrame * PageSize]), PageSize);
			bcopy(vmContents, &(kernel->machine->mainMemory[availableFrame * PageSize]), PageSize);
		}
		delete vmContents;
		break;
	}
	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
		exit(0);
		break;
	}
     return;
	ASSERTNOTREACHED();
		
	//ASSERTNOTREACHED();
}
