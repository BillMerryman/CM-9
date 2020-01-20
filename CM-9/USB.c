/*
 * USB.c
 *
 * Created: 11/17/2013 9:28:38 PM
 *  Author: Bill
 */ 

#include <string.h>
#include <stdio.h>

#include "led.h"
#include "USB.h"
#include "usb_istr.h"
#include "usb_lib.h"
#include "usb_hw_config.h"
#include "usb_pwr.h"
#include "usb_desc.h"

volatile uint8_t USBRxReadPosition = 0;
volatile uint8_t USBRxWritePosition = 0;
volatile uint8_t USBTxReadPosition = 0;
volatile uint8_t USBTxWritePosition = 0;
volatile uint8_t USBTxBuffer[BUFFER_SIZE], USBRxBuffer[BUFFER_SIZE];

volatile uint8_t USBError = USB_ERROR_NONE;
volatile uint8_t USBSending = 0;

void USB_LP_CAN_RX0_IRQHandler(void)
{
	USB_Istr();
}

void USBHandleSOF(void)
{

	uint8_t TxBuffer[VIRTUAL_COM_PORT_DATA_SIZE];
	uint8_t TxCount = 0;

	if(bDeviceState == CONFIGURED)
	{
		if(USBSending == 0)
		{
			do
			{
				if(USBTxReadPosition == USBTxWritePosition) break;
				TxBuffer[TxCount++] = USBTxBuffer[USBTxReadPosition++];
			}
			while(TxCount < VIRTUAL_COM_PORT_DATA_SIZE);

			if(TxCount > 0)
			{
				USBSending = 1;
				UserToPMABufferCopy((unsigned char*)TxBuffer, ENDP1_TXADDR, TxCount);
				SetEPTxCount(ENDP1, TxCount);
				SetEPTxValid(ENDP1);
			}
		}
	}

}

void USBHandleEP1IN(void)
{

	uint8_t TxBuffer[VIRTUAL_COM_PORT_DATA_SIZE];
	uint8_t TxCount = 0;

	do
	{
		if(USBTxReadPosition == USBTxWritePosition) break;
		TxBuffer[TxCount++] = USBTxBuffer[USBTxReadPosition++];
	}
	while(TxCount < VIRTUAL_COM_PORT_DATA_SIZE);

	if(TxCount > 0)
	{
		UserToPMABufferCopy((unsigned char*)TxBuffer, ENDP1_TXADDR, TxCount);
		SetEPTxCount(ENDP1, TxCount);
		SetEPTxValid(ENDP1);
	}
	else
	{
		USBSending = 0;
	}

}

void USBHandleEP3OUT(void)
{

	uint8_t rxBuffer[VIRTUAL_COM_PORT_DATA_SIZE];
	uint8_t rxDataLength = GetEPRxCount(ENDP3);

	PMAToUserBufferCopy(rxBuffer, ENDP3_RXADDR, rxDataLength);
	SetEPRxValid(ENDP3);

	if(USBRxGetAvailableMessageBuffer() < rxDataLength)
	{
		USBError = USB_ERROR_BUFFER_EXHAUSTED;
		return;
	}

	for(uint8_t counter = 0; counter < rxDataLength; counter++)
	{
		USBRxBuffer[USBRxWritePosition] = rxBuffer[counter];
		USBRxWritePosition++;
	}

}

void USBInitialize(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_StructInit(&GPIO_InitStruct);
	NVIC_StructInit(&NVIC_InitStructure);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_ResetBits(GPIOC, GPIO_Pin_13);

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN_RX0_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USB_PREEMPTION_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USB_SUB_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//Call the library USB init function
	USB_Init();

}

void USBTxString(const char *message)
{

	uint16_t messageLength = strlen(message);

	if(messageLength==0) return;

	while(USBTxGetAvailableMessageBuffer() < messageLength + 1);

	for(uint16_t count = 0; count < messageLength; count++)
	{
		USBTxBuffer[USBTxWritePosition] = message[count];
		USBTxWritePosition++; //can't increment in previous statement. Compiler creates bug
	}

}

void USBTxPacket(uint8_t ID, uint8_t error, uint8_t *TxParameters, uint8_t TxParameterLength)
{

	while(USBTxGetAvailableMessageBuffer() < (USB_PACKET_FLAG_WIDTH + USB_ID_WIDTH + USB_PARAMETER_LENGTH_WIDTH + USB_INSTRUCTION_WIDTH + TxParameterLength + USB_CHECKSUM_WIDTH + 1));

	USBTxBuffer[USBTxWritePosition] = 0xFF;
	USBTxWritePosition++;
	USBTxBuffer[USBTxWritePosition] = 0xFF;
	USBTxWritePosition++;
	USBTxBuffer[USBTxWritePosition] = ID;
	USBTxWritePosition++;
	uint8_t payloadLength = USB_ERROR_WIDTH + TxParameterLength + USB_CHECKSUM_WIDTH;
	USBTxBuffer[USBTxWritePosition] = payloadLength;
	USBTxWritePosition++;
	USBTxBuffer[USBTxWritePosition] = error;
	USBTxWritePosition++;

	uint8_t checkSum = ID + payloadLength + error;

	for(uint8_t count = 0; count < TxParameterLength; count++)
	{
		USBTxBuffer[USBTxWritePosition] = TxParameters[count];
		checkSum += TxParameters[count];
		USBTxWritePosition++;
	}

	USBTxBuffer[USBTxWritePosition] = ~checkSum;
	USBTxWritePosition++;

}

bool USBRxIsPacketReady(uint8_t *RxParametersLength)
{

	uint8_t occupiedBufferSize = USBRxGetOccupiedMessageBuffer();

	//if we don't even have the sentinel yet, exit
	if(occupiedBufferSize < USB_PACKET_FLAG_WIDTH) return FALSE;

	//if we have what should at least be a sentinel, but it isn't,
	//consume the buffer until we find one, or exit if it is exhausted
	while(USBRxBuffer[USBRxReadPosition] != 0xFF || USBRxBuffer[(uint8_t)(USBRxReadPosition + 1)] != 0xFF)
	{
		USBRxReadPosition++;
		occupiedBufferSize = USBRxGetOccupiedMessageBuffer();
		if(occupiedBufferSize < USB_PACKET_FLAG_WIDTH) return FALSE;
	}

	//if we don't have a headers worth yet, exit
	if(occupiedBufferSize < USB_PACKET_FLAG_WIDTH + USB_ID_WIDTH + USB_PARAMETER_LENGTH_WIDTH) return FALSE;

	uint8_t parameterSizePosition = USBRxReadPosition + USB_PACKET_FLAG_WIDTH + USB_ID_WIDTH;
	uint8_t packetSize = (USB_PACKET_FLAG_WIDTH + USB_ID_WIDTH + USB_PARAMETER_LENGTH_WIDTH) + USBRxBuffer[parameterSizePosition];

	//if we don't have what the header says is a packets worth, exit
	if(occupiedBufferSize < packetSize) return FALSE;

	uint8_t checkSumPosition = (USBRxReadPosition + packetSize) - USB_CHECKSUM_WIDTH;
	uint8_t checkSum = 0;
	uint8_t rxReadPosition = USBRxReadPosition + USB_PACKET_FLAG_WIDTH;
	while(rxReadPosition != checkSumPosition) checkSum += USBRxBuffer[rxReadPosition++];
	checkSum = ~checkSum;
	if(checkSum != USBRxBuffer[rxReadPosition++])
	{
		USBRxReadPosition = rxReadPosition;
		return FALSE;
	}

	*RxParametersLength = USBRxBuffer[parameterSizePosition] - (USB_INSTRUCTION_WIDTH + USB_CHECKSUM_WIDTH);
	return TRUE;

}

void USBRxPacket(uint8_t *ID, uint8_t *instruction, uint8_t *parameters, uint8_t parametersLength)
{

	USBRxReadPosition += USB_PACKET_FLAG_WIDTH;
	(*ID) = USBRxBuffer[USBRxReadPosition];
	USBRxReadPosition++;
	USBRxReadPosition += USB_PARAMETER_LENGTH_WIDTH;
	(*instruction) = USBRxBuffer[USBRxReadPosition];
	USBRxReadPosition++;

	for(uint8_t parametersPosition = 0; parametersPosition < parametersLength; parametersPosition++)
	{
		parameters[parametersPosition] = USBRxBuffer[USBRxReadPosition];
		USBRxReadPosition++;
	}
	USBRxReadPosition++;

}

uint8_t USBTxGetOccupiedMessageBuffer(void)
{

	return (USBTxReadPosition < USBTxWritePosition) ? (USBTxWritePosition - USBTxReadPosition) : (BUFFER_SIZE - (USBTxReadPosition - USBTxWritePosition));

}

uint16_t USBTxGetAvailableMessageBuffer(void)
{

	return BUFFER_SIZE - USBTxGetOccupiedMessageBuffer();

}

uint8_t USBRxGetOccupiedMessageBuffer(void)
{

	return (USBRxReadPosition < USBRxWritePosition) ? (USBRxWritePosition - USBRxReadPosition) : (BUFFER_SIZE - (USBRxReadPosition - USBRxWritePosition));

}

uint16_t USBRxGetAvailableMessageBuffer(void)
{

	return BUFFER_SIZE - USBRxGetOccupiedMessageBuffer();

}
