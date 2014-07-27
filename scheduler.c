#include "/usr/include/uarm/uARMconst.h"
#include "/usr/include/uarm/uARMtypes.h"
#include "/usr/include/uarm/libuarm.h"
#include "/usr/include/uarm/arch.h"
#include "pcb.h"
#include "asl.h"
#include "global.h"
void scheduler()
{
	tprint("$");
	setSTATUS(STATUS_ALL_INT_DISABLE(getSTATUS()));
//	setSTATUS(STATUS_DISABLE_TIMER(getSTATUS()));
	if (currentProcess==NULL)		
	{
		pcb_t* newProc=removeProcQ(&readyQueue);
		if (newProc==NULL) //è finita la coda readyTM
		{
			if (processCount==0)
			{
				tprint("Sono finiti i brocessiTM\n");
				HALT();
			}
			if (processCount>0 && softBlockCount==0)
			{
				tprint("AGLIUTO, C'E DEADLOCK\n");
				PANIC();
			}
			if (processCount<0)
			{
				tprint("siamo nella merda\n");
				PANIC();			
			}			
			else
			{
				tprint("io aspetto che non c'è un cazzo da fare\n");
				WAIT();
			}
		}
		else
                {
                        currentProcess=newProc;
                        setTIMER(SCHED_TIME_SLICE);
                        LDST(&(newProc->p_s));
                }

	}
	else 
	{
		setTIMER(SCHED_TIME_SLICE);
		LDST(&currentProcess->p_s);
	}
}
