/*
 * bridge.c
 *
 *  Created on: Mar 26, 2017
 *      Author: Bill
 */
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

void bridgeProcess()
{
	uint8_t requestParametersLength;

	if(USBRxIsPacketReady(&requestParametersLength))
	{
		uint8_t requestParameters[requestParametersLength];
		uint8_t ID;
		uint8_t instruction;

		USBRxPacket(&ID, &instruction, requestParameters, requestParametersLength);

		switch(instruction)
		{
			case INST_PING:
				bridgePing(ID);
				break;
			case INST_READ_DATA:
				bridgeReadData(ID, requestParameters, requestParametersLength);
				break;
			case INST_WRITE_DATA:
				bridgeWriteData(ID, requestParameters, requestParametersLength);
				break;
			case INST_REG_WRITE:
				bridgeRegWrite();
				break;
			case INST_ACTION:
				bridgeAction();
				break;
			case INST_RESET:
				bridgeReset();
				break;
			case INST_DIGITAL_RESET:
				bridgeDigitalReset();
				break;
			case INST_SYSTEM_READ:
				bridgeSystemRead();
				break;
			case INST_SYSTEM_WRITE:
				bridgeSystemWrite();
				break;
			case INST_SYNC_WRITE:
				bridgeSyncWrite();
				break;
			case INST_SYNC_REG_WRITE:
				bridgeSyncRegWrite();
				break;
			case INST_WRITE_PAGE_SECTION_TO_BUFFER:
				bridgeWritePageSectionToBuffer(requestParameters, requestParametersLength);
				break;
			case INST_READ_PAGE_SECTION_FROM_BUFFER:
				bridgeReadPageSectionFromBuffer(requestParameters, requestParametersLength);
				break;
			case INST_TRANSFER_PAGE_BUFFER_TO_FLASH:
				bridgeTransferPageBufferToFlash(requestParameters, requestParametersLength);
				break;
			case INST_TRANSFER_PAGE_BUFFER_TO_MOTION_PAGE:
				bridgeTransferPageBufferToMotionPage(requestParameters, requestParametersLength);
				break;
			case INST_TRANSFER_FLASH_TO_PAGE_BUFFER:
				bridgeTransferFlashToPageBuffer(requestParameters, requestParametersLength);
				break;
			case INST_TRANSFER_FLASH_TO_MOTION_PAGE:
				bridgeTransferFlashToMotionPage(requestParameters, requestParametersLength);
				break;
			case INST_EXECUTE_MOTION_PAGE:
				bridgeExecuteMotionPage(requestParameters, requestParametersLength);
				break;
			case INST_BREAK_MOTION_PAGE:
				bridgeBreakMotionPage();
				break;
			case INST_GET_AX12_ATTACHED_COUNT:
				bridgeGetAX12AttachedCount();
				break;
			case INST_GET_AX12_ATTACHED_IDS:
				bridgeGetAX12AttachedIDs();
				break;
			case INST_READ_AX12_IMAGE_IN_MEMORY:
				bridgeReadAX12ImageInMemory(ID);
				break;
			case INST_WRITE_AX12_IMAGE_IN_MEMORY:
				bridgeWriteAX12ImageInMemory(ID, requestParameters, requestParametersLength);
				break;
			case INST_UPDATE_AX12_IMAGE_IN_MEMORY_FROM_DEVICE:
				bridgeUpdateAX12ImageInMemoryFromDevice(ID);
				break;
			case INST_UPDATE_AX12_DEVICE_FROM_IMAGE_IN_MEMORY:
				break;
			case INST_UPDATE_ALL_AX12_IMAGES_IN_MEMORY_FROM_DEVICES:
				break;
			case INST_UPDATE_ALL_AX12_DEVICES_FROM_IMAGES_IN_MEMORY:
				break;
			case INST_READ_AXS1_IMAGE_IN_MEMORY:
				break;
		}
	}
}

void bridgePing(uint8_t bID)
{
	uint8_t errorNo;

	if(dynamixelsPing(bID, &errorNo))
	{
		USBTxPacket(HOST_ID, errorNo, NULL, 0);
	}
}

void bridgeReadData(uint8_t bID, uint8_t *requestParameters, uint8_t requestParametersLength)
{
	uint8_t errorNo;

	uint8_t responseParametersLength = requestParameters[1];
	uint8_t responseParameters[responseParametersLength];
	if(dynamixelsReadData(bID, &errorNo, requestParameters, requestParametersLength, responseParameters, responseParametersLength))
	{
		USBTxPacket(HOST_ID, errorNo, responseParameters, responseParametersLength);
	}
}

void bridgeWriteData(uint8_t bID, uint8_t *requestParameters, uint8_t requestParametersLength)
{
	uint8_t errorNo;

	if(dynamixelsWriteData(bID, &errorNo, requestParameters, requestParametersLength))
	{
		USBTxPacket(HOST_ID, errorNo, NULL, 0);
	}
}

void bridgeRegWrite()
{
}

void bridgeAction()
{
}

void bridgeReset()
{
}

void bridgeDigitalReset()
{
}

void bridgeSystemRead()
{
}

void bridgeSystemWrite()
{
}

void bridgeSyncWrite()
{
}

void bridgeSyncRegWrite()
{
}
/*
 * for the following functions, we need to check the parameters coming in, and send back
 * appropriate messages if the request is bad, or if one of the calls fails...
 */
void bridgeWritePageSectionToBuffer(uint8_t *requestParameters, uint8_t requestParametersLength)
{
	motionWritePageSectionToPageBuffer(requestParameters[0], &requestParameters[1]);
	USBTxPacket(HOST_ID, USB_ERROR_NONE, NULL, 0);
}

void bridgeReadPageSectionFromBuffer(uint8_t *requestParameters, uint8_t requestParametersLength)
{
	uint8_t motionSection[MOTION_PAGE_SECTION_LENGTH];
	motionReadPageSectionFromPageBuffer(requestParameters[0], motionSection);
	USBTxPacket(HOST_ID, USB_ERROR_NONE, motionSection, sizeof(motionSection));
}

void bridgeTransferPageBufferToFlash(uint8_t *requestParameters, uint8_t requestParametersLength)
{
	motionTransferPageBufferToFlash(requestParameters[0]);
	USBTxPacket(HOST_ID, USB_ERROR_NONE, NULL, 0);
}

void bridgeTransferPageBufferToMotionPage(uint8_t *requestParameters, uint8_t requestParametersLength)
{
	motionTransferPageBufferToMotionPage();
	USBTxPacket(HOST_ID, USB_ERROR_NONE, NULL, 0);
}

void bridgeTransferFlashToPageBuffer(uint8_t *requestParameters, uint8_t requestParametersLength)
{
	motionTransferFlashToPageBuffer(requestParameters[0]);
	USBTxPacket(HOST_ID, USB_ERROR_NONE, NULL, 0);
}

void bridgeTransferFlashToMotionPage(uint8_t *requestParameters, uint8_t requestParametersLength)
{
	motionTransferFlashToMotionPage(requestParameters[0]);
	USBTxPacket(HOST_ID, USB_ERROR_NONE, NULL, 0);
}

void bridgeExecuteMotionPage(uint8_t *requestParameters, uint8_t requestParametersLength)
{
	motionExecuteMotion(requestParameters[0], 0, 0);
	USBTxPacket(HOST_ID, USB_ERROR_NONE, NULL, 0);
}

void bridgeBreakMotionPage(void)
{
	motionSceneBreak();
	USBTxPacket(HOST_ID, USB_ERROR_NONE, NULL, 0);
}

void bridgeGetAX12AttachedCount(void)
{
	uint8_t responseParametersLength = 1;
	uint8_t responseParameters[responseParametersLength];
	responseParameters[0] = AX12sGetCount();
	USBTxPacket(HOST_ID, USB_ERROR_NONE, responseParameters, responseParametersLength);
}

void bridgeGetAX12AttachedIDs(void)
{
	uint8_t responseParametersLength = AX12sGetCount();
	if(responseParametersLength > 0)
	{
		uint8_t responseParameters[responseParametersLength];
		for(uint8_t counter = 0; counter < responseParametersLength; counter++)
		{
			responseParameters[counter] = AX12GetID(counter);
		}
		USBTxPacket(HOST_ID, USB_ERROR_NONE, responseParameters, responseParametersLength);
	}
	else
	{
		USBTxPacket(HOST_ID, USB_ERROR_NONE, NULL, 0);
	}
}

void bridgeReadAX12ImageInMemory(uint8_t bID)
{
	uint8_t responseParametersLength = sizeof(AX12);
	uint8_t responseParameters[responseParametersLength];
	if(AX12ReadAX12ImageInMemory(bID, responseParameters))
	{
		USBTxPacket(HOST_ID, USB_ERROR_NONE, responseParameters, responseParametersLength);
	}
	else
	{
		USBTxPacket(HOST_ID, USB_REQUEST_FAILED, NULL, 0);
	}
}

void bridgeWriteAX12ImageInMemory(uint8_t bID, uint8_t *requestParameters, uint8_t requestParametersLength)
{
	if(AX12WriteAX12ImageInMemory(bID, requestParameters))
	{
		USBTxPacket(HOST_ID, USB_ERROR_NONE, NULL, 0);
	}
	else
	{
		USBTxPacket(HOST_ID, USB_REQUEST_FAILED, NULL, 0);
	}
}

void bridgeUpdateAX12ImageInMemoryFromDevice(uint8_t bID)
{
	if(AX12UpdateAX12ImageInMemoryFromDevice(bID))
	{
		USBTxPacket(HOST_ID, USB_ERROR_NONE, NULL, 0);
	}
	else
	{
		USBTxPacket(HOST_ID, USB_REQUEST_FAILED, NULL, 0);
	}
}

void bridgeUpdateAX12DeviceFromImageInMemory(uint8_t bID)
{
	if(AX12UpdateAX12DeviceFromImageInMemory(bID))
	{
		USBTxPacket(HOST_ID, USB_ERROR_NONE, NULL, 0);
	}
	else
	{
		USBTxPacket(HOST_ID, USB_REQUEST_FAILED, NULL, 0);
	}
}

void bridgeUpdateAllAX12ImagesInMemoryFromDevices(void)
{
	AX12UpdateAllAX12ImagesInMemoryFromDevices();
}

void bridgeUpdateAllAX12DevicesFromImagesInMemory(void)
{
	AX12UpdateAllAX12DevicesFromImagesInMemory();
}

void bridgeGetAXS1AttachedCount(void)
{
	uint8_t responseParametersLength = 1;
	uint8_t responseParameters[responseParametersLength];
	responseParameters[0] = AXS1sGetCount();
	USBTxPacket(HOST_ID, USB_ERROR_NONE, responseParameters, responseParametersLength);
}

void bridgeGetAXS1AttachedIDs(void)
{
	uint8_t responseParametersLength = AXS1sGetCount();
	if(responseParametersLength > 0)
	{
		uint8_t responseParameters[responseParametersLength];
		for(uint8_t counter = 0; counter < responseParametersLength; counter++)
		{
			responseParameters[counter] = AXS1GetID(counter);
		}
		USBTxPacket(HOST_ID, USB_ERROR_NONE, responseParameters, responseParametersLength);
	}
	else
	{
		USBTxPacket(HOST_ID, USB_ERROR_NONE, NULL, 0);
	}
}

void bridgeReadAXS1ImageInMemory(uint8_t bID)
{
	uint8_t responseParametersLength = sizeof(AXS1);
	uint8_t responseParameters[responseParametersLength];
	if(AXS1ReadAXS1ImageInMemory(bID, responseParameters))
	{
		USBTxPacket(HOST_ID, USB_ERROR_NONE, responseParameters, responseParametersLength);
	}
	else
	{
		USBTxPacket(HOST_ID, USB_REQUEST_FAILED, NULL, 0);
	}
}

void bridgeWriteAXS1ImageInMemory(uint8_t bID, uint8_t *requestParameters, uint8_t requestParametersLength)
{
	if(AXS1WriteAXS1ImageInMemory(bID, requestParameters))
	{
		USBTxPacket(HOST_ID, USB_ERROR_NONE, NULL, 0);
	}
	else
	{
		USBTxPacket(HOST_ID, USB_REQUEST_FAILED, NULL, 0);
	}
}

void bridgeUpdateAXS1ImageInMemoryFromDevice(uint8_t bID)
{
	if(AXS1UpdateAXS1ImageInMemoryFromDevice(bID))
	{
		USBTxPacket(HOST_ID, USB_ERROR_NONE, NULL, 0);
	}
	else
	{
		USBTxPacket(HOST_ID, USB_REQUEST_FAILED, NULL, 0);
	}
}

void bridgeUpdateAXS1DeviceFromImageInMemory(uint8_t bID)
{
	if(AXS1UpdateAXS1DeviceFromImageInMemory(bID))
	{
		USBTxPacket(HOST_ID, USB_ERROR_NONE, NULL, 0);
	}
	else
	{
		USBTxPacket(HOST_ID, USB_REQUEST_FAILED, NULL, 0);
	}
}

void bridgeUpdateAllAXS1ImagesInMemoryFromDevices(void)
{
	AXS1UpdateAllAXS1ImagesInMemoryFromDevices();
}

void bridgeUpdateAllAXS1DevicesFromImagesInMemory(void)
{
	AXS1UpdateAllAXS1DevicesFromImagesInMemory();
}
