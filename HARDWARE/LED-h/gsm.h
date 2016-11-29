#ifndef __GSM_H
#define __GSM_H	 
#include "sys.h"

typedef struct{
	char *id ;  //A id of cloud  lenth=6
	char time[20]; //2015-03-02T02:31:12   lenth=19
	int data; // or some unit
}GPRS_Data;
extern GPRS_Data Data; 

u8 call(void);
u8 gprs(u8 data);
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8 gprs_send( GPRS_Data  Data);
void change(char a,int b);

#endif


