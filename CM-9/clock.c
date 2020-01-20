/*
 * clock.c
 *
 * Created: 11/19/2013 4:48:02 PM
 *  Author: Bill
 */

#include <stdlib.h>
#include "motion.h"
#include "led.h"
#include "RS485.h"
#include "clock.h"
#include "AX12.h"
#include "AXS1.h"

volatile bool clock2Expired = FALSE;
volatile bool clock3Expired = FALSE;
volatile bool clock4Expired = FALSE;

void TIM2_IRQHandler(void)
{

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		clock2Expired = TRUE;
	}

}

void TIM3_IRQHandler(void)
{

	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		clock3Expired = TRUE;
	}

}

void TIM4_IRQHandler(void)
{

}

void clocksInitialize()
{

	clock1Initialize();
	clock2Initialize();
	clock3Initialize();
	clock4Initialize();

}

void clock1Initialize()
{

}

void clock2Initialize()
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	NVIC_InitTypeDef NVICStruct;

	TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);

	TIM_DeInit(TIM2);
	TIM_TimeBaseStruct.TIM_Prescaler = CLOCK_2_PRESCALER;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStruct.TIM_Period = CLOCK_2_PERIOD;
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);

	NVICStruct.NVIC_IRQChannel = TIM2_IRQChannel;
	NVICStruct.NVIC_IRQChannelPreemptionPriority = CLOCK_2_PREEMPTION_PRIORITY;
	NVICStruct.NVIC_IRQChannelSubPriority = CLOCK_2_SUB_PRIORITY;
	NVICStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVICStruct);

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

}

void clock3Initialize()
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	NVIC_InitTypeDef NVICStruct;

	TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);

	TIM_DeInit(TIM3);
	TIM_TimeBaseStruct.TIM_Prescaler = CLOCK_3_PRESCALER;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStruct.TIM_Period = CLOCK_3_PERIOD;
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);

	NVICStruct.NVIC_IRQChannel = TIM3_IRQChannel;
	NVICStruct.NVIC_IRQChannelPreemptionPriority = CLOCK_3_PREEMPTION_PRIORITY;
	NVICStruct.NVIC_IRQChannelSubPriority = CLOCK_3_SUB_PRIORITY;
	NVICStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVICStruct);

	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

}

void clock4Initialize()
{

}

void clock2Start()
{

	TIM_SetCounter(TIM2, 0);
	TIM_Cmd(TIM2, ENABLE);

}

void clock2Stop()
{

	TIM_Cmd(TIM2, DISABLE);

}

bool clock2IsExpired()
{

	if(clock2Expired)
	{
		clock2Expired = FALSE;
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

void clock3Start()
{

	TIM_SetCounter(TIM3, 0);
	TIM_Cmd(TIM3, ENABLE);

}

void clock3Stop()
{

	TIM_Cmd(TIM3, DISABLE);

}

bool clock3IsExpired()
{

	if(clock3Expired)
	{
		clock3Expired = FALSE;
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

void clock4Start()
{

	TIM_SetCounter(TIM4, 0);
	TIM_Cmd(TIM4, ENABLE);

}

void clock4Stop()
{

	TIM_Cmd(TIM4, DISABLE);

}

bool clock4IsExpired()
{

	if(clock4Expired)
	{
		clock4Expired = FALSE;
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}
