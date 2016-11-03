#include "delay.h"
#include "sys.h"
#include "usart.h"	  
#include "led.h"

#include "mpu6050.h"
//ALIENTEK miniSTM32开发板实验1
//跑马灯实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
 int main(void)
 {	
	 //------------int----------------------
	short aacx,aacy,aacz;  //加速度传感器原始数据
	short gyrox,gyroy,gyroz; //陀螺仪原始数据
	short temp;  //温度
	 float pitch,roll,yaw; //欧拉角
	 //-----------Init--------------------
	delay_init();	    	 //延时函数初始化	  
	 uart_init(115200);
	LED_Init();		  	//初始化与LED连接的硬件接口
	MPU_Init();
	 while(mpu_dmp_init());
	while(1)
	{
		temp=MPU_Get_Temperature(); //得到温度值
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz); //得到加速度传感器数据
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz); //得到陀螺仪数据
		printf("aacx is %d\r\n",aacy);

	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
	{	temp=(int)(roll*10);	printf("roll is %d\r\n",roll);}
		LED1=~LED1;
		delay_ms(300);
	}
 }

