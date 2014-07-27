#include "/usr/include/uarm/uARMconst.h"
#include "/usr/include/uarm/uARMtypes.h"
#include "/usr/include/uarm/libuarm.h"
#include "/usr/include/uarm/arch.h"
#include "pcb.h"
#include "asl.h"
#include "global.h"

pcb_t* readyQueue=NULL; //è la ready queue
int processCount=0; //numero di processi
int softBlockCount=0; //numero di processi nella softBlockQueue
pcb_t* currentProcess=NULL; //puntatore al processo corrente
cpu_t startTime=0; //angora null, pe mo non lo sappiamo
int sem[MAX_DEVICES]; //semafori dei device
unsigned int TimeSlice;
unsigned int PseudoClock;
unsigned int TimeEsec; //min(TimeSlice,PseudoClock)
unsigned int TimeBackground;//max(TimeSlice,PseudoClock)


int main()
{
	int i;
	pcb_t* testproc; //pcb per il processo test
	tprint("\n");
	tprint("biedrOS\n");
	tprint("V 0.56 - non funzioning n'at votTM\n");
	tprint("Settaggio registri\n");

	STST(pgmtrap_np);
	pgmtrap_np->pc=(unsigned int)trapHandler;//setto il program counter
	pgmtrap_np->sp=RAM_TOP; // setta lo stack pointer a ram top
	pgmtrap_np->cpsr=pgmtrap_np->cpsr & STATUS_CLEAR_MODE;//pulisce il registro di stato
	pgmtrap_np->cpsr=pgmtrap_np->cpsr | STATUS_SYS_MODE;//imposta kernel mode
	pgmtrap_np->cpsr=STATUS_ENABLE_TIMER(pgmtrap_np->cpsr);//abilita il timer
	pgmtrap_np->cpsr=STATUS_ALL_INT_DISABLE(pgmtrap_np->cpsr);//disabilita gli interrupt
	
	STST(int_np);
	int_np->cpsr=int_np->cpsr & STATUS_CLEAR_MODE;
	int_np->cpsr=int_np->cpsr | STATUS_SYS_MODE;
	int_np->cpsr=STATUS_ENABLE_TIMER(int_np->cpsr);
	int_np->cpsr=STATUS_ALL_INT_DISABLE(int_np->cpsr);
	int_np->pc=(unsigned int)intHandler;
	int_np->sp=RAM_TOP;

    	STST(sysbk_np);
	sysbk_np->cpsr=sysbk_np->cpsr & STATUS_CLEAR_MODE;
    	sysbk_np->cpsr=sysbk_np->cpsr | STATUS_SYS_MODE;
    	sysbk_np->cpsr=STATUS_ENABLE_TIMER(sysbk_np->cpsr);
    	sysbk_np->cpsr=STATUS_ALL_INT_DISABLE(sysbk_np->cpsr);
    	sysbk_np->pc=(unsigned int)bpHandler;
	sysbk_np->sp=RAM_TOP;

	STST(tlb_np);
	tlb_np->cpsr=tlb_np->cpsr & STATUS_CLEAR_MODE;
	tlb_np->cpsr=tlb_np->cpsr | STATUS_SYS_MODE;
    	tlb_np->cpsr=STATUS_ENABLE_TIMER(tlb_np->cpsr);
    	tlb_np->cpsr=STATUS_ALL_INT_DISABLE(tlb_np->cpsr);
    	tlb_np->pc=(unsigned int)tlbHandler;
	tlb_np->sp=RAM_TOP;

	tprint("Settaggio registri completato\n");
	tprint("Avvio strutture dati\n");
	initPcbs();
	initASL();
	for (i=0;i<MAX_DEVICES;i++)
	{
	        sem[i]=0;
	}
	tprint("Avvio strutture dati completato\n");
	tprint("Alloco processo iniziale\n");
	testproc=allocPcb();
	(testproc->p_s).pc=(unsigned int)test;
	(testproc->p_s).sp=RAM_TOP-FRAME_SIZE;
	(testproc->p_s).cpsr=(testproc->p_s).cpsr & STATUS_CLEAR_MODE;
	(testproc->p_s).cpsr=(testproc->p_s).cpsr | STATUS_SYS_MODE;
	(testproc->p_s).cpsr=STATUS_ALL_INT_DISABLE((testproc->p_s).cpsr); //brovvisorio
	(testproc->p_s).cpsr=STATUS_ENABLE_TIMER((testproc->p_s).cpsr);
	insertProcQ(&readyQueue,testproc);	
	processCount++;
	tprint("Processo iniziale allocato\n");
	tprint("KERNEL AVVIATO\n");
	tprint("chiamo lo scheduler\n");
	scheduler();
}
