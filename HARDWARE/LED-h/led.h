#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED		 PCout(13)	 	
#define RELAY1 	 PBout(3)	 
#define RELAY2	 PBout(4)	
#define KEY		 PBin(5)

u8 scan(void);
void LED_Init(void);//≥ı ºªØ
#endif


