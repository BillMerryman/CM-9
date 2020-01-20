/*
 * bridge.h
 *
 *  Created on: Mar 26, 2017
 *      Author: Bill
 */

#ifndef BRIDGE_H_
#define BRIDGE_H_

void bridgeProcess(void);

void bridgePing(uint8_t bID);
void bridgeReadData(uint8_t bID, uint8_t *requestParameters, uint8_t requestParametersLength);
void bridgeWriteData(uint8_t bID, uint8_t *requestParameters, uint8_t requestParametersLength);
void bridgeRegWrite();
void bridgeAction();
void bridgeReset();
void bridgeDigitalReset();
void bridgeSystemRead();
void bridgeSystemWrite();
void bridgeSyncWrite();
void bridgeSyncRegWrite();
void bridgeWritePageSectionToBuffer(uint8_t *requestParameters, uint8_t requestParametersLength);
void bridgeReadPageSectionFromBuffer(uint8_t *requestParameters, uint8_t requestParametersLength);
void bridgeTransferPageBufferToFlash(uint8_t *requestParameters, uint8_t requestParametersLength);
void bridgeTransferPageBufferToMotionPage(uint8_t *requestParameters, uint8_t requestParametersLength);
void bridgeTransferFlashToPageBuffer(uint8_t *requestParameters, uint8_t requestParametersLength);
void bridgeTransferFlashToMotionPage(uint8_t *requestParameters, uint8_t requestParametersLength);
void bridgeExecuteMotionPage(uint8_t *requestParameters, uint8_t requestParametersLength);
void bridgeBreakMotionPage(void);
void bridgeGetAX12AttachedCount(void);
void bridgeGetAX12AttachedIDs(void);
void bridgeReadAX12ImageInMemory(uint8_t bID);
void bridgeWriteAX12ImageInMemory(uint8_t bID, uint8_t *requestParameters, uint8_t parametersLength);
void bridgeUpdateAX12ImageInMemoryFromDevice(uint8_t bID);
void bridgeUpdateAX12DeviceFromImageInMemory(uint8_t bID);
void bridgeUpdateAllAX12ImagesInMemoryFromDevices(void);
void bridgeUpdateAllAX12DevicesFromImagesInMemory(void);
void bridgeGetAXS1AttachedCount(void);
void bridgeGetAXS1AttachedIDs(void);
void bridgeReadAXS1ImageInMemory(uint8_t bID);
void bridgeWriteAXS1ImageInMemory(uint8_t bID, uint8_t *requestParameters, uint8_t parametersLength);
void bridgeUpdateAXS1ImageInMemoryFromDevice(uint8_t bID);
void bridgeUpdateAXS1DeviceFromImageInMemory(uint8_t bID);
void bridgeUpdateAllAXS1ImagesInMemoryFromDevices(void);
void bridgeUpdateAllAXS1DevicesFromImagesInMemory(void);

#endif /* BRIDGE_H_ */
