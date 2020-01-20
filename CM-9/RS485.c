/*
 * RS485.c
 *
 *  Created on: Dec 19, 2013
 *      Author: Bill
 */

#include <string.h>
#include <stdio.h>

#include "bluetooth.h"
#include "led.h"
#include "clock.h"
#include "dynamixels.h"
#include "protocol.h"
#include "RS485.h"

volatile uint8_t RS485RxReadPosition = 0;
volatile uint8_t RS485RxWritePosition = 0;
volatile uint8_t RS485TxReadPosition = 0;
volatile uint8_t RS485TxWritePosition = 0;
volatile uint8_t RS485TxBuffer[BUFFER_SIZE], RS485RxBuffer[BUFFER_SIZE];


void USART1_IRQHandler(void)
{

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{

		clock3Stop();
		RS485RxBuffer[RS485RxWritePosition] = USART_ReceiveData(USART1);
		RS485RxWritePosition++;
		clock3Start();

	}

	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{
		if(RS485TxReadPosition != RS485TxWritePosition)
		{
			USART_SendData(USART1, RS485TxBuffer[RS485TxReadPosition]);
			RS485TxReadPosition++;
		}
		else
		{
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
			GPIO_ResetBits(GPIOB, GPIO_Pin_5);	// RX enable
		}
	}

}

void RS485Initialize(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_StructInit(&GPIO_InitStruct);
	USART_StructInit(&USART_InitStruct);
	NVIC_StructInit(&NVIC_InitStructure);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);

	GPIO_SetBits(GPIOB, GPIO_Pin_5);

	USART_DeInit(USART1);

	USART_InitStruct.USART_BaudRate = RS485_BAUD_RATE_1M;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStruct);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RS485_PREEMPTION_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = RS485_SUB_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART1, ENABLE);

}

void RS485TxPacket(uint8_t ID, uint8_t instruction, uint8_t *TxParameters, uint8_t TxParameterLength)
{

	while(RS485TxGetAvailableMessageBuffer() < (RS485_PACKET_FLAG_WIDTH + RS485_ID_WIDTH + RS485_PARAMETER_LENGTH_WIDTH + RS485_INSTRUCTION_WIDTH + TxParameterLength + RS485_CHECKSUM_WIDTH + 1));

	RS485TxBuffer[RS485TxWritePosition] = 0xFF;
	RS485TxWritePosition++;
	RS485TxBuffer[RS485TxWritePosition] = 0xFF;
	RS485TxWritePosition++;
    RS485TxBuffer[RS485TxWritePosition] = ID;
    RS485TxWritePosition++;
	uint8_t payloadLength = RS485_INSTRUCTION_WIDTH + TxParameterLength + RS485_CHECKSUM_WIDTH;
    RS485TxBuffer[RS485TxWritePosition] = payloadLength;
    RS485TxWritePosition++;
    RS485TxBuffer[RS485TxWritePosition] = instruction;
    RS485TxWritePosition++;

    uint8_t checkSum = ID + payloadLength + instruction;

    for(uint8_t count = 0; count < TxParameterLength; count++)
    {
		RS485TxBuffer[RS485TxWritePosition] = TxParameters[count];
        checkSum += TxParameters[count];
        RS485TxWritePosition++;
    }

	RS485TxBuffer[RS485TxWritePosition] = ~checkSum;
	RS485TxWritePosition++;

	GPIO_SetBits(GPIOB, GPIO_Pin_5);

	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

}

bool RS485RxPacket(uint8_t bID, uint8_t *dynamixelError, uint8_t *bpRxParameters, uint8_t bRxParameterLength)
{

	//wait for it to switch to input
	while(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5));
	//start the input clock
	clock3Start();

	while(!clock3IsExpired())
	{
		uint8_t occupiedBufferSize = RS485RxGetOccupiedMessageBuffer();

		//if we don't even have a sentinels worth yet, continue
		if(occupiedBufferSize < RS485_PACKET_FLAG_WIDTH) continue;

		//if we have what should at least be a sentinel, but it isn't,
		//consume a byte and try again...
		if(RS485RxBuffer[RS485RxReadPosition] != 0xFF || RS485RxBuffer[(uint8_t)(RS485RxReadPosition + 1)] != 0xFF)
		{
			RS485RxReadPosition++;
			continue;
		}

		//if we don't have a headers worth yet, continue
		if(occupiedBufferSize < RS485_PACKET_FLAG_WIDTH + RS485_ID_WIDTH + RS485_PARAMETER_LENGTH_WIDTH) continue;

		uint8_t parameterSizePosition = RS485RxReadPosition + RS485_PACKET_FLAG_WIDTH + RS485_ID_WIDTH;
		uint8_t packetSize = (RS485_PACKET_FLAG_WIDTH + RS485_ID_WIDTH + RS485_PARAMETER_LENGTH_WIDTH) + RS485RxBuffer[parameterSizePosition];

		//if we don't have what the header says is a packets worth yet, continue
		if(occupiedBufferSize < packetSize) continue;

		//we have a packets worth, evaluate checksum to make sure it is a valid packet
		uint8_t checkSumPosition = (RS485RxReadPosition + packetSize) - RS485_CHECKSUM_WIDTH;
		uint8_t checkSum = 0;
		uint8_t rxReadPosition = RS485RxReadPosition + RS485_PACKET_FLAG_WIDTH;
		while(rxReadPosition != checkSumPosition) checkSum += RS485RxBuffer[rxReadPosition++];
		checkSum = ~checkSum;
		if(checkSum != RS485RxBuffer[rxReadPosition])
		{
			RS485RxReadPosition++;
			continue;
		}
		//we got a good packet back, so lets deal with it..
		clock3Stop();
		//move past header
		RS485RxReadPosition += RS485_PACKET_FLAG_WIDTH;
		//get returned ID
		uint8_t returnedID = RS485RxBuffer[RS485RxReadPosition];
		RS485RxReadPosition++;
		//get returned length
		uint8_t returnedLength = RS485RxBuffer[RS485RxReadPosition];
		RS485RxReadPosition++;
		//get returned error, if there is an error, eat rest of packet and return false
		*dynamixelError = RS485RxBuffer[RS485RxReadPosition];
		if(*dynamixelError & (RS485_RX_CHECKSUM_ERROR | RS485_RX_INSTRUCTION_ERROR))
		{
			RS485RxReadPosition += returnedLength;
			return FALSE;
		}
		RS485RxReadPosition++;
		//get payload length
		uint8_t payloadLength = returnedLength - (RS485_INSTRUCTION_WIDTH + RS485_CHECKSUM_WIDTH);
		//read payload
		for(uint8_t counter = 0; counter < payloadLength; counter++)
		{
			bpRxParameters[counter] = RS485RxBuffer[RS485RxReadPosition];
			RS485RxReadPosition++;
		}
		//eat checksum
		RS485RxReadPosition++;
		return TRUE;
	}

	RS485_CLEAR_RECEIVE_BUFFER;

	clock3Stop();
	return FALSE;

}

uint8_t RS485TxGetOccupiedMessageBuffer(void)
{

	return (RS485TxReadPosition < RS485TxWritePosition) ? (RS485TxWritePosition - RS485TxReadPosition) : (BUFFER_SIZE - (RS485TxReadPosition - RS485TxWritePosition));

}

uint16_t RS485TxGetAvailableMessageBuffer(void)
{

	return BUFFER_SIZE - RS485TxGetOccupiedMessageBuffer();

}

uint8_t RS485RxGetOccupiedMessageBuffer(void)
{

	return (RS485RxReadPosition < RS485RxWritePosition) ? (RS485RxWritePosition - RS485RxReadPosition) : (BUFFER_SIZE - (RS485RxReadPosition - RS485RxWritePosition));

}

uint16_t RS485RxGetAvailableMessageBuffer(void)
{

	return BUFFER_SIZE - RS485RxGetOccupiedMessageBuffer();

}
