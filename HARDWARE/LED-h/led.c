#include "led.h"
#include "sys.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK miniSTM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PA8和PD2为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 //使能PA,B,AFIO
 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//jatg
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED-->Pc.13 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOc13 led
 GPIO_SetBits(GPIOC,GPIO_Pin_13);						

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	    		 
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  	
GPIO_ResetBits(GPIOB,GPIO_Pin_3);						
GPIO_ResetBits(GPIOB,GPIO_Pin_4);						

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化 GPIOA15	
 						 
}
 
u8 scan(void)
//key  pb5 pu   
//return 0=> KEY=0=>  yes 
{

if(KEY==0)
{
delay_ms(10);
	if(KEY==0)
return 0;
}

return 1;
}
