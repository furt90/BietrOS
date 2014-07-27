#include "/usr/include/uarm/uARMconst.h"
#include "/usr/include/uarm/uARMtypes.h"
#include "/usr/include/uarm/libuarm.h"
#include "/usr/include/uarm/arch.h"
#include "pcb.h"
#include "asl.h"
#include "global.h"
int getDevice(int*bitmap)
{
	if ((*bitmap)==((*bitmap) | DEV_CHECK_ADDRESS_0))
		return 0;
	if ((*bitmap)==((*bitmap) | DEV_CHECK_ADDRESS_1))
                return 1;
	if ((*bitmap)==((*bitmap) | DEV_CHECK_ADDRESS_2))
                return 2;
	if ((*bitmap)==((*bitmap) | DEV_CHECK_ADDRESS_3))
                return 3;
	if ((*bitmap)==((*bitmap) | DEV_CHECK_ADDRESS_4))
                return 4;
	if ((*bitmap)==((*bitmap) | DEV_CHECK_ADDRESS_5))
             	return 5;
	if ((*bitmap)==((*bitmap) | DEV_CHECK_ADDRESS_6))
                return 6;
        else
	        return 7;
	
}
void handleTerminal(int line)
{	
	termreg_t*baseAddr=NULL;
	int*bitmap;
        int i=0,DevNo=0;
        bitmap=(int*)TERMINAL_BITMAP;
        DevNo=getDevice(bitmap);
        //baseAddr=(termreg_t*)(DEV_BASE_ADDR+((line - 3) * 0x80) + (DevNo * 0x10));
        baseAddr=(termreg_t*)GET_REGISTER(line,DevNo);
	if ((baseAddr->recv_status & 0xFF)==DEV_TRCV_S_CHARRECV)
        {
                baseAddr->recv_command=DEV_C_ACK;
        }
        else if ((baseAddr->transm_status & 0xFF)==DEV_TTRS_S_CHARTRSM)
        {
                baseAddr->transm_command=DEV_C_ACK;
        }
	return;
}
void handleDevice(int line)
{
	int DevNo=0;
	int*bitmap;
	dtpreg_t* baseAddr=NULL;
	switch(line)
	{
		case INT_DISK:
			bitmap=(int*)DISK_BITMAP;
			break;
		case INT_TAPE:
			bitmap=(int*)TAPES_BITMAP;
			break;
		case INT_UNUSED:
			bitmap=(int*)NETWORK_BITMAP;
			break;
		case INT_PRINTER:
			bitmap=(int*)PRINTER_BITMAP;
			break;
		default: break;
	}
 	DevNo=getDevice(bitmap);
        baseAddr=(dtpreg_t*)GET_REGISTER(line,DevNo);
        baseAddr->command=DEV_C_ACK;
	return;
}
void timerHandler()
{
	memcopy((char*)&(currentProcess->p_s),(char*)int_op,sizeof(state_t));
      	(currentProcess->p_s).pc-=8;
       	insertProcQ(&readyQueue,currentProcess);
        currentProcess=NULL;
}
void intHandler()
{
	int*bitmap;
	int DevNo;
	termreg_t*baseAddr=NULL; //for devices
	
	if  (CAUSE_IP_GET(int_op->CP15_Cause, INT_TIMER)) /*end of timeslice*/
		timerHandler();
	else
	{
		if (CAUSE_IP_GET(int_op->CP15_Cause,INT_TERMINAL))
			handleTerminal(INT_TERMINAL);
		else if (CAUSE_IP_GET(int_op->CP15_Cause,INT_TAPE))
			handleDevice(INT_TAPE);
  		else if (CAUSE_IP_GET(int_op->CP15_Cause,INT_DISK))
        	        handleDevice(INT_DISK);
        	else if (CAUSE_IP_GET(int_op->CP15_Cause,INT_UNUSED))
        	        handleDevice(INT_UNUSED);
        	else if (CAUSE_IP_GET(int_op->CP15_Cause,INT_PRINTER))
        	        handleDevice(INT_PRINTER);
	}	
	scheduler();
}
