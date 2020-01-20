/*
 * packet.h
 *
 *  Created on: Jun 1, 2017
 *      Author: Bill
 */

#ifndef PACKET_H_
#define PACKET_H_

#define BUFFER_SIZE									256

#define PACKET_FLAG_WIDTH							2
#define ID_WIDTH									1
#define ID_POSITION									PACKET_FLAG_WIDTH
#define PAYLOAD_LENGTH_WIDTH						1
#define PAYLOAD_LENGTH_POSITION						ID_POSITION + ID_WIDTH
#define INSTRUCTION_OR_ERROR_WIDTH					1
#define INSTRUCTION_OR_ERROR_POSITION				PAYLOAD_LENGTH_POSITION + PAYLOAD_LENGTH_WIDTH
#define ERROR_WIDTH									1
#define ERROR_POSITION								PAYLOAD_LENGTH_POSITION + PAYLOAD_LENGTH_WIDTH
#define PARAMETER_START_POSITION					INSTRUCTION_OR_ERROR_POSITION + INSTRUCTION_OR_ERROR_WIDTH
#define CHECKSUM_WIDTH								1
#define HEADER_WIDTH								PACKET_FLAG_WIDTH + ID_WIDTH + PAYLOAD_LENGTH_WIDTH
#define NON_PARAMETER_WIDTH							HEADER_WIDTH + INSTRUCTION_OR_ERROR_WIDTH + CHECKSUM_WIDTH

void assemblePacket(uint8_t *buffer, uint8_t *readPosition, uint8_t *writePosition, uint8_t ID, uint8_t instructionOrError, uint8_t *TxParameters, uint8_t TxParameterLength);
bool isPacketReady(uint8_t *buffer, uint8_t *readPosition, uint8_t *writePosition, uint8_t *parametersLength);
void disassemblePacket(uint8_t *buffer, uint8_t *readPosition, uint8_t *writePosition, uint8_t *ID, uint8_t *instructionOrError, uint8_t *parameters, uint8_t parametersLength);
uint8_t getOccupiedMessageBuffer(uint8_t *readPosition, uint8_t *writePosition);
uint16_t getAvailableMessageBuffer(uint8_t *readPosition, uint8_t *writePosition);

#endif /* PACKET_H_ */
