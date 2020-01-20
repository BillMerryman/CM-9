/*
 * AXS1.c
 *
 * Created: 1/12/2013 12:31:07 AM
 *  Author: MAIN
 */ 

#include <stdio.h>
#include <string.h>

#include "led.h"
#include "bluetooth.h"
#include "protocol.h"
#include "RS485.h"
#include "dynamixels.h"
#include "AXS1.h"

AXS1 AXS1s[AXS1_NUM_ATTACHED];
uint8_t AXS1Count;

// Initialize device representations in memory

void AXS1sInitialize(void)
{

	#ifdef DYNAMIXEL_DEBUG
		bluetoothTxString("Start Initializing AXS1s\r\n");
	#endif

	uint8_t errorNo;
	AXS1Count = 0;

	for(uint8_t ID = 0; ID < 254; ID++){
		if(dynamixelsPing(ID, &errorNo)){
			if(dynamixelsIsType(ID, AXS1_MODEL_NUMBER)){
				AXS1s[AXS1Count].ID = ID;
				AXS1GetInfo(ID, &AXS1s[AXS1Count], AXS1_FIRST_REGISTER_REPRESENTED_IN_STRUCTURE, AXS1_LAST_REGISTER_REPRESENTED_IN_STRUCTURE);

				#ifdef DYNAMIXEL_DEBUG
					AXS1DisplayStats(&AXS1s[AXS1Count]);
				#endif

				AXS1Count++;
			}
		}
	}

	#ifdef DYNAMIXEL_DEBUG
	{
		char outputMessage[32];
		sprintf(outputMessage, "AXS1 Count: %u\r\n", AXS1Count);
		bluetoothTxString(outputMessage);
		bluetoothTxString("End Initializing AXS1s\r\n");
	}
	#endif

}

// Local getters and setters

uint8_t AXS1sGetCount(void)
{

	return AXS1Count;

}

uint8_t AXS1GetID(uint8_t slot)
{

	return AXS1s[slot].ID;

}

uint8_t AXS1GetObstacleDetectedCompareValue(uint8_t slot)
{

	return AXS1s[slot].obstacleDetectedCompareValue;

}

void AXS1SetObstacleDetectedCompareValue(uint8_t slot, uint8_t value)
{

	AXS1s[slot].obstacleDetectedCompareValue = value;

}

uint8_t AXS1GetLightDetectedCompareValue(uint8_t slot)
{

	return AXS1s[slot].lightDetectedCompareValue;

}

void AXS1SetLightDetectedCompareValue(uint8_t slot, uint8_t value)
{

	AXS1s[slot].lightDetectedCompareValue = value;

}

uint8_t AXS1GetLeftIRSensorData(uint8_t slot)
{

	return AXS1s[slot].leftIRSensorData;

}

uint8_t AXS1GetCenterIRSensorData(uint8_t slot)
{

	return AXS1s[slot].centerIRSensorData;

}

uint8_t AXS1GetRightIRSensorData(uint8_t slot)
{

	return AXS1s[slot].rightIRSensorData;

}

uint8_t AXS1GetLeftLuminosity(uint8_t slot)
{

	return AXS1s[slot].leftLuminosity;

}

uint8_t AXS1GetCenterLuminosity(uint8_t slot)
{

	return AXS1s[slot].centerLuminosity;

}

uint8_t AXS1GetRightLuminosity(uint8_t slot)
{

	return AXS1s[slot].rightLuminosity;

}

uint8_t AXS1GetObstacleDetectionFlag(uint8_t slot)
{

	return AXS1s[slot].obstacleDetectionFlag;

}

uint8_t AXS1GetLuminosityDetectionFlag(uint8_t slot)
{

	return AXS1s[slot].luminosityDetectionFlag;

}

uint8_t AXS1GetSoundDataMaxHold(uint8_t slot)
{

	return AXS1s[slot].soundDataMaxHold;

}

void AXS1SetSoundDataMaxHold(uint8_t slot, uint8_t value)
{

	AXS1s[slot].soundDataMaxHold = value;

}

uint8_t AXS1GetSoundDetectedCount(uint8_t slot)
{

	return AXS1s[slot].soundDetectedCount;

}

void AXS1SetSoundDetectedCount(uint8_t slot, uint8_t value)
{

	AXS1s[slot].soundDetectedCount = value;

}

uint16_t AXS1GetSoundDetectedTime(uint8_t slot)
{

	return (uint16_t)(((uint16_t)AXS1s[slot].soundDetectedTimeH << 8) + (uint16_t)AXS1s[slot].soundDetectedTimeL);

}

void AXS1SetSoundDetectedTime(uint8_t slot, uint16_t value)
{

	AXS1s[slot].soundDetectedTimeH = (uint8_t)(value >> 8);
	AXS1s[slot].soundDetectedTimeL = (uint8_t)(value);

}

uint8_t AXS1GetBuzzerIndex(uint8_t slot)
{

	return AXS1s[slot].buzzerIndex;

}

void AXS1SetBuzzerIndex(uint8_t slot, uint8_t value)
{

	AXS1s[slot].buzzerIndex = value;

}

uint8_t AXS1GetBuzzerTime(uint8_t slot)
{

	return AXS1s[slot].buzzerTime;

}

void AXS1SetBuzzerTime(uint8_t slot, uint8_t value)
{

	AXS1s[slot].buzzerTime = value;

}

uint8_t AXS1GetPresentVoltage(uint8_t slot)
{

	return AXS1s[slot].presentVoltage;

}

uint8_t AXS1GetPresentTemperature(uint8_t slot)
{

	return AXS1s[slot].presentTemperature;

}

uint8_t AXS1GetRegisteredInstruction(uint8_t slot)
{

	return AXS1s[slot].registeredInstruction;

}

void AXS1SetRegisteredInstruction(uint8_t slot, uint8_t value)
{

	AXS1s[slot].registeredInstruction = value;

}

uint8_t AXS1GetIRRemoconArrived(uint8_t slot)
{

	return AXS1s[slot].irRemoconArrived;

}

uint8_t AXS1GetLock(uint8_t slot)
{

	return AXS1s[slot].lock;

}

void AXS1SetLock(uint8_t slot, uint8_t value)
{

	AXS1s[slot].lock = value;

}

uint8_t AXS1GetIRRemoconRXData0(uint8_t slot)
{

	return AXS1s[slot].irRemoconRXData0;

}

uint8_t AXS1GetIRRemoconRXData1(uint8_t slot)
{

	return AXS1s[slot].irRemoconRXData1;

}

uint8_t AXS1GetIRRemoconTXData0(uint8_t slot)
{

	return AXS1s[slot].irRemoconTXData0;

}

void AXS1SetIRRemoconTXData0(uint8_t slot, uint8_t value)
{

	AXS1s[slot].irRemoconTXData0 = value;

}

uint8_t AXS1GetIRRemoconTXData1(uint8_t slot)
{

	return AXS1s[slot].irRemoconTXData1;

}

void AXS1SetIRRemoconTXData1(uint8_t slot, uint8_t value)
{

	AXS1s[slot].irRemoconTXData1 = value;

}

uint8_t AXS1GetObstacleDetectedCompare(uint8_t slot)
{

	return AXS1s[slot].obstacleDetectedCompare;

}

void AXS1SetObstacleDetectedCompare(uint8_t slot, uint8_t value)
{

	AXS1s[slot].obstacleDetectedCompare = value;

}

uint8_t AXS1GetLightDetectedCompare(uint8_t slot)
{

	return AXS1s[slot].lightDetectedCompare;

}

void AXS1SetLightDetectedCompare(uint8_t slot, uint8_t value)
{

	AXS1s[slot].lightDetectedCompare = value;

}

// Getters from RS485 bus

void AXS1GetInfo(uint8_t bID, AXS1 *AXS1, uint8_t startPosition, uint8_t endPosition)
{

	uint8_t dynamixelError;
	int positionCount = (endPosition - startPosition) + 1;

	uint8_t txParameters[] = {startPosition, positionCount};

	RS485TxPacket(bID, INST_READ_DATA, txParameters, sizeof(txParameters));

	RS485RxPacket(bID, &dynamixelError, (uint8_t *)AXS1 + (startPosition - AXS1_FIRST_REGISTER_REPRESENTED_IN_STRUCTURE) + AXS1_ID_FIELD_WIDTH, positionCount);

}

void AXS1GetInfoSingle(uint8_t slot, uint8_t startPosition, uint8_t endPosition)
{

	AXS1GetInfo(AXS1s[slot].ID, &AXS1s[slot], startPosition, endPosition);

}

void AXS1GetInfoAll(uint8_t startPosition, uint8_t endPosition)
{

	for(int slot = 0; slot < AXS1Count; slot++){
		AXS1GetInfoSingle(slot, startPosition, endPosition);
	}

}

// Setters for individual devices across RS485 bus

void AXS1SetInfo(uint8_t bID, AXS1 *AXS1, uint8_t startPosition, uint8_t endPosition)
{

	uint8_t dynamixelError;
	uint8_t *AXS1Bytes = (uint8_t *)AXS1;
	int bufferSize = AXS1_INSTRUCTION_START_POSITION_WIDTH + (endPosition - startPosition) + 1;
	uint8_t txParameters[bufferSize];

	txParameters[0] = startPosition;

	AXS1Bytes += (startPosition - AXS1_FIRST_REGISTER_REPRESENTED_IN_STRUCTURE);

	for(uint8_t counter = 1; counter < bufferSize; counter++)
	{
		txParameters[counter] = AXS1Bytes[counter];
	}

	RS485TxPacket(bID, INST_WRITE_DATA, txParameters, bufferSize);
	RS485RxPacket(bID, &dynamixelError, NULL, 0);

}

void AXS1SetInfoSingle(uint8_t slot, uint8_t startPosition, uint8_t endPosition)
{

	AXS1SetInfo(AXS1s[slot].ID, &AXS1s[slot], startPosition, endPosition);

}

void AXS1SetInfoAll(uint8_t startPosition, uint8_t endPosition)
{

	for(int slot = 0; slot < AXS1Count; slot++){
		AXS1SetInfoSingle(slot, startPosition, endPosition);
	}

}

// Setter for broadcast over RS485 bus

void AXS1SetInfoBroadcast(uint8_t txParameters[], int txParametersLength)
{

	RS485TxPacket(DYNAMIXEL_BROADCASTING_ID, INST_WRITE_DATA, txParameters, txParametersLength);

}

// Setters for mass update of devices across RS485

void AXS1SetSyncInfo(uint8_t txParameters[], int txParametersLength)
{

	RS485TxPacket(DYNAMIXEL_BROADCASTING_ID, INST_SYNC_WRITE, txParameters, txParametersLength);

}

void AXS1SetSyncInfoAll(uint8_t startPosition, uint8_t endPosition)
{

	int positionCount = (endPosition - startPosition) + 1;
	int frameSize = DYNAMIXEL_ID_WIDTH + positionCount;
	//calculate size of array
	int txParametersLength = DYNAMIXEL_SYNC_STARTING_ADDRESS_WIDTH + DYNAMIXEL_SYNC_LENGTH_OF_DATA_WIDTH + (AXS1Count * frameSize);
	uint8_t txParameters[txParametersLength];
	//populate
	txParameters[DYNAMIXEL_SYNC_STARTING_ADDRESS_POSITION] = startPosition;
	txParameters[DYNAMIXEL_SYNC_LENGTH_OF_DATA_POSITION] = positionCount;
	for(int slot = 0; slot < AXS1Count; slot++){
		txParameters[DYNAMIXEL_SYNC_STARTING_ADDRESS_WIDTH + DYNAMIXEL_SYNC_LENGTH_OF_DATA_WIDTH + (slot * frameSize)] = AXS1s[slot].ID;
		memcpy((void *)(txParameters + (DYNAMIXEL_SYNC_STARTING_ADDRESS_WIDTH + DYNAMIXEL_SYNC_LENGTH_OF_DATA_WIDTH) + (slot * frameSize) + DYNAMIXEL_ID_WIDTH), (void *)((uint8_t *)&(AXS1s[slot].obstacleDetectedCompareValue)) + (startPosition - AXS1_OBSTACLE_DETECTED_COMPARE), positionCount);
	}

	AXS1SetSyncInfo(txParameters, txParametersLength);

}

// Serialize
//do a version of this that does all of the servos

bool AXS1ReadAXS1ImageInMemory(uint8_t ID, uint8_t *buffer)
{
	for(uint8_t slot = 0; slot < AXS1Count; slot++){
		if(AXS1s[slot].ID == ID)
		{
			memcpy((void *)buffer, (void *)(&(AXS1s[slot])), sizeof(AXS1));
			return TRUE;
		}
	}
	return FALSE;
}

bool AXS1WriteAXS1ImageInMemory(uint8_t ID, uint8_t *buffer)
{
	for(uint8_t slot = 0; slot < AXS1Count; slot++){
		if(AXS1s[slot].ID == ID)
		{
			memcpy((void *)(&(AXS1s[slot])), (void *)buffer, sizeof(AXS1));
			return TRUE;
		}
	}
	return FALSE;
}

bool AXS1UpdateAXS1ImageInMemoryFromDevice(uint8_t ID)
{
	for(uint8_t slot = 0; slot < AXS1Count; slot++){
		if(AXS1s[slot].ID == ID)
		{
			AXS1GetInfoSingle(slot, AXS1_FIRST_REGISTER_REPRESENTED_IN_STRUCTURE, AXS1_LAST_REGISTER_REPRESENTED_IN_STRUCTURE);
			return TRUE;
		}
	}
	return FALSE;
}

bool AXS1UpdateAXS1DeviceFromImageInMemory(uint8_t ID)
{
	for(uint8_t slot = 0; slot < AXS1Count; slot++){
		if(AXS1s[slot].ID == ID)
		{
			AXS1SetInfoSingle(slot, AXS1_FIRST_REGISTER_REPRESENTED_IN_STRUCTURE, AXS1_LAST_REGISTER_REPRESENTED_IN_STRUCTURE);
			return TRUE;
		}
	}
	return FALSE;
}

void AXS1UpdateAllAXS1ImagesInMemoryFromDevices()
{
	AXS1SetSyncInfoAll(AXS1_FIRST_REGISTER_REPRESENTED_IN_STRUCTURE, AXS1_LAST_REGISTER_REPRESENTED_IN_STRUCTURE);
}

void AXS1UpdateAllAXS1DevicesFromImagesInMemory()
{
	AXS1GetInfoAll(AXS1_FIRST_REGISTER_REPRESENTED_IN_STRUCTURE, AXS1_LAST_REGISTER_REPRESENTED_IN_STRUCTURE);
}

// Display stats from device representation in memory

void AXS1DisplayStats(AXS1 *AXS1)
{

	char outputMessage[64];

	sprintf(outputMessage, "AXS1 Found. ID: %u\r\n", AXS1->ID);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Obstacle Detected Compare Value: %u\r\n", AXS1->obstacleDetectedCompareValue);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Light Detected Compare Value: %u\r\n", AXS1->lightDetectedCompareValue);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Left IR Sensor Data: %u\r\n", AXS1->leftIRSensorData);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Center IR Sensor Data: %u\r\n", AXS1->centerIRSensorData);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Right IRSensor Data: %u\r\n", AXS1->rightIRSensorData);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Left Luminosity: %u\r\n", AXS1->leftLuminosity);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Center Luminosity: %u\r\n", AXS1->centerLuminosity);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Right Luminosity: %u\r\n", AXS1->rightLuminosity);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Obstacle Detection Flag: %u\r\n", AXS1->obstacleDetectionFlag);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Luminosity Detection Flag: %u\r\n", AXS1->luminosityDetectionFlag);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Sound Data: %u\r\n", AXS1->soundData);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Sound Data Max Hold: %u\r\n", AXS1->soundDataMaxHold);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Sound Detected Count: %u\r\n", AXS1->soundDetectedCount);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Sound Detected Time L: %u\r\n", AXS1->soundDetectedTimeL);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "sound Detected Time H: %u\r\n", AXS1->soundDetectedTimeH);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Buzzer Index: %u\r\n", AXS1->buzzerIndex);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Buzzer Time: %u\r\n", AXS1->buzzerTime);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "IR Remocon Arrived: %u\r\n", AXS1->irRemoconArrived);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "IR Remocon RX Data 0: %u\r\n", AXS1->irRemoconRXData0);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "IR Remocon RX Data 1: %u\r\n", AXS1->irRemoconRXData1);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "IR Remocon TX Data 0: %u\r\n", AXS1->irRemoconTXData0);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "IR Remocon TX Data 1: %u\r\n", AXS1->irRemoconTXData1);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Obstacle Detected Compare: %u\r\n", AXS1->obstacleDetectedCompare);
	bluetoothTxString(outputMessage);
	sprintf(outputMessage, "Light Detected Compare: %u\r\n", AXS1->lightDetectedCompare);
	bluetoothTxString(outputMessage);

	bluetoothTxString("\r\n");

}
