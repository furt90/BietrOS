#include "/usr/include/uarm/uARMconst.h"
#include "/usr/include/uarm/uARMtypes.h"
#include "/usr/include/uarm/libuarm.h"
#include "/usr/include/uarm/arch.h"
#include "pcb.h"
#include "asl.h"
#include "global.h"
void test3();
int i=0;
void test()
{
	state_t t2,t3;
	STST(&t2); //obzionale
	STST(&t3);
	t2.pc=(unsigned int)test2;
	tprint("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	SYSCALL(CREATEPROCESS,(unsigned int)&t2,0,0);
	tprint("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	tprint("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	SYSCALL(PASSEREN,(unsigned int)&i,0,0);
	t3.pc=(unsigned int)test3;
	SYSCALL(CREATEPROCESS,(unsigned int)&t3,0,0);
	tprint("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	tprint("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	tprint("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	tprint("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	tprint("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	tprint("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	tprint("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	tprint("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	//SYSCALL(VERHOGEN,(unsigned int)&i,0,0);	
	SYSCALL(TERMINATEPROCESS,0,0,0);
/*
	tprint("ciao, sono il programma di test.\n");
	tprint("adesso faccio una novità brutta brutta (Y)\n");
	SYSCALL(CREATEPROCESS,(unsigned int)&ciao,0,0);
	tprint("Molti mi conoscono per il mio lavoro come scienziat nei laboratori LorealTM\n");
	tprint("Ma in realtà sono solo un brogramma di prova\n");
	tprint("Non faccio niente, sto solo consumando il tuo tempo\n");
	tprint("Dovresti killarmi, sai? Avanti, premi ctrl+c!\n");
	tprint("^C\n");
	tprint("...\n");
	tprint("che succede, non funziona?\n");
	tprint("ma guarda, chi è che non ha più la shell?\n");
	tprint("ci sei rimasto male?\n");
	tprint("eh eh? che c'è? vuoi il sistema operativo?\n");
	tprint("allora smettila di masturbarti e torna a scrivere lo scheduler\n");*/
}

void test2()
{
	tprint("ciao sono furt test\n");
	tprint("potresti pensare che vim sia utile\n");
	tprint("invece non serve a un cazzo e ti fa solo perdere tempo\n");
	SYSCALL(PASSEREN,(unsigned int)&i,0,0);
	tprint("e' crashare il brogetto\n");
	SYSCALL(TERMINATEPROCESS,0,0,0);
}
void test3()
{
	tprint("ciao sono test 3\n");
	tprint("molti di voi conosceranno gia i miei famosi fratelli\n");
	tprint("non è che faccio nulla di buono devo stare qui a scrivere robba\n");
	tprint("ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc\n");
	tprint("ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc\n");
	tprint("ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc\n");
	tprint("ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc\n");
	SYSCALL(PASSEREN,(unsigned int)&i,0,0);
	tprint("ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc\n");
	tprint("ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc\n");
	SYSCALL(TERMINATEPROCESS,0,0,0);
}
