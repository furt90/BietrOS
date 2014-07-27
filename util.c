#include "/usr/include/uarm/uARMconst.h"
#include "/usr/include/uarm/uARMtypes.h"
#include "/usr/include/uarm/libuarm.h"
#include "/usr/include/uarm/arch.h"
#include "pcb.h"
#include "asl.h"

void stampa_cafona (int n)
{
		int i;
		tprint("******");
		for (i=0;i<n;i++)
			tprint("O");
		tprint("******");
		return;
}
char buffer[50];

void emptyBuf()
{
	int i;
	for(i=0;i<50;i++)
		buffer[i]=0;
}
int div(int x, int y)
{
	int ris=0;
	while(x>0)
	{
		x-=y;
		ris++;
	}
	ris--;
	return ris;
}
int mod(int x, int y)
{
	while(1)
	{
		if ((x-y)<0)
			return x;
		else
			x=x-y;
	}
}
char *_itoa(int value, char *string, int radix) {
    char tmp[33];
    char *tp = tmp;
    int i;
    unsigned v;
    int sign;
    char *sp;

    if (radix > 36 || radix <= 1) {
        return 0;
    }
    sign = (radix == 10 && value < 0);
    if (sign)
        v = -value;
    else
        v = (unsigned) value;
    while (v || tp == tmp) {
        i = mod(v,radix);
        v = div(v,radix);
        if (i < 10)
            *tp++ = i + '0';
        else
            *tp++ = i + 'a' - 10;
    }
    sp = string;
    if (sign)
        *sp++ = '-';
    while (tp > tmp)
        *sp++ = *--tp;
    *sp = 0;
    return string;
}

void memcopy(char* ptr1,char* ptr2,int size)
{
	int i;
	for(i=0;i<size;i++)
		ptr1[i]=ptr2[i];
	return;
}

char*pintel(int x)
{
	emptyBuf();
	return _itoa(x,buffer,10);
}
