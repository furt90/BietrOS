#include "/usr/include/uarm/uARMconst.h"
#include "/usr/include/uarm/uARMtypes.h"
#include "/usr/include/uarm/libuarm.h"
#include "/usr/include/uarm/arch.h"
#include "pcb.h"
#include "asl.h"
#include "global.h"
extern void kill();
void trapHandler()
{
	tprint("E' UNA TRAPPOLA!!!\n");
	if (currentProcess->SYS5_Handler_New[0]==NULL)
		kill();
	else
	{
		memcopy((char*)currentProcess->SYS5_Handler_Old[1],(char*)tlb_op,sizeof(state_t));
		LDST(currentProcess->SYS5_Handler_New[1]);
	}
	scheduler();
}
