/*
 * RS485.h
 *
 *  Created on: Dec 19, 2013
 *      Author: Bill
 */

#ifndef RS485_H_
#define RS485_H_

#include "common.h"

#define RS485_PREEMPTION_PRIORITY					0
#define RS485_SUB_PRIORITY							0
#define RS485_BAUD_RATE_1M							(u32)1000000

#define RS485_CLEAR_TRANSMIT_BUFFER					RS485TxReadPosition = RS485TxWritePosition = 0
#define RS485_CLEAR_RECEIVE_BUFFER					RS485RxReadPosition = RS485RxWritePosition = 0
#define RS485_CLEAR_ALL_BUFFERS						RS485_CLEAR_TRANSMIT_BUFFER, RS485_CLEAR_RECEIVE_BUFFER

#define RS485_PACKET_FLAG_WIDTH						2
#define RS485_ID_WIDTH								1
#define RS485_ID_POSITION							2
#define RS485_PARAMETER_LENGTH_WIDTH				1
#define RS485_PARAMETER_LENGTH_POSITION				3
#define RS485_INSTRUCTION_WIDTH						1
#define RS485_INSTRUCTION_POSITION					4
#define RS485_ERROR_WIDTH							1
#define RS485_ERROR_POSITION						4
#define RS485_PARAMETER_START_POSITION				5
#define RS485_CHECKSUM_WIDTH						1
#define RS485_TX_HEADER_WIDTH						(RS485_PACKET_FLAG_WIDTH + RS485_ID_WIDTH + RS485_PARAMETER_LENGTH_WIDTH + RS485_INSTRUCTION_WIDTH)
#define RS485_RX_HEADER_WIDTH						(RS485_PACKET_FLAG_WIDTH + RS485_ID_WIDTH + RS485_PARAMETER_LENGTH_WIDTH + RS485_ERROR_WIDTH)
#define RS485_TX_NON_PARAMETER_WIDTH				(RS485_TX_HEADER_WIDTH + RS485_CHECKSUM_WIDTH)
#define RS485_RX_NON_PARAMETER_WIDTH				(RS485_RX_HEADER_WIDTH + RS485_CHECKSUM_WIDTH)

#define RS485_TX_PARAMETERS_READ_START_POSITION		0
#define RS485_TX_PARAMETERS_READ_START_WIDTH		1
#define RS485_TX_PARAMETERS_READ_LENGTH_POSITION	1
#define RS485_TX_PARAMETERS_READ_LENGTH_WIDTH		1

#define	RS485_RX_INPUT_VOLTAGE_ERROR				(1<<0)
#define	RS485_RX_ANGLE_LIMIT_ERROR					(1<<1)
#define	RS485_RX_OVERHEATING_ERROR					(1<<2)
#define	RS485_RX_RANGE_ERROR						(1<<3)
#define	RS485_RX_CHECKSUM_ERROR						(1<<4)
#define	RS485_RX_OVERLOAD_ERROR						(1<<5)
#define	RS485_RX_INSTRUCTION_ERROR					(1<<6)
#define RS485_RX_GENERAL_ERROR						(1<<7)

#define BUFFER_SIZE									256

void RS485Initialize(void);
void RS485TxPacket(uint8_t bID, uint8_t bInstruction, uint8_t *bpTxParameters, uint8_t bTxParameterLength);
bool RS485RxPacket(uint8_t bID, uint8_t *error, uint8_t *bpRxParameters, uint8_t bRxParameterLength);

uint8_t RS485TxGetOccupiedMessageBuffer(void);
uint16_t RS485TxGetAvailableMessageBuffer(void);
uint8_t RS485RxGetOccupiedMessageBuffer(void);
uint16_t RS485RxGetAvailableMessageBuffer(void);

#endif /* RS485_H_ */
