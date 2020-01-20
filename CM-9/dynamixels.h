/*
 * dynamixels.h
 *
 *  Created on: Dec 19, 2013
 *      Author: Bill
 */

#ifndef DYNAMIXELS_H_
#define DYNAMIXELS_H_

#include "common.h"

#define DYNAMIXEL_ID_WIDTH							1
#define DYNAMIXEL_SYNC_STARTING_ADDRESS_POSITION	0
#define DYNAMIXEL_SYNC_STARTING_ADDRESS_WIDTH		1
#define DYNAMIXEL_SYNC_LENGTH_OF_DATA_POSITION		1
#define DYNAMIXEL_SYNC_LENGTH_OF_DATA_WIDTH			1

#define DYNAMIXEL_BROADCASTING_ID					0xfe

#define DYNAMIXEL_MODEL_NUMBER_L					0x00
#define DYNAMIXEL_MODEL_NUMBER_H					0x01

#define DYNAMIXEL_MAX_NUM							253

bool dynamixelsPing(uint8_t bID, uint8_t *errorNo);
bool dynamixelsReadData(uint8_t ID, uint8_t *errorNo, uint8_t *requestParameters, uint8_t requestParametersLength, uint8_t *responseParameters, uint8_t responseParametersLength);
bool dynamixelsWriteData(uint8_t ID, uint8_t *errorNo, uint8_t *requestParameters, uint8_t requestParametersLength);

bool dynamixelsIsType(uint8_t bID, uint16_t dynamixelType);

#endif /* DYNAMIXELS_H_ */
