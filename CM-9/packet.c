/*
 * packet.c
 *
 *  Created on: Jun 1, 2017
 *      Author: Bill
 */

#include "packet.h"

void assemblePacket(uint8_t *buffer, uint8_t *readPosition, uint8_t *writePosition, uint8_t ID, uint8_t instructionOrError, uint8_t *TxParameters, uint8_t TxParameterLength)
{

	while (getAvailableMessageBuffer(readPosition, writePosition) < (HEADER_WIDTH + INSTRUCTION_OR_ERROR_WIDTH + TxParameterLength + CHECKSUM_WIDTH + 1));

	buffer[*writePosition] = 0xFF;
	(*writePosition)++;
	buffer[*writePosition] = 0xFF;
	(*writePosition)++;
	buffer[*writePosition] = ID;
	(*writePosition)++;
	uint8_t payloadLength = ERROR_WIDTH + TxParameterLength + CHECKSUM_WIDTH;
	buffer[*writePosition] = payloadLength;
	(*writePosition)++;
	buffer[*writePosition] = instructionOrError;
	(*writePosition)++;

	uint8_t checkSum = ID + payloadLength + instructionOrError;

	for(uint8_t count = 0; count < TxParameterLength; count++)
	{
		buffer[*writePosition] = TxParameters[count];
		checkSum += TxParameters[count];
		(*writePosition)++;
	}

	buffer[*writePosition] = ~checkSum;
	(*writePosition)++;

}

bool isPacketReady(uint8_t *buffer, uint8_t *readPosition, uint8_t *writePosition, uint8_t *parametersLength)
{

	uint8_t occupiedBufferSize = getOccupiedMessageBuffer(readPosition, writePosition);

	//if we don't even have the sentinel yet, exit
	if(occupiedBufferSize < PACKET_FLAG_WIDTH) return FALSE;

	//if we have what should at least be a sentinel, but it isn't,
	//consume the buffer until we find one, or exit if it is exhausted
	while(buffer[*readPosition] != 0xFF || buffer[(uint8_t)((*readPosition) + 1)] != 0xFF)
	{
		(*readPosition)++;
		occupiedBufferSize = getOccupiedMessageBuffer(readPosition, writePosition);
		if(occupiedBufferSize < PACKET_FLAG_WIDTH) return FALSE;
	}

	//if we don't have a headers worth yet, exit
	if(occupiedBufferSize < HEADER_WIDTH) return FALSE;

	uint8_t parameterSizePosition = *readPosition + PACKET_FLAG_WIDTH + ID_WIDTH;
	uint8_t packetSize = HEADER_WIDTH + buffer[parameterSizePosition];

	//if we don't have what the header says is a packets worth, exit
	if(occupiedBufferSize < packetSize) return FALSE;

	uint8_t checkSumPosition = (*readPosition + packetSize) - CHECKSUM_WIDTH;
	uint8_t checkSum = 0;
	uint8_t tmpReadPosition = *readPosition + PACKET_FLAG_WIDTH;
	while(tmpReadPosition != checkSumPosition) checkSum += buffer[tmpReadPosition++];
	checkSum = ~checkSum;
	if(checkSum != buffer[tmpReadPosition++])
	{
		*readPosition = tmpReadPosition;
		return FALSE;
	}

	*parametersLength = buffer[parameterSizePosition] - (INSTRUCTION_OR_ERROR_WIDTH + CHECKSUM_WIDTH);
	return TRUE;

}

void disassemblePacket(uint8_t *buffer, uint8_t *readPosition, uint8_t *writePosition, uint8_t *ID, uint8_t *instructionOrError, uint8_t *parameters, uint8_t parametersLength)
{

	*readPosition += PACKET_FLAG_WIDTH;
	(*ID) = buffer[*readPosition];
	(*readPosition)++;
	uint8_t packetLength = buffer[*readPosition];
	(*readPosition)++;
	(*instructionOrError) = buffer[*readPosition];
	(*readPosition)++;

	uint8_t tmpParametersLength = packetLength - (INSTRUCTION_OR_ERROR_WIDTH + CHECKSUM_WIDTH);
	for(uint8_t parametersPosition = 0; parametersPosition < parametersLength; parametersPosition++)
	{
		parameters[parametersPosition] = buffer[*readPosition];
		(*readPosition)++;
	}
	(*readPosition)++;

}

uint8_t getOccupiedMessageBuffer(uint8_t *readPosition, uint8_t *writePosition)
{

	return (*readPosition < *writePosition) ? (*writePosition - *readPosition) : (BUFFER_SIZE - (*readPosition - *writePosition));

}

uint16_t getAvailableMessageBuffer(uint8_t *readPosition, uint8_t *writePosition)
{

	return BUFFER_SIZE - getOccupiedMessageBuffer(readPosition, writePosition);

}
