#ifndef __LENGTH_H
#define __LENGTH_H
#include "sys.h" 
////////////////////////////////////////////////////////////////////////////////// 	
   	   		   
//IO方向设置
//GPIOB->CRH|=0X00000030;//PB9 推挽输出
//GPIOB->CRH&=0XFFFFFF0F; //PB9 设置成输入
void Lenth_Init(void);
extern u32 Tim1_Sta;

#endif
















