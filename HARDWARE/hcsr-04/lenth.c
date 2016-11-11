#include "lenth.h"
#include "delay.h"

//初始化IIC
void Lenth_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimBaseStructure;
	TIM_ICInitTypeDef TIM1_ICInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;  //????
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//---------------------pwm_test----------------
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//---------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn; //TIM3 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; 
	NVIC_Init(&NVIC_InitStructure); 

	TIM_TimBaseStructure.TIM_Period = 10000;		//10000  ->100ms-> 17M
	TIM_TimBaseStructure.TIM_Prescaler = 7200; //7200  -> 10k 
	TIM_TimBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//????
//	TIM_TimBaseStructure.TIM_RepetitionCounter=
	TIM_TimBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1,&TIM_TimBaseStructure);
	
	TIM1_ICInitStructure.TIM_Channel = TIM_Channel_4; //选择输入端 IC1 映射到 TI1 上
	TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  //上升沿捕获
	TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到 TI1 上
	TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //配置输入分频,不分频
	TIM1_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM1, &TIM1_ICInitStructure);
	//---------------------pwm_test------------------------
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //脉宽调制模式 2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); //③初始化外设 TIMx
	TIM_CtrlPWMOutputs(TIM1,ENABLE);  //⑤MOE 主输出使能
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); //CH1 预装载使能 
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能 TIMx 在 ARR 上的预装载寄存器
	TIM_SetCompare1(TIM1,5000-1);
	//---------------------pwm_test------------------------TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //脉宽调制模式 2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
	TIM_OC4Init(TIM2, &TIM_OCInitStructure); //③初始化外设 TIMx
//	TIM_CtrlPWMOutputs(TIM2,ENABLE);  //⑤MOE 主输出使能
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); //CH1 预装载使能 
	TIM_ARRPreloadConfig(TIM2, ENABLE); //使能 TIMx 在 ARR 上的预装载寄存器
	TIM_SetCompare1(TIM2,500-1);
	TIM_ITConfig( TIM2,TIM_IT_Update,ENABLE);//允许更新中断和捕获中断
	//---------------------pwm_test2------------------------

	TIM_ITConfig( TIM1,TIM_IT_Update|TIM_IT_CC4,ENABLE);//允许更新中断和捕获中断
	
	TIM_Cmd(TIM1,ENABLE ); //使能定时器 2
}

void pwm_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// ①使能 tim1 时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
//①使能 GPIO 外设时钟使能
//设置该引脚为复用输出功能,输出 TIM1 CH1 的 PWM 脉冲波形
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);
TIM_TimeBaseStructure.TIM_Period = 10000;
//设置在下一个更新事件装入活动的自动重装载寄存器周期的值  80K
TIM_TimeBaseStructure.TIM_Prescaler =7200;
//设置用来作为 TIMx 时钟频率除数的预分频值 不分频
TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //②初始化 TIMx
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //脉宽调制模式 2
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
TIM_OC1Init(TIM1, &TIM_OCInitStructure); //③初始化外设 TIMx
TIM_CtrlPWMOutputs(TIM1,ENABLE);  //⑤MOE 主输出使能
//TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); //CH1 预装载使能 
//TIM_ARRPreloadConfig(TIM1, ENABLE); //使能 TIMx 在 ARR 上的预装载寄存器
TIM_Cmd(TIM1, ENABLE); //④使能 TIM1
}
u32 Tim1_Sta;

void TIM1_UP_IRQHandler(void)
{
TIM_ClearITPendingBit(TIM1,TIM_IT_Update); //清除中断标志位

}

void TIM1_CC_IRQHandler(void)
{u16 date;
if(TIM_GetITStatus(TIM1,TIM_IT_CC4)==SET)
{
	if ((Tim1_Sta&0x01)==0)
	{
		TIM_OC4PolarityConfig(TIM1,TIM_ICPolarity_Falling);
		TIM_SetCounter(TIM1,0);
		Tim1_Sta|=1;
	}
	else
	{
		date=TIM_GetCapture4(TIM1);
		TIM_OC4PolarityConfig(TIM1,TIM_ICPolarity_Rising);
		Tim1_Sta&=~(1);
		Tim1_Sta&=~(0xff<<16);
		Tim1_Sta|=date<<16;
	}
}
TIM_ClearITPendingBit(TIM1,TIM_IT_CC4); //清除中断标志位
}




