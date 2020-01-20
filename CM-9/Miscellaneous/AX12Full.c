/*
 * AX12Full.c
 *
 * Created: 1/12/2013 11:12:38 PM
 *  Author: MAIN
 */

#include "AX12Full.h" 

void dynamixelsAX12GetInfo(byte bID, AX12 *AX12, byte startPosition, byte endPosition)
{
	
	int positionCount=(endPosition-startPosition)+1;
	
	byte txParameters[2];
	txParameters[0]=startPosition;
	txParameters[1]=positionCount;
	RS485TxPacket(bID,RS485_INST_READ_DATA,txParameters,2);
	byte receiveError=RS485RxPacket(bID,(byte *)AX12+startPosition,positionCount);
	
	#ifdef DYNAMIXEL_DEBUG
		if(receiveError==0){
			//char outputMessage[10]; 
			//sprintf(outputMessage,"SH:%uSL:%u\n\r",AX12->presentSpeedH, AX12->presentSpeedL);
			//RS232SendString(outputMessage);
		}else{
			RS232SendString("Receive Error.\r\n");
		}
	#endif
	
}

void dynamixelsAX12GetInfoWholeTable(byte bID, AX12 *AX12)
{
	
	dynamixelsAX12GetInfo(bID, AX12, DYNAMIXEL_MODEL_NUMBER_L, DYNAMIXEL_PUNCH_H);

}

void dynamixelsAX12GetInfoPositionSpeedLoad(AX12 *AX12)
{
	
	dynamixelsAX12GetInfo(AX12->ID, AX12, DYNAMIXEL_PRESENT_POSITION_L, DYNAMIXEL_PRESENT_LOAD_H);

}

void dynamixelsAX12GetInfoAllPresentPositionSpeedLoad(void)
{
	
	for(int counter=0;counter<AX12Count;counter++){
		dynamixelsAX12GetInfoPositionSpeedLoad(&AX12s[counter]);		
	}
	
}

void dynamixelsAX12SetInfo(AX12 *AX12)
{
	//do a regular write here and read the result
}

void dynamixelsAX12SetSyncInfo(byte txParameters[], int txParametersLength)
{

	RS485TxPacket(DYNAMIXEL_BROADCASTING_ID, RS485_INST_SYNC_WRITE, txParameters, txParametersLength);

}

void dynamixelsAX12SetSyncInfoSingle(AX12 *AX12, byte startPosition, byte endPosition)
{
	
	//This function needs testing
	
	int positionCount=(endPosition-startPosition)+1;
	int frameSize=DYNAMIXEL_ID_WIDTH+positionCount;
	int txParametersLength=DYNAMIXEL_SYNC_STARTING_ADDRESS_WIDTH+DYNAMIXEL_SYNC_LENGTH_OF_DATA_WIDTH+frameSize;
	byte txParameters[txParametersLength];
	//populate
	txParameters[DYNAMIXEL_SYNC_STARTING_ADDRESS_POSITION]=startPosition;
	txParameters[DYNAMIXEL_SYNC_LENGTH_OF_DATA_POSITION]=positionCount;

	txParameters[DYNAMIXEL_SYNC_STARTING_ADDRESS_WIDTH+DYNAMIXEL_SYNC_LENGTH_OF_DATA_WIDTH]=AX12->ID;
	memcpy((void *)(txParameters+DYNAMIXEL_SYNC_STARTING_ADDRESS_WIDTH+DYNAMIXEL_SYNC_LENGTH_OF_DATA_WIDTH+DYNAMIXEL_ID_WIDTH), (void *)((byte *)AX12+startPosition), positionCount);
	
	#ifdef DYNAMIXEL_DEBUG
		//char outputMessage[5];
		//for(int counter=0;counter<txParametersLength;counter++){
			//sprintf(outputMessage,"%u ",txParameters[counter]);
			//RS232SendString(outputMessage);
		//}
		//RS232SendString("\r\n");
	#endif
	
	dynamixelsAX12SetSyncInfo(txParameters, txParametersLength);
	
}

void dynamixelsAX12SetSyncInfoAll(byte startPosition, byte endPosition)
{
	
	int positionCount=(endPosition-startPosition)+1;
	int frameSize=DYNAMIXEL_ID_WIDTH+positionCount;
	//calculate size of array
	int txParametersLength=DYNAMIXEL_SYNC_STARTING_ADDRESS_WIDTH+DYNAMIXEL_SYNC_LENGTH_OF_DATA_WIDTH+(AX12Count*frameSize);
	byte txParameters[txParametersLength];
	//populate
	txParameters[DYNAMIXEL_SYNC_STARTING_ADDRESS_POSITION]=startPosition;
	txParameters[DYNAMIXEL_SYNC_LENGTH_OF_DATA_POSITION]=positionCount;
	for(int counter=0;counter<AX12Count;counter++){
		txParameters[DYNAMIXEL_SYNC_STARTING_ADDRESS_WIDTH+DYNAMIXEL_SYNC_LENGTH_OF_DATA_WIDTH+(counter*frameSize)]=AX12s[counter].ID;
		memcpy((void *)(txParameters+(DYNAMIXEL_SYNC_STARTING_ADDRESS_WIDTH+DYNAMIXEL_SYNC_LENGTH_OF_DATA_WIDTH)+(counter*frameSize)+DYNAMIXEL_ID_WIDTH), (void *)(((byte *)&AX12s[counter])+startPosition), positionCount);
	}
	
	dynamixelsAX12SetSyncInfo(txParameters, txParametersLength);
	
}

void dynamixelsAX12SetSyncInfoAllGoalPositionSpeedTorque(void)
{
	
	dynamixelsAX12SetSyncInfoAll(DYNAMIXEL_GOAL_POSITION_L, DYNAMIXEL_TORQUE_LIMIT_H);
	
}


void dynamixelsAX12DisplayStats(AX12 *AX12)
{

	char outputMessage[30];

	sprintf(outputMessage, "Model Number Low: %u\r\n", AX12->modelNumberL);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Model Number High: %u\r\n", AX12->modelNumberH);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Firmware version: %u\r\n", AX12->firmwareVersion);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "ID: %u\r\n", AX12->ID);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Baud rate: %u\r\n", AX12->baudRate);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Return Delay Time: %u\r\n", AX12->returnDelayTime);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "CW Angle Limit Low: %u\r\n", AX12->cwAngleLimitL);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "CW Angle Limit High: %u\r\n", AX12->cwAngleLimitH);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "CCW Angle Limit Low: %u\r\n", AX12->ccwAngleLimitL);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "CCW Angle Limit High: %u\r\n", AX12->ccwAngleLimitH);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Reserved 1: %u\r\n", AX12->reserved1);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "High Temp Limit: %u\r\n", AX12->highTempLimit);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Low Voltage Limit: %u\r\n", AX12->lowVoltageLimit);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "High Voltage Limit: %u\r\n", AX12->highVoltageLimit);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Max Torque Low: %u\r\n", AX12->maxTorqueL);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Max Torque High: %u\r\n", AX12->maxTorqueH);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Status Return Level: %u\r\n", AX12->statusReturnLevel);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Alarm LED: %u\r\n", AX12->alarmLED);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Alarm Shutdown: %u\r\n", AX12->alarmShutdown);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Reserved 2: %u\r\n", AX12->reserved2);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Down Calibration Low: %u\r\n", AX12->downCalibrationL);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Down Calibration High: %u\r\n", AX12->downCalibrationH);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Up Calibration Low: %u\r\n", AX12->upCalibrationL);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Up Calibration High: %u\r\n", AX12->upCalibrationH);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Torque Enable: %u\r\n", AX12->torqueEnable);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "LED: %u\r\n", AX12->LED);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "CW Compliance Margin: %u\r\n", AX12->cwComplianceMargin);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "CCW Compliance Margin: %u\r\n", AX12->ccwComplianceMargin);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "CW Compliance Slope: %u\r\n", AX12->cwComplianceSlope);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "CCW Compliance Slope: %u\r\n", AX12->ccwComplianceSlope);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Goal Position Low: %u\r\n", AX12->goalPositionL);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Goal Position High: %u\r\n", AX12->goalPositionH);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Goal Speed Low: %u\r\n", AX12->movingSpeedL);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Goal Speed High: %u\r\n", AX12->movingSpeedH);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Torque Limit Low: %u\r\n", AX12->torqueLimitL);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Torque Limit High: %u\r\n", AX12->torqueLimitH);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Present Position Low: %u\r\n", AX12->presentPositionL);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Present Position High: %u\r\n", AX12->presentPositionH);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Present Speed Low: %u\r\n", AX12->presentSpeedL);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Present Speed High: %u\r\n", AX12->presentSpeedH);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Present Load Low: %u\r\n", AX12->presentLoadL);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Present Load High: %u\r\n", AX12->presentLoadH);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Present Voltage: %u\r\n", AX12->presentVoltage);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Present Temperature: %u\r\n", AX12->presentTemperature);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Registered Instruction: %u\r\n", AX12->registeredInstruction);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Pause Time: %u\r\n", AX12->pauseTime);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Moving: %u\r\n", AX12->moving);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Lock: %u\r\n", AX12->lock);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Punch Low: %u\r\n", AX12->punchL);
	RS232SendString(outputMessage);
	sprintf(outputMessage, "Punch High: %u\r\n", AX12->punchH);
	RS232SendString(outputMessage);
			
	RS232SendString("\r\n");
	
}