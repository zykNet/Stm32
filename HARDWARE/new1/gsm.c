//#include "myiic.h"
#include "delay.h"
#include "usart2.h"
#include "malloc.h"
#include <string.h>   
#include "text.h"
#include "gsm.h"

 GPRS_Data Data; 
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//usmart支持部分 
//将收到的AT指令应答数据返回给电脑串口
//mode:0,不清零USART2_RX_STA;
//     1,清零USART2_RX_STA;
void sim_at_response(u8 mode)
{
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		printf("%s",USART2_RX_BUF);	//发送到串口
		if(mode)USART2_RX_STA=0;
	} 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//ATK-SIM900A 各项测试(拨号测试、短信测试、GPRS测试)共用代码

//sim900a发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
u8* sim900a_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//向sim900a发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		while(DMA1_Channel7->CNDTR!=0);	//等待通道7传输完成   
		USART2->DR=(u32)cmd;
	}else u2_printf("%s\r\n",cmd);//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(sim900a_check_cmd(ack))break;//得到有效数据 
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//将1个字符转换为16进制数字
//chr:字符,0~9/A~F/a~F
//返回值:chr对应的16进制数值
u8 sim900a_chr2hex(u8 chr)
{
	if(chr>='0'&&chr<='9')return chr-'0';
	if(chr>='A'&&chr<='F')return (chr-'A'+10);
	if(chr>='a'&&chr<='f')return (chr-'a'+10); 
	return 0;
}
//将1个16进制数字转换为字符
//hex:16进制数字,0~15;
//返回值:字符
u8 sim900a_hex2chr(u8 hex)
{
	if(hex<=9)return hex+'0';
	if(hex>=10&&hex<=15)return (hex-10+'A'); 
	return '0';
}
u8 call(void)
{	char callbuf[]="18702625534";
	if(sim900a_send_cmd("AT+CLIP=1","OK",2000))return 1;	//设置来电显示  
	if(sim900a_send_cmd("AT+COLP=1","OK",200))return 2;	//设置被叫号码显示
	u2_printf("ATD%s;\r\n",callbuf);//拨号
	delay_ms(3000);
	//u2_printf("AT+CLDTMF=2,\"%c\"\r\n",kbd_tbl[key-1][0])//dtmf
	//u2_printf("AT+VTS=%c\r\n",kbd_tbl[key-1][0]);
 	//while(1)
	{
		if(USART2_RX_STA&0X8000)
		sim_at_response(0);
		//if(0)break;
	}
	sim900a_send_cmd("ATH","OK",100);//挂机
	sim900a_send_cmd("ATH","OK",100);//挂机
}

u8 gprs2(u8 data)
{
// 	sim900a_send_cmd("AT+CIPCLOSE=1","CLOSE OK",100);	//关闭连接
//	sim900a_send_cmd("AT+CIPSHUT","SHUT OK",100);		//关闭移动场景 

	sim900a_send_cmd("AT+CIPSHUT","OK",100);

	sim900a_send_cmd("AT+CSTT","OK",100);		//init ERROR
	sim900a_send_cmd("AT+CIICR","OK",100);		//init2  ERROR  1=10ms
	
	sim900a_send_cmd("AT+CIPSTART=\"TCP\",\"api.heclouds.com\",80","OK",100);		//link  ERROR	
	sim900a_send_cmd("AT+CIPSEND=223",">",100);		//send  ERROR
	u2_printf("User-Agent: Fiddler\r\n\
	api-key: XgD=K7o=EhKrAbiWDgr462z1pQU=\r\n\
	Host: api.heclouds.com\r\nContent-Length: 46\r\n\
	\r\n{\"temperature\":{\"2015-03-2T02:31:12\":22.5}}");
	sim900a_send_cmd("AT+CIPSHUT","OK",100);
}


/*
in GPRS_Data 
out 0 --ok
	1 --net?
	10 --big wrong 
*/


u8 gprs_send( GPRS_Data  Data)
{
	int length_all=30,length_data=4;//strlen()
	u8 *p1,*p2;//p1 cmd!, p2, data
	u8 res=0;
	//--------------


	p1 = mymalloc(20);
	p2 = mymalloc(200);
	if(p1==NULL)res= 10;
	if(p2==NULL)res= 10;
 	sim900a_send_cmd("AT+CIPCLOSE=1","CLOSE OK",100);	//关闭连接
	if(sim900a_send_cmd("AT+CIPSHUT","SHUT OK",100))res= 10;		//关闭移动场景 
	sim900a_send_cmd("AT+CSTT","OK",100);		//init ERROR
	sim900a_send_cmd("AT+CIICR","OK",100);		//init2  ERROR
	if(sim900a_send_cmd("AT+CIPSTART=\"TCP\",\"api.heclouds.com\",80","OK",1000))res= 1;	//link  ERROR
	
	if (res!=0) goto end;
	
	length_all+=strlen(Data.id);
	sprintf((char*)p2,"User-Agent: ssim900A\r\napi-key: XgD=K7o=EhKrAbiWDgr462z1pQU=\r\nHost: api.heclouds.com\r\nContent-Length: %d\r\n\r\n{\"%s\":{\"%s\":%d}}\r\n",length_all,Data.id,Data.time,Data.data);//4位数据
	sprintf((char*)p1,"AT+CIPSEND=%d",strlen(p2));//4位数据
	sim900a_send_cmd(p1,">",100);		//length_all//send  ERROR 
	u2_printf(p2);
	delay_ms(10);
	sim900a_send_cmd("AT+CIPSHUT","OK",100);
	
	end:
	myfree(p1);
	myfree(p2);
	return res;
	
}

void change(char a,int b)
{
Data.id=&a;
Data.time="2015-03-02T02:31:12";
Data.data=b;
}
void get_time(void)
{



}