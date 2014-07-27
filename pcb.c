#include "/usr/include/uarm/uARMconst.h"
#include "/usr/include/uarm/uARMtypes.h"
#include "/usr/include/uarm/libuarm.h"
#include "asl.h"

pcb_t procList[MAXPROC];
pcb_t* freeRoot=procList;
pcb_t* procQ;
void freePcb(pcb_t *p)
{
	p->p_next=freeRoot;
	freeRoot=p;
	return;
}
pcb_t *allocPcb(void)
{
	if(freeRoot==NULL)
		return NULL;
	else
	{
		pcb_t*res=freeRoot;
		freeRoot=freeRoot->p_next;
		res->p_next=NULL; 
                res->p_prnt=NULL;
		res->SYS5_Handler_New[0]=NULL;
		res->SYS5_Handler_New[1]=NULL;
		res->SYS5_Handler_New[2]=NULL;
		res->SYS5_Handler_Old[0]=NULL;
		res->SYS5_Handler_Old[1]=NULL;
		res->SYS5_Handler_Old[2]=NULL;
                res->p_child=NULL;
                res->p_sib=NULL;
		res->cpu_time=0;
        	(res->p_s).a1=0;
        	(res->p_s).a2=0;
        	(res->p_s).a3=0;
        	(res->p_s).a4=0;
        	(res->p_s).v1=0;
        	(res->p_s).v2=0;
        	(res->p_s).v3=0;
        	(res->p_s).v4=0;
        	(res->p_s).v5=0;
        	(res->p_s).v6=0;
        	(res->p_s).sl=0;
        	(res->p_s).fp=0;
        	(res->p_s).ip=0;
        	(res->p_s).sp=0;
        	(res->p_s).lr=0;
        	(res->p_s).pc=0;
        	(res->p_s).cpsr=0;
        	(res->p_s).CP15_Control=0;
        	(res->p_s).CP15_EntryHi=0;
        	(res->p_s).CP15_Cause=0;
		(res->p_s).TOD_Hi=0;
		(res->p_s).TOD_Low=0;
        	res->p_semAdd=NULL;
		return res;
	}
}
void initPcbs(void)
{
	int i;
	freeRoot=&procList[0];
	for(i=0;i<MAXPROC-1;i++)
	{
		procList[i].p_next=&(procList[i+1]);
	}
	procList[MAXPROC].p_next=NULL;
	return;
}
pcb_t *mkEmptyProcQ(void)
{
	procQ=NULL;
	return procQ;
}
int emptyProcQ(pcb_t *tp)
{
	if(tp==NULL)
		return 1;
	else
		return 0;
}
void insertProcQ(pcb_t **tp, pcb_t *p)
{
	if (tp==NULL) //bad address
		return;
	else if (*tp==NULL) //empty queue
	{
		*tp=p;
		p->p_next=p;
		return;
	}
	else
	{
		pcb_t* tmp=*tp;
		p->p_next=tmp->p_next;
		tmp->p_next=p;
		*tp=p;
		return;
	}
}
pcb_t *removeProcQ(pcb_t **tp)
{
	if (tp==NULL) //bad address
		return NULL;
	else if (*tp==NULL) //empty queue
		return NULL;
	else if ((*tp)->p_next==*tp) //single process
	{
		pcb_t* tmp=*tp;
		*tp=NULL;
		return tmp;
	}
	else
	{
		pcb_t* res=(*tp)->p_next;
		(*tp)->p_next=((*tp)->p_next)->p_next;
		return res;
	}
}

pcb_t *outProcQ(pcb_t **tp, pcb_t *p)
{
	if (tp==NULL) //bad address
		return NULL;
	else if (*tp==NULL) //empty queue
		return NULL;
	else if (*tp==p)
	{
		if ((*tp)->p_next==(*tp))
		{
			*tp=NULL;
			return p;
		}
		else
		{
			pcb_t* tmp=(*tp)->p_next;
			while(tmp->p_next!=*tp)
				tmp=tmp->p_next;
			tmp->p_next=(tmp->p_next)->p_next;
			*tp=(*tp)->p_next;
			//correggi puntatori
			return p;
		}
	}
	else
	{
		pcb_t*tmp=*tp;
		do
		{
			if (tmp->p_next==p)
			{
				tmp->p_next=(tmp->p_next)->p_next;
				return p;		
			}	
			else if (tmp->p_next==*tp) 
			{
				break;
			}
			else
				tmp=tmp->p_next;
		} while(tmp!=*tp);
		return NULL;
	}
}
pcb_t *headProcQ(pcb_t *tp)
{
	if (tp==NULL)
		return NULL;
	else
		return tp->p_next;
}

int emptyChild(pcb_t *p)
{
	if (p->p_child==NULL)
		return 1;
	else
		return 0;
}

pcb_t *collega (pcb_t *sib, pcb_t *p) /*restituisce il fratello precedente di p*/
{
    if(sib == p) /*il sib iniziale è uguale al primo figlio del nodo padre quindi se p==sib il padre non ha figli*/
        return NULL;
    else if (sib->p_sib == p)
        return sib;
    else
        return collega(sib->p_sib, p);
}
 
void insertChild(pcb_t *parent, pcb_t *p)
{
    pcb_t *tmp=collega(parent->p_child, NULL); /*in questo modo la funzione collega torna l'ultimo figlio del nodo*/
    if(tmp == NULL)
    {
        parent->p_child = p;
        p->p_prnt = parent;
        p->p_sib = NULL; /*si presume che il nodo in entrata non abbia fratelli*/
        p->p_child; /*ne figli, in caso contrario eliminare questa riga e la precedente*/
    }
    else
    {      
        tmp->p_sib = p;
        p->p_prnt = parent;
        p->p_sib = NULL; /*si presume che il nodo in entrata non abbia fratelli*/
        p->p_child; /*ne figli, in caso contrario eliminare questa riga e la precedente*/
    }
}
 
pcb_t *removeChild(pcb_t *p) /*dato che non era specificato tutti i figli del nodo da eliminare rimangono legate al nodo*/
{
    if (p == NULL || p->p_child == NULL)
        return NULL;
    else
    {
        pcb_t *tmp = p->p_child;
        p->p_child = tmp->p_sib;
        return tmp;
    }
}
 
pcb_t *outChild(pcb_t *p) /*dato che non era specificato tutti i figli del nodo da eliminare rimangono legate al nodo*/
{
    if(p == NULL || p->p_prnt == NULL)
        return NULL;
    else if (p == p->p_prnt->p_child)
    {
        p->p_prnt->p_child = p->p_sib;
        return p;
    }
    else
    {
        pcb_t *tmp = collega(p->p_prnt->p_child, p);
        tmp->p_sib = p->p_sib;
        return p;
    }
}
