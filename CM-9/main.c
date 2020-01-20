#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include "common.h"
#include "system.h"
#include "led.h"
#include "clock.h"
#include "protocol.h"
#include "USB.h"
#include "bluetooth.h"
#include "RS485.h"
#include "dynamixels.h"
#include "AX12.h"
#include "AXS1.h"
#include "motion.h"
#include "bridge.h"

int main(void)
{

	systemInitialize();
	LEDInitialize();
	RS485Initialize();
	USBInitialize();
	bluetoothInitialize();
	clock3Initialize();
	AX12sInitialize();
	AXS1sInitialize();
	motionInitialize();
	clock2Initialize();

	while(TRUE)
	{
		if(clock2IsExpired())
		{
			motionProcess();
			if(motionScenePlaying()) AX12SetSyncInfoAll(AX12_CW_COMPLIANCE_SLOPE, AX12_GOAL_POSITION_H);
			AXS1GetInfoAll(AXS1_LEFT_IR_SENSOR_DATA, AXS1_RIGHT_IR_SENSOR_DATA);
		}
		bridgeProcess();
	}
	return 0;

}
