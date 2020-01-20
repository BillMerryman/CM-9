/*
 * AXS1Full.h
 *
 * Created: 1/12/2013 11:39:29 PM
 *  Author: MAIN
 */ 


#ifndef AXS1FULL_H_
#define AXS1FULL_H_

#include "common.h"

#define AXS1_MODEL_NUMBER						0x0D

#define AXS1_MODEL_NUMBER_L						0x00
#define AXS1_MODEL_NUMBER_H						0x01
#define AXS1_FIRMWARE_VERSION					0x02
#define AXS1_ID									0x03
#define AXS1_BAUD_RATE							0x04
#define AXS1_RETURN_DELAY_TIME					0x05
#define AXS1_RESERVED_1							0x06
#define AXS1_RESERVED_2							0x07
#define AXS1_RESERVED_3							0x08
#define AXS1_RESERVED_4							0x09
#define AXS1_RESERVED_5							0x0A
#define AXS1_HIGH_TEMP_LIMIT					0x0B
#define AXS1_LOW_VOLTAGE_LIMIT					0x0C
#define AXS1_HIGH_VOLTAGE_LIMIT					0x0D
#define AXS1_RESERVED_6							0x0E
#define AXS1_RESERVED_7							0x0F
#define AXS1_STATUS_RETURN_LEVEL				0x10
#define AXS1_RESERVED_8							0x11
#define AXS1_RESERVED_9							0x12
#define AXS1_RESERVED_10						0x13
#define AXS1_OBSTACLE_DETECTED_COMPARE_VALUE	0x14
#define AXS1_LIGHT_DETECTED_COMPARE_VALUE		0x15
#define AXS1_RESERVED_11						0x16
#define AXS1_RESERVED_12						0x17
#define AXS1_RESERVED_13						0x18
#define AXS1_RESERVED_14						0x19
#define AXS1_LEFT_IR_SENSOR_DATA				0x1A
#define AXS1_CENTER_IR_SENSOR_DATA				0x1B
#define AXS1_RIGHT_IR_SENSOR_DATA				0x1C
#define AXS1_LEFT_LUMINOSITY					0x1D
#define AXS1_CENTER_LUMINOSITY					0x1E
#define AXS1_RIGHT_LUMINOSITY					0x1F
#define AXS1_OBSTACLE_DETECTION_FLAG			0x20
#define AXS1_LUMINOSITY_DETECTION_FLAG			0x21
#define AXS1_RESERVED_15						0x22
#define AXS1_SOUND_DATA							0x23
#define AXS1_SOUND_DATA_MAX_HOLD				0x24
#define AXS1_SOUND_DETECTED_COUNT				0x25
#define AXS1_SOUND_DETECTED_TIME_L				0x26
#define AXS1_SOUND_DETECTED_TIME_H				0x27
#define AXS1_BUZZER_INDEX						0x28
#define AXS1_BUZZER_TIME						0x29
#define AXS1_PRESENT_VOLTAGE					0x2A
#define AXS1_PRESENT_TEMPERATURE				0x2B
#define AXS1_REGISTERED_INSTRUCTION				0x2C
#define AXS1_RESERVED_16						0x2D
#define AXS1_IR_REMOCON_ARRIVED					0x2E
#define AXS1_LOCK								0x2F
#define AXS1_IR_REMOCON_RX_DATA_0				0x30
#define AXS1_IR_REMOCON_RX_DATA_1				0x31
#define AXS1_IR_REMOCON_RX_DATA_2				0x32
#define AXS1_IR_REMOCON_RX_DATA_3				0x33
#define AXS1_OBSTACLE_DETECTED_COMPARE			0x34
#define AXS1_LIGHT_DETECTED_COMPARE				0x35

typedef struct{	
	byte modelNumberL;
	byte modelNumberH;
	byte firmwareVersion;
	byte ID;
	byte baudRate;
	byte returnDelayTime;
	byte reserved1;
	byte reserved2;
	byte reserved3;
	byte reserved4;
	byte reserved5;
	byte highTempLimit;
	byte lowVoltageLimit;
	byte highVoltageLimit;
	byte reserved6;
	byte reserved7;
	byte statusReturnLevel;
	byte reserved8;
	byte reserved9;
	byte reserved10;
	byte obstacleDetectedCompareValue;
	byte lightDetectedCompareValue;
	byte reserved11;
	byte reserved12;
	byte reserved13;
	byte reserved14;
	byte leftIRSensorData;
	byte centerIRSensorData;
	byte rightIRSensorData;
	byte leftLuminosity;
	byte centerLuminosity;
	byte rightLuminosity;
	byte obstacleDetectionFlag;
	byte luminosityDetectionFlag;
	byte reserved15;
	byte soundData;
	byte soundDataMaxHold;
	byte soundDetectedCount;
	byte soundDetectedTimeL;
	byte soundDetectedTimeH;
	byte buzzerIndex;
	byte buzzerTime;
	byte presentVoltage;
	byte presentTemperature;
	byte registeredInstruction;
	byte reserved16;
	byte irRemoconArrived;
	byte lock;
	byte irRemoconRXData0;
	byte irRemoconRXData1;
	byte irRemoconTXData0;
	byte irRemoconTXData1;
	byte obstacleDetectedCompare;
	byte lightDetectedCompare;
}AXS1;

#endif /* AXS1FULL_H_ */
