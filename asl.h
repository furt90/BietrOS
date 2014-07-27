#ifndef ASL_H
#define ASL_H
/* Semaphore list handling functions */
#include "pcb.h"
typedef struct semd_t
{
	struct semd_t* s_next;
	int* s_semAdd;
	pcb_t* s_procQ;
}
semd_t;

extern void initASL(void);
extern int insertBlocked(int *semAdd, pcb_t *p);
extern pcb_t *removeBlocked(int *semAdd);
extern pcb_t *outBlocked(pcb_t *p);
extern pcb_t *headBlocked(int *semAdd);

#endif
