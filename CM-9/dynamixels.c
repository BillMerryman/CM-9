/*
 * dynamixels.c
 *
 *  Created on: Dec 19, 2013
 *      Author: Bill
 */

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "led.h"
#include "bluetooth.h"
#include "protocol.h"
#include "RS485.h"
#include "dynamixels.h"

bool dynamixelsPing(uint8_t bID, uint8_t *errorNo)
{

	RS485TxPacket(bID, INST_PING, NULL, 0);
	if(RS485RxPacket(bID, errorNo, NULL, 0))
	{
		return TRUE;
	}
	else
	{
		#ifdef DYNAMIXEL_ENABLE_MISSING_WARNING
		{
			char outputMessage[32];
			sprintf(outputMessage, "Device #%i not connected\r\n", bID);
			bluetoothTxString(outputMessage);
		}
		#endif
		return FALSE;
	}

}

bool dynamixelsReadData(uint8_t ID, uint8_t *errorNo, uint8_t *requestParameters, uint8_t requestParametersLength, uint8_t *responseParameters, uint8_t responseParametersLength)
{

	RS485TxPacket(ID, INST_READ_DATA, requestParameters, requestParametersLength);
	if(RS485RxPacket(ID, errorNo, responseParameters, responseParametersLength))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

bool dynamixelsWriteData(uint8_t ID, uint8_t *errorNo, uint8_t *requestParameters, uint8_t requestParametersLength)
{

	RS485TxPacket(ID, INST_WRITE_DATA, requestParameters, requestParametersLength);
	if(RS485RxPacket(ID, errorNo, NULL, 0))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

bool dynamixelsIsType(uint8_t bID, uint16_t dynamixelType)
{

	uint8_t dynamixelError = 0;
	uint8_t TxAndRxParameters[] = {DYNAMIXEL_MODEL_NUMBER_L, 2};
	RS485TxPacket(bID, INST_READ_DATA, TxAndRxParameters, sizeof(TxAndRxParameters));
	RS485RxPacket(bID, &dynamixelError, TxAndRxParameters, sizeof(TxAndRxParameters));
	uint16_t modelNumberL = TxAndRxParameters[0];
	uint16_t modelNumberH = TxAndRxParameters[1];
	uint16_t modelNumber = (modelNumberH << 8) + modelNumberL;

	if(modelNumber == dynamixelType)
	{
		return TRUE;
	}
	else
	{
		#ifdef DYNAMIXEL_ENABLE_WARNINGS
		{
			char outputMessage[32];
			sprintf(outputMessage, "Device #%i not requested type\r\n", bID);
			bluetoothTxString(outputMessage);
		}
		#endif
		return FALSE;
	}

}
