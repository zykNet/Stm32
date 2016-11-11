#ifndef __GSM_H
#define __GSM_H	 
#include "sys.h"

u8 call(void);
u8 gprs(u8 data);
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
void LED_Init(void);//≥ı ºªØ
#endif


