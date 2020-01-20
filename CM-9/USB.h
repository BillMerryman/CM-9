/*
 * USB.h
 *
 * Created: 11/17/2013 9:28:57 PM
 *  Author: Bill
 */ 


#ifndef USB_H_
#define USB_H_

#include "common.h"

#define USB_PREEMPTION_PRIORITY						3
#define USB_SUB_PRIORITY							3

#define USB_SERIAL_BAUD_RATE						115200
#define USB_SERIAL_STOP_BITS						0x00
#define USB_SERIAL_PARITY							0x00
#define USB_SERIAL_NUMBER_OF_BITS					0x08

#define USB_ERROR_NONE								0x00
#define USB_ERROR_BUFFER_EXHAUSTED					0x01
#define USB_ERROR_BAD_CHECKSUM						0x02
#define USB_REQUEST_FAILED							0xF0

#define USB_CLEAR_TRANSMIT_BUFFER					USBTxReadPosition=USBTxWritePosition=0
#define USB_CLEAR_RECEIVE_BUFFER					USBRxReadPosition=USBRxWritePosition=0
#define USB_CLEAR_ALL_BUFFERS						USB_CLEAR_TRANSMIT_BUFFER, USB_CLEAR_RECEIVE_BUFFER

#define BUFFER_SIZE									256

#define USB_PACKET_FLAG_WIDTH						2
#define USB_ID_WIDTH								1
#define USB_ID_POSITION								2
#define USB_PARAMETER_LENGTH_WIDTH					1
#define USB_PARAMETER_LENGTH_POSITION				3
#define USB_INSTRUCTION_WIDTH						1
#define USB_INSTRUCTION_POSITION					4
#define USB_ERROR_WIDTH								1
#define USB_ERROR_POSITION							4
#define USB_PARAMETER_START_POSITION				5
#define USB_CHECKSUM_WIDTH							1

void USB_LP_CAN_RX0_IRQHandler(void);
void USBHandleSOF(void);
void USBHandleEP1IN(void);
void USBHandleEP3OUT(void);

void USBInitialize(void);

void USBTxString(const char *message);
void USBTxPacket(uint8_t ID, uint8_t error, uint8_t *TxParameters, uint8_t TxParameterLength);
bool USBRxIsPacketReady(uint8_t *RxParametersLength);
void USBRxPacket(uint8_t *ID, uint8_t *instruction, uint8_t *parameters, uint8_t parametersLength);

uint8_t USBTxGetOccupiedMessageBuffer(void);
uint16_t USBTxGetAvailableMessageBuffer(void);
uint8_t USBRxGetOccupiedMessageBuffer(void);
uint16_t USBRxGetAvailableMessageBuffer(void);

#endif /* USB_H_ */
