/*
 * AX12.c
 *
 *  Created on: Dec 22, 2013
 *      Author: Bill
 */

#include <stdio.h>
#include <string.h>

#include "led.h"
#include "bluetooth.h"
#include "protocol.h"
#include "RS485.h"
#include "dynamixels.h"
#include "AX12.h"

AX12 AX12s[AX12_NUM_ATTACHED];
uint8_t AX12Count;

// Initialize device representations in memory

void AX12sInitialize(void)
{

	#ifdef DYNAMIXEL_DEBUG
		bluetoothTxString("Start Initializing AX12s\r\n");
	#endif

	uint8_t errorNo;
	AX12Count = 0;

	for(uint8_t ID = 0; ID < DYNAMIXEL_BROADCASTING_ID; ID++){
		if(dynamixelsPing(ID, &errorNo)){
			if(dynamixelsIsType(ID, AX12_MODEL_NUMBER)){
				AX12s[AX12Count].ID = ID;
				AX12GetInfo(ID, &AX12s[AX12Count], AX12_FIRST_REGISTER_REPRESENTED_IN_STRUCTURE, AX12_LAST_REGISTER_REPRESENTED_IN_STRUCTURE);

				#ifdef DYNAMIXEL_DEBUG
					AX12DisplayStats(&AX12s[AX12Count]);
				#endif

				AX12Count++;
			}
		}
	}

	#ifdef DYNAMIXEL_DEBUG
	{
		char outputMessage[32];
		sprintf(outputMessage, "AX12 Count: %u\r\n", AX12Count);
		bluetoothTxString(outputMessage);
		bluetoothTxString("End Initializing AX12s\r\n");
	}
	#endif

}

// Local getters and setters

uint8_t AX12sGetCount(void)
{

	return AX12Count;

}

uint8_t AX12GetID(uint8_t slot)
{

	return AX12s[slot].ID;

}

uint8_t AX12GetTorqueEnable(uint8_t slot)
{

	return AX12s[slot].torqueEnable;

}

void AX12SetTorqueEnable(uint8_t slot, uint8_t enable)
{

	AX12s[slot].torqueEnable = enable;

}

uint8_t AX12GetLED(uint8_t slot)
{

	return AX12s[slot].LED;

}

void AX12SetLED(uint8_t slot, uint8_t value)
{

	AX12s[slot].LED = value;

}

uint8_t AX12GetCWComplianceMargin(uint8_t slot)
{

	return AX12s[slot].cwComplianceMargin;

}

void AX12SetCWComplianceMargin(uint8_t slot, uint8_t margin)
{

	AX12s[slot].cwComplianceMargin = margin;

}

uint8_t AX12GetCCWComplianceMargin(uint8_t slot)
{

	return AX12s[slot].ccwComplianceMargin;

}

void AX12SetCCWComplianceMargin(uint8_t slot, uint8_t margin)
{

	AX12s[slot].ccwComplianceMargin = margin;

}

uint8_t AX12GetCWComplianceSlope(uint8_t slot)
{

	return AX12s[slot].cwComplianceSlope;

}

void AX12SetCWComplianceSlope(uint8_t slot, uint8_t slope)
{

	AX12s[slot].cwComplianceSlope = slope;

}

uint8_t AX12GetCCWComplianceSlope(uint8_t slot)
{

	return AX12s[slot].ccwComplianceSlope;

}

void AX12SetCCWComplianceSlope(uint8_t slot, uint8_t slope)
{

	AX12s[slot].ccwComplianceSlope = slope;

}

uint16_t AX12GetGoalPosition(uint8_t slot)
{

	return (uint16_t)(((uint16_t)AX12s[slot].goalPositionH << 8) + (uint16_t)AX12s[slot].goalPositionL);

}

void AX12SetGoalPosition(uint8_t slot, int16_t position)
{

	if(position < AX12_MIN_ANGLE) position = AX12_MIN_ANGLE;
	if(position > AX12_MAX_ANGLE) position = AX12_MAX_ANGLE;

	AX12s[slot].goalPositionH = (uint8_t)(position >> 8);
	AX12s[slot].goalPositionL = (uint8_t)(position);

}

uint16_t AX12GetMovingSpeed(uint8_t slot)
{

	return (uint16_t)(((uint16_t)AX12s[slot].movingSpeedH << 8) + (uint16_t)AX12s[slot].movingSpeedL);

}

void AX12SetMovingSpeed(uint8_t slot, uint16_t speed)
{

	AX12s[slot].movingSpeedH = (uint8_t)(speed >> 8);
	AX12s[slot].movingSpeedL = (uint8_t)(speed);

}

uint16_t AX12GetTorqueLimit(uint8_t slot)
{

	return (uint16_t)(((uint16_t)AX12s[slot].torqueLimitH << 8) + (uint16_t)AX12s[slot].torqueLimitL);

}

void AX12SetTorqueLimit(uint8_t slot, uint16_t torque)
{

	AX12s[slot].torqueLimitH = (uint8_t)(torque >> 8);
	AX12s[slot].torqueLimitL = (uint8_t)(torque);

}

uint16_t AX12GetPresentPosition(uint8_t slot)
{

	return (uint16_t)(((uint16_t)AX12s[slot].presentPositionH << 8) + (uint16_t)AX12s[slot].presentPositionL);

}

uint16_t AX12GetPresentSpeed(uint8_t slot)
{

	return (uint16_t)(((uint16_t)AX12s[slot].presentSpeedH << 8) + (uint16_t)AX12s[slot].presentSpeedL);

}

uint16_t AX12GetPresentLoad(uint8_t slot)
{

	return (uint16_t)(((uint16_t)AX12s[slot].presentLoadH << 8) + (uint16_t)AX12s[slot].presentLoadL);

}

// Getters from RS485 bus

void AX12GetInfo(uint8_t bID, AX12 *AX12, uint8_t startPosition, uint8_t endPosition)
{

	uint8_t dynamixelError;
	int positionCount = (endPosition - startPosition) + 1;

	uint8_t txParameters[] = {startPosition, positionCount};

	RS485TxPacket(bID, INST_READ_DATA, txParameters, sizeof(txParameters));

	RS485RxPacket(bID, &dynamixelError, (uint8_t *)AX12 + (startPosition - AX12_FIRST_REGISTER_REPRESENTED_IN_STRUCTURE) + AX12_ID_FIELD_WIDTH, positionCount);

}

void AX12GetInfoSingle(uint8_t slot, uint8_t startPosition, uint8_t endPosition)
{

	AX12GetInfo(AX12s[slot].ID, &AX12s[slot], startPosition, endPosition);

}

void AX12GetInfoAll(uint8_t startPosition, uint8_t endPosition)
{

	for(int slot = 0; slot < AX12Count; slot++){
		AX12GetInfoSingle(slot, startPosition, endPosition);
	}

}

// Setters for individual devices across RS485 bus

void AX12SetInfo(uint8_t bID, AX12 *AX12, uint8_t startPosition, uint8_t endPosition)
{

	uint8_t dynamixelError;
	uint8_t *AX12Bytes = (uint8_t *)AX12;
	int bufferSize = AX12_INSTRUCTION_START_POSITION_WIDTH + (endPosition - startPosition) + 1;
	uint8_t txParameters[bufferSize];

	txParameters[0] = startPosition;

	AX12Bytes += (startPosition - AX12_FIRST_REGISTER_REPRESENTED_IN_STRUCTURE);

	for(uint8_t counter = 1; counter < bufferSize; counter++)
	{
		txParameters[counter] = AX12Bytes[counter];
	}

	RS485TxPacket(bID, INST_WRITE_DATA, txParameters, bufferSize);
	RS485RxPacket(bID, &dynamixelError, NULL, 0);

}

void AX12SetInfoSingle(uint8_t slot, uint8_t startPosition, uint8_t endPosition)
{

	AX12SetInfo(AX12s[slot].ID, &AX12s[slot], startPosition, endPosition);

}

void AX12SetInfoAll(uint8_t startPosition, uint8_t endPosition)
{

	for(int slot = 0; slot < AX12Count; slot++){
		AX12SetInfoSingle(slot, startPosition, endPosition);
	}

}

// Setter for broadcast over RS485 bus

void AX12SetInfoBroadcast(uint8_t txParameters[], int txParametersLength)
{

	RS485TxPacket(DYNAMIXEL_BROADCASTING_ID, INST_WRITE_DATA, txParameters, txParametersLength);

}

// Setters for mass update of devices across RS485

void AX12SetSyncInfo(uint8_t txParameters[], int txParametersLength)
{

	RS485TxPacket(DYNAMIXEL_BROADCASTING_ID, INST_SYNC_WRITE, txParameters, txParametersLength);

}

void AX12SetSyncInfoAll(uint8_t startPosition, uint8_t endPosition)
{

	int positionCount = (endPosition - startPosition) + 1;
	int frameSize = DYNAMIXEL_ID_WIDTH + positionCount;
	//calculate size of array
	int txParametersLength = DYNAMIXEL_SYNC_STARTING_ADDRESS_WIDTH + DYNAMIXEL_SYNC_LENGTH_OF_DATA_WIDTH + (AX12Count * frameSize);
	uint8_t txParameters[txParametersLength];
	//populate
	txParameters[DYNAMIXEL_SYNC_STARTING_ADDRESS_POSITION] = startPosition;
	txParameters[DYNAMIXEL_SYNC_LENGTH_OF_DATA_POSITION] = positionCount;
	for(int slot = 0; slot < AX12Count; slot++){
		txParameters[DYNAMIXEL_SYNC_STARTING_ADDRESS_WIDTH + DYNAMIXEL_SYNC_LENGTH_OF_DATA_WIDTH + (slot * frameSize)] = AX12s[slot].ID;
		memcpy((void *)(txParameters + (DYNAMIXEL_SYNC_STARTING_ADDRESS_WIDTH + DYNAMIXEL_SYNC_LENGTH_OF_DATA_WIDTH) + (slot * frameSize) + DYNAMIXEL_ID_WIDTH), (void *)((uint8_t *)&(AX12s[slot].torqueEnable)) + (startPosition - AX12_FIRST_REGISTER_REPRESENTED_IN_STRUCTURE), positionCount);
	}

	AX12SetSyncInfo(txParameters, txParametersLength);

}

// Serialize
//do a version of this that does all of the servos

bool AX12ReadAX12ImageInMemory(uint8_t ID, uint8_t *buffer)
{
	for(uint8_t slot = 0; slot < AX12Count; slot++){
		if(AX12s[slot].ID == ID)
		{
			memcpy((void *)buffer, (void *)(&(AX12s[slot])), sizeof(AX12));
			return TRUE;
		}
	}
	return FALSE;
}

bool AX12WriteAX12ImageInMemory(uint8_t ID, uint8_t *buffer)
{
	for(uint8_t slot = 0; slot < AX12Count; slot++){
		if(AX12s[slot].ID == ID)
		{
			memcpy((void *)(&(AX12s[slot])), (void *)buffer, sizeof(AX12));
			return TRUE;
		}
	}
	return FALSE;
}

bool AX12UpdateAX12ImageInMemoryFromDevice(uint8_t ID)
{
	for(uint8_t slot = 0; slot < AX12Count; slot++){
		if(AX12s[slot].ID == ID)
		{
			AX12GetInfoSingle(slot, AX12_FIRST_REGISTER_REPRESENTED_IN_STRUCTURE, AX12_LAST_REGISTER_REPRESENTED_IN_STRUCTURE);
			return TRUE;
		}
	}
	return FALSE;
}

bool AX12UpdateAX12DeviceFromImageInMemory(uint8_t ID)
{
	for(uint8_t slot = 0; slot < AX12Count; slot++){
		if(AX12s[slot].ID == ID)
		{
			AX12SetInfoSingle(slot, AX12_FIRST_REGISTER_REPRESENTED_IN_STRUCTURE, AX12_LAST_REGISTER_REPRESENTED_IN_STRUCTURE);
			return TRUE;
		}
	}
	return FALSE;
}

void AX12UpdateAllAX12ImagesInMemoryFromDevices()
{
	AX12SetSyncInfoAll(AX12_FIRST_REGISTER_REPRESENTED_IN_STRUCTURE, AX12_LAST_REGISTER_REPRESENTED_IN_STRUCTURE);
}

void AX12UpdateAllAX12DevicesFromImagesInMemory()
{
	AX12GetInfoAll(AX12_FIRST_REGISTER_REPRESENTED_IN_STRUCTURE, AX12_LAST_REGISTER_REPRESENTED_IN_STRUCTURE);
}

// Display stats from device representation in memory

void AX12DisplayStats(AX12 *AX12)
{

	char outputMessage[48];

	sprintf(outputMessage, "AX12 Found. ID: %u\r\n", AX12->ID);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Torque Enable: %u\r\n", AX12->torqueEnable);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "LED: %u\r\n", AX12->LED);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "CW Compliance Margin: %u\r\n", AX12->cwComplianceMargin);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "CCW Compliance Margin: %u\r\n", AX12->ccwComplianceMargin);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "CW Compliance Slope: %u\r\n", AX12->cwComplianceSlope);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "CCW Compliance Slope: %u\r\n", AX12->ccwComplianceSlope);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Goal Position Low: %u\r\n", AX12->goalPositionL);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Goal Position High: %u\r\n", AX12->goalPositionH);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Goal Speed Low: %u\r\n", AX12->movingSpeedL);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Goal Speed High: %u\r\n", AX12->movingSpeedH);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Torque Limit Low: %u\r\n", AX12->torqueLimitL);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Torque Limit High: %u\r\n", AX12->torqueLimitH);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Present Position Low: %u\r\n", AX12->presentPositionL);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Present Position High: %u\r\n", AX12->presentPositionH);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Present Speed Low: %u\r\n", AX12->presentSpeedL);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Present Speed High: %u\r\n", AX12->presentSpeedH);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Present Load Low: %u\r\n", AX12->presentLoadL);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Present Load High: %u\r\n", AX12->presentLoadH);
	bluetoothTxString(outputMessage);

	bluetoothTxString("\r\n");

}


