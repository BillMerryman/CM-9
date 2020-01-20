/*
 * AX12Full.h
 *
 * Created: 1/12/2013 11:12:13 PM
 *  Author: MAIN
 */ 


#ifndef AX12FULL_H_
#define AX12FULL_H_

#define AX12_MODEL_NUMBER					0x0C

#define AX12_MODEL_NUMBER_L					0x00
#define AX12_MODEL_NUMBER_H					0x01
#define AX12_FIRMWARE_VERSION				0x02
#define AX12_ID								0x03
#define AX12_BAUD_RATE						0x04
#define AX12_RETURN_DELAY_TIME				0x05
#define AX12_CW_ANGLE_LIMIT_L				0x06
#define AX12_CW_ANGLE_LIMIT_H				0x07
#define AX12_CCW_ANGLE_LIMIT_L				0x08
#define AX12_CCW_ANGLE_LIMIT_H				0x09
#define AX12_RESERVED_1						0x0A
#define AX12_HIGH_TEMP_LIMIT				0x0B
#define AX12_LOW_VOLTAGE_LIMIT				0x0C
#define AX12_HIGH_VOLTAGE_LIMIT				0x0D
#define AX12_MAX_TORQUE_L					0x0E
#define AX12_MAX_TORQUE_H					0x0F
#define AX12_STATUS_RETURN_LEVEL			0x10
#define AX12_ALARM_LED						0x11
#define AX12_ALARM_SHUTDOWN					0x12
#define AX12_RESERVED_2						0x13
#define AX12_DOWN_CALIBRATION_L				0x14
#define AX12_DOWN_CALIBRATION_H				0x15
#define AX12_UP_CALIBRATION_L				0x16
#define AX12_UP_CALIBRATION_H				0x17
#define AX12_TORQUE_ENABLE					0x18
#define AX12_LED							0x19
#define AX12_CW_COMPLIANCE_MARGIN			0x1A
#define AX12_CCW_COMPLIANCE_MARGIN			0x1B
#define AX12_CW_COMPLIANCE_SLOPE			0x1C
#define AX12_CCW_COMPLIANCE_SLOPE			0x1D
#define AX12_GOAL_POSITION_L				0x1E
#define AX12_GOAL_POSITION_H				0x1F
#define AX12_MOVING_SPEED_L					0x20
#define AX12_MOVING_SPEED_H					0x21
#define AX12_TORQUE_LIMIT_L					0x22
#define AX12_TORQUE_LIMIT_H					0x23
#define AX12_PRESENT_POSITION_L				0x24
#define AX12_PRESENT_POSITION_H				0x25
#define AX12_PRESENT_SPEED_L				0x26
#define AX12_PRESENT_SPEED_H				0x27
#define AX12_PRESENT_LOAD_L					0x28
#define AX12_PRESENT_LOAD_H					0x29
#define AX12_PRESENT_VOLTAGE				0x2A
#define AX12_PRESENT_TEMPERATURE			0x2B
#define AX12_REGISTERED_INSTRUCTION			0x2C
#define AX12_PAUSE_TIME						0x2D
#define AX12_MOVING							0x2E
#define AX12_LOCK							0x2F
#define AX12_PUNCH_L						0x30
#define AX12_PUNCH_H						0x31

#define AX12_DEGREES_PER_REVOLUTION			360
#define AX12_SECONDS_PER_MINUTE				60
#define AX12_UPDATES_PER_SECOND				128
#define AX12_DEGREE_TO_POSITION_UNIT_RATIO	.29 //According to Robotis documentation
#define AX12_RPM_TO_SPEED_UNIT_RATIO		.111	//According to Robotis documentation
#define AX12_DPM_TO_SPEED_UNIT_RATIO		AX12_RPM_TO_SPEED_UNIT_RATIO * AX12_DEGREES_PER_REVOLUTION
#define AX12_PPM_TO_SPEED_UNIT_RATIO		AX12_DPM_TO_SPEED_UNIT_RATIO / AX12_DEGREE_TO_POSITION_UNIT_RATIO
#define AX12_PPS_TO_SPEED_UNIT_RATIO		AX12_PPM_TO_SPEED_UNIT_RATIO / AX12_SECONDS_PER_MINUTE
#define AX12_PPU_TO_SPEED_UNIT_RATIO		AX12_PPS_TO_SPEED_UNIT_RATIO / AX12_UPDATES_PER_SECOND

typedef struct{	
	byte modelNumberL;
	byte modelNumberH;
	byte firmwareVersion;
	byte ID;
	byte baudRate;
	byte returnDelayTime;
	byte cwAngleLimitL;
	byte cwAngleLimitH;
	byte ccwAngleLimitL;
	byte ccwAngleLimitH;
	byte reserved1;
	byte highTempLimit;
	byte lowVoltageLimit;
	byte highVoltageLimit;
	byte maxTorqueL;
	byte maxTorqueH;
	byte statusReturnLevel;
	byte alarmLED;
	byte alarmShutdown;
	byte reserved2;
	byte downCalibrationL;
	byte downCalibrationH;
	byte upCalibrationL;
	byte upCalibrationH;
	byte torqueEnable;
	byte LED;
	byte cwComplianceMargin;
	byte ccwComplianceMargin;
	byte cwComplianceSlope;
	byte ccwComplianceSlope;
	volatile byte goalPositionL;
	volatile byte goalPositionH;
	volatile byte movingSpeedL;
	volatile byte movingSpeedH;
	volatile byte torqueLimitL;
	volatile byte torqueLimitH;
	volatile byte presentPositionL;
	volatile byte presentPositionH;
	volatile byte presentSpeedL;
	volatile byte presentSpeedH;
	volatile byte presentLoadL;
	volatile byte presentLoadH;
	byte presentVoltage;
	byte presentTemperature;
	byte registeredInstruction;
	byte pauseTime;
	byte moving;
	byte lock;
	byte punchL;
	byte punchH;
}AX12;

void dynamixelsAX12GetInfo(byte bID, AX12 *AX12, byte startPosition, byte endPosition);
void dynamixelsAX12GetInfoWholeTable(byte bID, AX12 *AX12);
void dynamixelsAX12GetInfoPositionSpeedLoad(AX12 *AX12);
void dynamixelsAX12GetInfoAllPresentPositionSpeedLoad(void);
void dynamixelsAX12SetInfo(AX12 *AX12);
void dynamixelsAX12SetSyncInfo(byte txParameters[], int txParametersLength);
void dynamixelsAX12SetSyncInfoSingle(AX12 *AX12, byte startPosition, byte endPosition);
void dynamixelsAX12SetSyncInfoAll(byte startPosition, byte endPosition);
void dynamixelsAX12SetSyncInfoAllGoalPositionSpeedTorque(void);
void dynamixelsAX12DisplayStats(AX12 *AX12);

#endif /* AX12FULL_H_ */