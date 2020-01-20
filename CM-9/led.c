#include "led.h"

void LEDInitialize()
{

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void LEDSet(unsigned char LEDno, unsigned char state)
{

	uint16_t pin = GPIO_Pin_9;

	switch (state)
	{
		case LED_OFF:
			GPIOB->ODR |= pin;
			break;
		case LED_ON:
			GPIOB->ODR &= ~pin;
			break;
		case LED_TOGGLE:
			GPIOB->ODR ^= pin;
			break;
		default:
			break;
	}

}
