#ifndef BIEDROS_CONST
#define BIEDROS_CONST
#define U32 unsigned int 
//device bit check
#define SPECEXCVECT 5
#define DEV_CHECK_ADDRESS_0 0x1
#define DEV_CHECK_ADDRESS_1 0x2
#define DEV_CHECK_ADDRESS_2 0x4
#define DEV_CHECK_ADDRESS_3 0x8
#define DEV_CHECK_ADDRESS_4 0x10
#define DEV_CHECK_ADDRESS_5 0x20
#define DEV_CHECK_ADDRESS_6 0x40
#define DEV_CHECK_ADDRESS_7 0x80

//device base address
#define DEV_BASE_ADDR 0x40
#define DISK_BITMAP 0x6FE0
#define TAPES_BITMAP 0x6FE4
#define NETWORK_BITMAP 0x6FE8
#define PRINTER_BITMAP 0x6FEC
#define TERMINAL_BITMAP 0x6FF0

#define GET_REGISTER(line,DevNo) (DEV_BASE_ADDR+((line - 3) * 0x80) + (DevNo * 0x10));

typedef U32 cpu_t;
typedef U32 pid_t;
extern char* pintel(int x);
extern void scheduler();
extern void trapHandler();
extern void intHandler();
extern void bpHandler();
extern void tlbHandler();
extern void test();
extern void test2();
extern pcb_t* softBlockQueue;
extern pcb_t* readyQueue;
extern int processCount;
extern int softBlockCount;
extern pcb_t* currentProcess;
extern cpu_t startTime;
extern int sem[MAX_DEVICES];
extern int int_stop;
extern void memcopy(char*ptr1,char*ptr2,int size);
//puntatori alle aree della ram
HIDDEN state_t* pgmtrap_np=(state_t*) PGMTRAP_NEWAREA;
HIDDEN state_t* int_np=(state_t*) INT_NEWAREA;
HIDDEN state_t* sysbk_np=(state_t*) SYSBK_NEWAREA;
HIDDEN state_t* tlb_np=(state_t*) TLB_NEWAREA;
HIDDEN state_t* pgmtrap_op=(state_t*) PGMTRAP_OLDAREA;
HIDDEN state_t* int_op=(state_t*) INT_OLDAREA;
HIDDEN state_t* sysbk_op=(state_t*) SYSBK_OLDAREA;
HIDDEN state_t* tlb_op=(state_t*) TLB_OLDAREA;

#endif
