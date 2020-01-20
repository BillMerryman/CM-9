/*
 * bluetooth.h
 *
 * Created: 11/17/2013 9:28:57 PM
 *  Author: Bill
 */


#ifndef bluetooth_H_
#define bluetooth_H_

#include "common.h"

#define BLUETOOTH_PREEMPTION_PRIORITY					3
#define BLUETOOTH_SUB_PRIORITY							3
#define BLUETOOTH_BAUD_RATE_57600						57600

//--- Host Instruction ---
#define BLUETOOTH_INST_PING								0x01
#define BLUETOOTH_INST_READ_DATA						0x02
#define BLUETOOTH_INST_WRITE_DATA						0x03
#define BLUETOOTH_INST_REG_WRITE						0x04
#define BLUETOOTH_INST_ACTION							0x05
#define BLUETOOTH_INST_RESET							0x06
#define BLUETOOTH_INST_SYNC_WRITE						0x83
#define BLUETOOTH_INST_SEND_ERROR						0xC0
#define BLUETOOTH_INST_SEND_STRING						0xC1
#define BLUETOOTH_INST_CHANGE_MODE						0xC2
#define BLUETOOTH_INST_PERFORM_PAGE						0xC3
#define BLUETOOTH_INST_PERFORM_PAGE_POSE				0xC4
#define BLUETOOTH_INST_SET_PAGE_SECTION					0xC5
#define BLUETOOTH_INST_WRITE_PAGE						0xC6
#define BLUETOOTH_INST_QUERY_AVAILABLE_MESSAGE_SPACE	0xC7
#define BLUETOOTH_INST_RESPOND_AVAILABLE_MESSAGE_SPACE	0xC8

#define BLUETOOTH_ERROR_NONE							0x00
#define BLUETOOTH_ERROR_BUFFER_EXHAUSTED				0x01
#define BLUETOOTH_ERROR_BAD_CHECKSUM					0x02

#define BLUETOOTH_CLEAR_TRANSMIT_BUFFER					BluetoothTxReadPosition=BluetoothTxWritePosition=0
#define BLUETOOTH_CLEAR_RECEIVE_BUFFER					BluetoothRxReadPosition=BluetoothRxWritePosition=0
#define BLUETOOTH_CLEAR_ALL_BUFFERS						BLUETOOTH_CLEAR_TRANSMIT_BUFFER, BLUETOOTH_CLEAR_RECEIVE_BUFFER

#define BUFFER_SIZE										256

#define BLUETOOTH_PACKET_FLAG_WIDTH						2
#define BLUETOOTH_ID_WIDTH								1
#define BLUETOOTH_ID_POSITION							2
#define BLUETOOTH_PARAMETER_LENGTH_WIDTH				1
#define BLUETOOTH_PARAMETER_LENGTH_POSITION				3
#define BLUETOOTH_INSTRUCTION_WIDTH						1
#define BLUETOOTH_INSTRUCTION_POSITION					4
#define BLUETOOTH_ERROR_WIDTH							1
#define BLUETOOTH_ERROR_POSITION						4
#define BLUETOOTH_PARAMETER_START_POSITION				5
#define BLUETOOTH_CHECKSUM_WIDTH						1

void USART2_IRQHandler(void);
void bluetoothInitialize(void);

void bluetoothTxString(const char *message);
void bluetoothTxPacket(uint8_t ID, uint8_t error, uint8_t *TxParameters, uint8_t TxParameterLength);
void bluetoothRxPacket(uint8_t *ID, uint8_t *instruction, uint8_t *parameters, uint8_t parametersLength);
uint8_t bluetoothRxPacketsReady(uint8_t *commandLength);
uint8_t bluetoothGetError(void);

uint8_t bluetoothTxGetOccupiedMessageBuffer(void);
uint16_t bluetoothTxGetAvailableMessageBuffer(void);
uint8_t bluetoothRxGetOccupiedMessageBuffer(void);
uint16_t bluetoothRxGetAvailableMessageBuffer(void);

#endif /* bluetooth_H_ */
