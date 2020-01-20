/*
 * AXS1.h
 *
 * Created: 1/12/2013 12:30:47 AM
 *  Author: MAIN
 */ 

#ifndef AXS1_H_
#define AXS1_H_

#include "common.h"

#define AXS1_MODEL_NUMBER						0x0D
#define AXS1_NUM_ATTACHED						1
#define AXS1_ID_FIELD_WIDTH						1
#define AXS1_INSTRUCTION_START_POSITION_WIDTH	1

#define AXS1_MODEL_NUMBER_L						0x00
#define AXS1_MODEL_NUMBER_H						0x01
#define AXS1_ID									0x03
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
#define AXS1_IR_REMOCON_TX_DATA_0				0x32
#define AXS1_IR_REMOCON_TX_DATA_1				0x33
#define AXS1_OBSTACLE_DETECTED_COMPARE			0x34
#define AXS1_LIGHT_DETECTED_COMPARE				0x35

#define AXS1_FIRST_REGISTER_REPRESENTED_IN_STRUCTURE	AXS1_OBSTACLE_DETECTED_COMPARE_VALUE
#define AXS1_LAST_REGISTER_REPRESENTED_IN_STRUCTURE 	AXS1_LIGHT_DETECTED_COMPARE

typedef struct{	
	uint8_t ID;
	volatile uint8_t obstacleDetectedCompareValue;
	volatile uint8_t lightDetectedCompareValue;
	volatile uint8_t reserved11;
	volatile uint8_t reserved12;
	volatile uint8_t reserved13;
	volatile uint8_t reserved14;
	volatile uint8_t leftIRSensorData;
	volatile uint8_t centerIRSensorData;
	volatile uint8_t rightIRSensorData;
	volatile uint8_t leftLuminosity;
	volatile uint8_t centerLuminosity;
	volatile uint8_t rightLuminosity;
	volatile uint8_t obstacleDetectionFlag;
	volatile uint8_t luminosityDetectionFlag;
	volatile uint8_t reserved15;
	volatile uint8_t soundData;
	volatile uint8_t soundDataMaxHold;
	volatile uint8_t soundDetectedCount;
	volatile uint8_t soundDetectedTimeL;
	volatile uint8_t soundDetectedTimeH;
	volatile uint8_t buzzerIndex;
	volatile uint8_t buzzerTime;
	volatile uint8_t presentVoltage;
	volatile uint8_t presentTemperature;
	volatile uint8_t registeredInstruction;
	volatile uint8_t reserved16;
	volatile uint8_t irRemoconArrived;
	volatile uint8_t lock;
	volatile uint8_t irRemoconRXData0;
	volatile uint8_t irRemoconRXData1;
	volatile uint8_t irRemoconTXData0;
	volatile uint8_t irRemoconTXData1;
	volatile uint8_t obstacleDetectedCompare;
	volatile uint8_t lightDetectedCompare;
} AXS1;

// Initialize device representations in memory

void AXS1sInitialize(void);

// Local getters and setters

uint8_t AXS1sGetCount(void);
uint8_t AXS1GetID(uint8_t slot);
uint8_t AXS1GetObstacleDetectedCompareValue(uint8_t slot);
void AXS1SetObstacleDetectedCompareValue(uint8_t slot, uint8_t value);
uint8_t AXS1GetLightDetectedCompareValue(uint8_t slot);
void AXS1SetLightDetectedCompareValue(uint8_t slot, uint8_t value);
uint8_t AXS1GetLeftIRSensorData(uint8_t slot);
uint8_t AXS1GetCenterIRSensorData(uint8_t slot);
uint8_t AXS1GetRightIRSensorData(uint8_t slot);
uint8_t AXS1GetLeftLuminosity(uint8_t slot);
uint8_t AXS1GetCenterLuminosity(uint8_t slot);
uint8_t AXS1GetRightLuminosity(uint8_t slot);
uint8_t AXS1GetObstacleDetectionFlag(uint8_t slot);
uint8_t AXS1GetLuminosityDetectionFlag(uint8_t slot);
uint8_t AXS1GetSoundDataMaxHold(uint8_t slot);
void AXS1SetSoundDataMaxHold(uint8_t slot, uint8_t value);
uint8_t AXS1GetSoundDetectedCount(uint8_t slot);
void AXS1SetSoundDetectedCount(uint8_t slot, uint8_t value);
uint16_t AXS1GetSoundDetectedTime(uint8_t slot);
void AXS1SetSoundDetectedTime(uint8_t slot, uint16_t value);
uint8_t AXS1GetBuzzerIndex(uint8_t slot);
void AXS1SetBuzzerIndex(uint8_t slot, uint8_t value);
uint8_t AXS1GetBuzzerTime(uint8_t slot);
void AXS1SetBuzzerTime(uint8_t slot, uint8_t value);
uint8_t AXS1GetPresentVoltage(uint8_t slot);
uint8_t AXS1GetPresentTemperature(uint8_t slot);
uint8_t AXS1GetRegisteredInstruction(uint8_t slot);
void AXS1SetRegisteredInstruction(uint8_t slot, uint8_t value);
uint8_t AXS1GetIRRemoconArrived(uint8_t slot);
uint8_t AXS1GetLock(uint8_t slot);
void AXS1SetLock(uint8_t slot, uint8_t value);
uint8_t AXS1GetIRRemoconRXData0(uint8_t slot);
uint8_t AXS1GetIRRemoconRXData1(uint8_t slot);
uint8_t AXS1GetIRRemoconTXData0(uint8_t slot);
void AXS1SetIRRemoconTXData0(uint8_t slot, uint8_t value);
uint8_t AXS1GetIRRemoconTXData1(uint8_t slot);
void AXS1SetIRRemoconTXData1(uint8_t slot, uint8_t value);
uint8_t AXS1GetObstacleDetectedCompare(uint8_t slot);
void AXS1SetObstacleDetectedCompare(uint8_t slot, uint8_t value);
uint8_t AXS1GetLightDetectedCompare(uint8_t slot);
void AXS1SetLightDetectedCompare(uint8_t slot, uint8_t value);

// Getters from RS485 bus

void AXS1GetInfo(uint8_t bID, AXS1 *AXS1, uint8_t startPosition, uint8_t endPosition);
void AXS1GetInfoSingle(uint8_t slot, uint8_t startPosition, uint8_t endPosition);
void AXS1GetInfoAll(uint8_t startPosition, uint8_t endPosition);

// Setters for individual devices across RS485 bus

void AXS1SetInfo(uint8_t slot, AXS1 *AXS1, uint8_t startPosition, uint8_t endPosition);
void AXS1SetInfoSingle(uint8_t slot, uint8_t startPosition, uint8_t endPosition);
void AXS1SetInfoAll(uint8_t startPosition, uint8_t endPosition);

// Setter for broadcast over RS485 bus

void AXS1SetInfoBroadcast(uint8_t txParameters[], int txParametersLength);

// Setters for mass update of devices across RS485

void AXS1SetSyncInfo(uint8_t txParameters[], int txParametersLength);
void AXS1SetSyncInfoAll(uint8_t startPosition, uint8_t endPosition);

// Serialize

bool AXS1ReadAXS1ImageInMemory(uint8_t ID, uint8_t *buffer);
bool AXS1WriteAXS1ImageInMemory(uint8_t ID, uint8_t *buffer);
bool AXS1UpdateAXS1ImageInMemoryFromDevice(uint8_t ID);
bool AXS1UpdateAXS1DeviceFromImageInMemory(uint8_t ID);
void AXS1UpdateAllAXS1ImagesInMemoryFromDevices(void);
void AXS1UpdateAllAXS1DevicesFromImagesInMemory(void);

// Display stats from device representation in memory

void AXS1DisplayStats(AXS1 *AXS1);

#endif /* AXS1_H_ */
