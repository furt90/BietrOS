#include "/usr/include/uarm/uARMconst.h"
#include "/usr/include/uarm/uARMtypes.h"
#include "/usr/include/uarm/libuarm.h"
#include "/usr/include/uarm/arch.h"
#include "pcb.h"
#include "asl.h"
#include "global.h"
void v(int* s)
{
	pcb_t *tmp;
	*s++;
	tmp=removeBlocked(s);
	if(tmp)
	{
		insertProcQ(&readyQueue, tmp);
		softBlockCount--;
	}
	memcopy((char*)&(currentProcess->p_s),(char*)sysbk_op,sizeof(state_t));
	
	return;
}

void p(int*s)
{
	stampa_cafona(*s);
	(*s)--;	
	if(*s<0)
	{
		softBlockCount++;
		insertBlocked(s, currentProcess);
		outProcQ(&readyQueue,currentProcess);
		currentProcess=NULL;
	}
	else
	{
		tprint("Semaforo positivo: miracolo!\n");
		memcopy((char*)&(currentProcess->p_s),(char*)sysbk_op,sizeof(state_t));
	}
	return;
}

void crepa_progenie(pcb_t* p)
{
	pcb_t *tmp;
	if (p==NULL) return;
	crepa_progenie(p->p_child);
	crepa_progenie(p->p_sib);
	tmp=outProcQ(&readyQueue,p);
	if (tmp!=NULL)
	{
		freePcb(tmp);
	}
	tmp=outBlocked(p);
	if (tmp!=NULL)
	{
		softBlockCount--;
		freePcb(tmp);
	}
	processCount--;
	return;
}

void kill()
{
	pcb_t *p,*tmp;
	tmp=outChild(currentProcess);//torna l'albero da eliminare e NULL se è radice o se il nodo è NULL
	if(tmp) 
		p=tmp;
	else
		p=currentProcess;
	crepa_progenie(p);
	currentProcess=NULL;
	return;
}
void create(state_t* p)
{
	pcb_t* newProc=allocPcb();
	if (newProc==NULL)
	{
		sysbk_op->v1=-1;	
		return;	
	}
	newProc->p_prnt=currentProcess;
	memcopy((char*)&(newProc->p_s),(char*)p,sizeof(state_t));
	insertChild(currentProcess,newProc);
	insertProcQ(&readyQueue,newProc);
	processCount++;
	memcopy((char*)&(currentProcess->p_s),(char*)sysbk_op,sizeof(state_t));
	return;
}
void get_cpu_time()
{
	sysbk_op->v1=currentProcess->cpu_time;
	return;
}
void wait_clock()
{
	p(&(sem[MAX_DEVICES]));
	return;
}
void wait_io(int line, int devNo,int isRead)
{
	SYSCALL(PASSEREN,sem[((devNo-3))*8+line],0,0);
	if (line==INT_TERMINAL)
	{
		
	}
	else
	{
	}
	return;
}
void specify_exception_state_vector(int type,state_t*oldp,state_t*newp)
{
	if (currentProcess->SYS5_Handler_New[type]!=NULL) //richiesta di un handler per un'eccezione già gestita: kill
	{
		kill();
		return;
	}
	else
	{
		currentProcess->SYS5_Handler_New[type]=newp;
		currentProcess->SYS5_Handler_Old[type]=oldp;
		return;
	}
	
}
void bpHandler()
{
	unsigned int causa=sysbk_op->a1;
	if (currentProcess->SYS5_Handler_New[2]!=NULL)
	{	
		memcopy((char*)currentProcess->SYS5_Handler_Old[2],(char*)tlb_op,sizeof(state_t));
		LDST(currentProcess->SYS5_Handler_New[2]);
	}
	else
	{
		switch(causa)
		{
			case CREATEPROCESS:
				create((state_t*)sysbk_op->a2);
				break;
			case TERMINATEPROCESS:
				kill();
				break;
			case VERHOGEN:
				v((int*)sysbk_op->a2);
				break;
			case PASSEREN:
				p((int*)sysbk_op->a2);
				break;
			case SPECEXCVECT:
				specify_exception_state_vector(sysbk_op->a2,(state_t*)sysbk_op->a3,(state_t*)sysbk_op->a4);
				break;
			case GETCPUTIME:
				get_cpu_time();
				break;
			case WAITCLOCK:	
				wait_clock();
				break;
			case WAITIO:
				wait_io(sysbk_op->a2,sysbk_op->a3,sysbk_op->a4);
				break;
			default:
				kill();
				break;
		}
	}
	scheduler();
}
