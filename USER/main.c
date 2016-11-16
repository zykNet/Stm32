#include "delay.h"
#include "sys.h"
#include "usart.h"	  
#include "led.h"
//-------------1------------
#include "mpu6050.h"
#include <math.h>
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
//--------------2------------
#include "usmart.h"
#include "malloc.h"
//--------------3------------
#include "lenth.h"
#include "usart2.h"
#include "gsm.h"

void Length_Show(void)
{	u32 temp;
		PAout(12)=1;
		delay_ms(11);
		PAout(12)=0;
		temp=(Tim1_Sta>>16)/1000*17;
		printf("aacx is %d\r\n",temp);
		printf("ok \r\n");
}
void uart2_show(void)
{
if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;
				u2_printf("%s",USART2_RX_BUF);
				USART2_RX_STA=0;
			}
}
u8 call2(void)
{	char callbuf[]="18702625534";
	if(sim900a_send_cmd("AT+CLIP=1","OK",200))return 1;	//设置来电显示  
	if(sim900a_send_cmd("AT+COLP=1","OK",200))return 2;	//设置被叫号码显示
	u2_printf("ATD%s;\r\n",callbuf);//拨号
	delay_ms(3000);
	//u2_printf("AT+CLDTMF=2,\"%c\"\r\n",kbd_tbl[key-1][0])//dtmf
	//u2_printf("AT+VTS=%c\r\n",kbd_tbl[key-1][0]);
 	//while(1)
//	{
//		if(USART2_RX_STA&0X8000)
//		sim_at_response(0);
//		//if(0)break;
//	}
	sim900a_send_cmd("ATH","OK",100);//挂机
	sim900a_send_cmd("ATH","OK",100);//挂机
}
//********************************************
int main(void)
 {	
	 //------------int----------------------

u8 report=1;	 
	 int GuiJu,QvLv,ShuiPing,test=0;
	float test_roll=0;
Data.id="188";
Data.time="2015-03-02T02:31:12";
Data.data=1;
	 //-----------Init--------------------
	delay_init();	    	 //延时函数初始化	  
	uart_init(9600);
	LED_Init();
RELAY1 =0;	 
RELAY2 =1;
RELAY2 =0;//???
	 
	 usmart_dev.init(72);
	 
	 mem_init();
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 
	MPU_Init();
	

	 while(mpu_dmp_init());


//	 Lenth_Init();
//pwm_Init();	
USART2_Init(9600);
	while(1)
	{
		temp=MPU_Get_Temperature(); //得到温度值
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz); //得到加速度传感器数据
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz); //得到陀螺仪数据
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0);
		
		//4.5ms
		//{temp=(int)(roll*10);	printf("roll is %d\r\n",roll);}

	//if(report)mpu6050_send_data(aacz,ShuiPing,QvLv,0,0,0);//用自定义帧发送加速度和陀螺仪原始数据
	//if(report)usart1_report_imu(aacz,ShuiPing,QvLv,test,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));
	if (scan()) report =1;
		else report =0;
	ShuiPing=sin(roll*10)*1420;
	test=sin(test_roll*10)*1420;
		test_roll+=0.05;
	QvLv=(yaw-yaw_old)*1.0/3000;
	yaw_old=yaw;

	
// Length_Show();
//uart2_show();
//gprs_send(Data );
		LED=~LED;
//		RELAY1=~RELAY1;
//		RELAY2=~RELAY2;		
		delay_ms(30);
		RELAY1 =0;	 
		RELAY2 =0;

		
	}
 }

