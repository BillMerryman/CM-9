/*
 * bluetooth.c
 *
 * Created: 11/17/2013 9:28:38 PM
 *  Author: Bill
 */
#include "led.h"
#include "bluetooth.h"
#include <string.h>

volatile uint8_t bluetoothRxReadPosition = 0;
volatile uint8_t bluetoothRxWritePosition = 0;
volatile uint8_t bluetoothRxCurrentlyBuildingMessageStart = 0;
volatile uint8_t bluetoothTxReadPosition = 0;
uint8_t bluetoothTxWritePosition = 0;
volatile uint8_t bluetoothTxBuffer[BUFFER_SIZE], bluetoothRxBuffer[BUFFER_SIZE];
volatile uint8_t bluetoothMessagesReady = 0;
volatile uint8_t bluetoothError = BLUETOOTH_ERROR_NONE;

void USART2_IRQHandler(void)
{

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{

		bluetoothRxBuffer[bluetoothRxWritePosition++] = USART_ReceiveData(USART2);

		uint8_t occupiedBufferSize = bluetoothRxGetOccupiedMessageBuffer();

		//if we don't even have the sentinel yet, exit
		if(occupiedBufferSize < BLUETOOTH_PACKET_FLAG_WIDTH) return;

		//if we have what is supposed to be a sentinel, but it isn't,
		//move forward one and exit
		if(bluetoothRxBuffer[bluetoothRxReadPosition] != 0xFF || bluetoothRxBuffer[(uint8_t)(bluetoothRxReadPosition + 1)] != 0xFF)
		{
			bluetoothRxReadPosition++;
			return;
		}

		//if we don't have a headers worth yet, exit
		if(occupiedBufferSize < BLUETOOTH_PACKET_FLAG_WIDTH + BLUETOOTH_ID_WIDTH + BLUETOOTH_PARAMETER_LENGTH_WIDTH) return;

		uint8_t parameterSizePosition = bluetoothRxReadPosition + BLUETOOTH_PACKET_FLAG_WIDTH + BLUETOOTH_ID_WIDTH;
		uint16_t packetSize = (BLUETOOTH_PACKET_FLAG_WIDTH + BLUETOOTH_ID_WIDTH + BLUETOOTH_PARAMETER_LENGTH_WIDTH) + bluetoothRxBuffer[parameterSizePosition];

		//if we don't have what the header says is a packets worth, exit
		if(occupiedBufferSize < packetSize) return;

		uint8_t checkSumPosition = (bluetoothRxReadPosition + packetSize) - BLUETOOTH_CHECKSUM_WIDTH;
		uint8_t checkSum = 0;
		uint8_t rxReadPosition = bluetoothRxReadPosition + BLUETOOTH_PACKET_FLAG_WIDTH;
		while(rxReadPosition != checkSumPosition) checkSum += bluetoothRxBuffer[rxReadPosition++];
		checkSum = ~checkSum;
		if(checkSum == bluetoothRxBuffer[rxReadPosition])
		{
			bluetoothError = BLUETOOTH_ERROR_NONE;
			bluetoothMessagesReady++;
			return;
		}

		//we had a bad packet, but maybe it was incomplete and another packet was
		//started. See if there is another header in the remainder.
		bluetoothError = BLUETOOTH_ERROR_BAD_CHECKSUM;
		while(++bluetoothRxReadPosition != bluetoothRxWritePosition)
		{
			if(bluetoothRxBuffer[bluetoothRxReadPosition] == 0xFF && bluetoothRxBuffer[(uint8_t)(bluetoothRxReadPosition + 1)] == 0xFF) return;
		}

	}

	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
	{

		if(bluetoothTxReadPosition != bluetoothTxWritePosition)
		{
			USART_SendData(USART2, bluetoothTxBuffer[(bluetoothTxReadPosition++)]);
		}
		else
		{
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		}

	}

}

void bluetoothInitialize(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_StructInit(&GPIO_InitStruct);
	USART_StructInit(&USART_InitStruct);
	NVIC_StructInit(&NVIC_InitStructure);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	USART_DeInit(USART2);

	USART_InitStruct.USART_BaudRate = BLUETOOTH_BAUD_RATE_57600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART2, &USART_InitStruct);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BLUETOOTH_PREEMPTION_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = BLUETOOTH_SUB_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART2, ENABLE);

}

void bluetoothTxString(const char *message)
{

	uint16_t messageLength = strlen(message);

	while(bluetoothTxGetAvailableMessageBuffer() < messageLength + 1);

	for(uint16_t count = 0; count < messageLength; count++)
	{
		bluetoothTxBuffer[bluetoothTxWritePosition++] = message[count];
	}

    if (bluetoothTxReadPosition != bluetoothTxWritePosition) USART_ITConfig(USART2, USART_IT_TXE, ENABLE); // USART2->SR |= USART_IT_TC;

}

void bluetoothTxPacket(uint8_t ID, uint8_t error, uint8_t *TxParameters, uint8_t TxParameterLength)
{

	while(bluetoothTxGetAvailableMessageBuffer() < (BLUETOOTH_PACKET_FLAG_WIDTH + BLUETOOTH_ID_WIDTH + BLUETOOTH_PARAMETER_LENGTH_WIDTH + BLUETOOTH_INSTRUCTION_WIDTH + TxParameterLength + BLUETOOTH_CHECKSUM_WIDTH + 1));

	bluetoothTxBuffer[bluetoothTxWritePosition] = 0xFF;
	bluetoothTxWritePosition++;
	bluetoothTxBuffer[bluetoothTxWritePosition] = 0xFF;
	bluetoothTxWritePosition++;
	bluetoothTxBuffer[bluetoothTxWritePosition] = ID;
	bluetoothTxWritePosition++;
	uint8_t payloadLength = BLUETOOTH_ERROR_WIDTH + TxParameterLength + BLUETOOTH_CHECKSUM_WIDTH;
	bluetoothTxBuffer[bluetoothTxWritePosition] = payloadLength;
	bluetoothTxWritePosition++;
	bluetoothTxBuffer[bluetoothTxWritePosition] = error;
	bluetoothTxWritePosition++;

	uint8_t checkSum = ID + payloadLength + error;

    for(uint8_t count = 0; count < TxParameterLength; count++)
    {
		bluetoothTxBuffer[bluetoothTxWritePosition] = TxParameters[count];
        checkSum += TxParameters[count];
		bluetoothTxWritePosition++;
    }

	bluetoothTxBuffer[bluetoothTxWritePosition] = ~checkSum;
	bluetoothTxWritePosition++;

	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);

}

void bluetoothRxPacket(uint8_t *ID, uint8_t *instruction, uint8_t *parameters, uint8_t parametersLength)
{

	bluetoothRxReadPosition += BLUETOOTH_PACKET_FLAG_WIDTH;
	(*ID) = bluetoothRxBuffer[bluetoothRxReadPosition++];
	bluetoothRxReadPosition += BLUETOOTH_PARAMETER_LENGTH_WIDTH;
	(*instruction) = bluetoothRxBuffer[bluetoothRxReadPosition++];

	for(uint8_t parametersPosition = 0; parametersPosition < parametersLength; parametersPosition++)
	{
		parameters[parametersPosition] = bluetoothRxBuffer[bluetoothRxReadPosition++];
	}
	bluetoothRxReadPosition++;

	bluetoothMessagesReady--;

}

uint8_t bluetoothRxPacketsReady(uint8_t *commandLength)
{

	if(bluetoothMessagesReady > 0)
	{
		uint8_t messageStartPosition = bluetoothRxReadPosition + BLUETOOTH_PACKET_FLAG_WIDTH + BLUETOOTH_ID_WIDTH;
		*commandLength = bluetoothRxBuffer[messageStartPosition] - (BLUETOOTH_INSTRUCTION_WIDTH + BLUETOOTH_CHECKSUM_WIDTH);
	}

	return bluetoothMessagesReady;

}

uint8_t bluetoothGetError(void)
{

	uint8_t error = bluetoothError;
	bluetoothError = BLUETOOTH_ERROR_NONE;
	return error;

}

uint8_t bluetoothTxGetOccupiedMessageBuffer(void)
{

	return (bluetoothTxReadPosition < bluetoothTxWritePosition) ? (bluetoothTxWritePosition - bluetoothTxReadPosition) : (BUFFER_SIZE - (bluetoothTxReadPosition - bluetoothTxWritePosition));

}

uint16_t bluetoothTxGetAvailableMessageBuffer(void)
{

	return BUFFER_SIZE - bluetoothTxGetOccupiedMessageBuffer();

}

uint8_t bluetoothRxGetOccupiedMessageBuffer(void)
{

	return (bluetoothRxReadPosition < bluetoothRxWritePosition) ? (bluetoothRxWritePosition - bluetoothRxReadPosition) : (BUFFER_SIZE - (bluetoothRxReadPosition - bluetoothRxWritePosition));

}

uint16_t bluetoothRxGetAvailableMessageBuffer(void)
{

	return BUFFER_SIZE - bluetoothRxGetOccupiedMessageBuffer();

}
