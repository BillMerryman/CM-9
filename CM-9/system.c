/*
 * system.c
 *
 * Created: 11/19/2013 5:20:27 PM
 *  Author: Bill
 */ 

#include "system.h"

void systemInitialize()
{

	#ifdef  VECT_TAB_RAM
	// Set the Vector Table base location at 0x20000000
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
	#else  // VECT_TAB_FLASH
	// Set the Vector Table base location at 0x08003000
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);
	//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	#endif

	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);

	while(RCC_WaitForHSEStartUp() != SUCCESS); //halt here if external oscillator doesn't start/stabilize

	//for some reason we still need a large delay here or UART1 will not work...
	for(volatile u32 delay = 0; delay < 100000; delay++)
	{
		asm("nop");
	}

	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

	FLASH_SetLatency(FLASH_Latency_2);

	//set hclk to sysclk speed
	RCC_HCLKConfig(RCC_SYSCLK_Div1);

	//set apb2 clock to hclk speed
	RCC_PCLK2Config(RCC_HCLK_Div1);

	//configure apb1 clock to half of system clock
	RCC_PCLK1Config(RCC_HCLK_Div2);

	RCC_ADCCLKConfig(RCC_PCLK2_Div4);

	//configure the phase lock loop to the external clock * 9
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

	//enable the phase lock loop
	RCC_PLLCmd(ENABLE);

	//wait til the phase lock loop is ready
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

	//make the phase lock loop the system clock
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	//wait until the phase lock loop is stabilized as the system clock
	while(RCC_GetSYSCLKSource() != 0x08);

	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 | RCC_APB1Periph_UART5, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

}
