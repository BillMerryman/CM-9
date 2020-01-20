/*
 * protocol.h
 *
 *  Created on: Jun 21, 2016
 *      Author: Bill
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

enum PROTOCOL
{
	//Robotis existing instructions
	INST_PING = 0x01,
	INST_READ_DATA = 0x02,
	INST_WRITE_DATA = 0x03,
	INST_REG_WRITE = 0x04,
	INST_ACTION = 0x05,
	INST_RESET = 0x06,
	INST_DIGITAL_RESET = 0x07,
	INST_SYSTEM_READ = 0x0C,
	INST_SYSTEM_WRITE = 0x0D,
	INST_SYNC_WRITE = 0x83,
	INST_SYNC_REG_WRITE = 0x84,
	//Added instructions
	//controller level
	INST_WRITE_PAGE_SECTION_TO_BUFFER = 0xA0,
	INST_READ_PAGE_SECTION_FROM_BUFFER = 0xA1,
	INST_TRANSFER_PAGE_BUFFER_TO_FLASH = 0xA2,
	INST_TRANSFER_PAGE_BUFFER_TO_MOTION_PAGE = 0xA3,
	INST_TRANSFER_FLASH_TO_PAGE_BUFFER = 0xA4,
	INST_TRANSFER_FLASH_TO_MOTION_PAGE = 0xA5,
	INST_EXECUTE_MOTION_PAGE = 0xA6,
	INST_BREAK_MOTION_PAGE = 0xA7,
	INST_STOP_MOTION_PAGE = 0xA8,
	INST_GET_AX12_ATTACHED_COUNT = 0xB0,
	INST_GET_AX12_ATTACHED_IDS = 0xB1,
	INST_READ_AX12_IMAGE_IN_MEMORY = 0xB2,
	INST_WRITE_AX12_IMAGE_IN_MEMORY = 0xB3,
	INST_UPDATE_AX12_IMAGE_IN_MEMORY_FROM_DEVICE = 0xB4,
	INST_UPDATE_AX12_DEVICE_FROM_IMAGE_IN_MEMORY = 0xB5,
	INST_UPDATE_ALL_AX12_IMAGES_IN_MEMORY_FROM_DEVICES = 0xB6,
	INST_UPDATE_ALL_AX12_DEVICES_FROM_IMAGES_IN_MEMORY = 0xB7,
	INST_GET_AXS1_ATTACHED_COUNT = 0xC0,
	INST_GET_AXS1_ATTACHED_IDS = 0xC1,
	INST_READ_AXS1_IMAGE_IN_MEMORY = 0xC2,
	INST_WRITE_AXS1_IMAGE_IN_MEMORY = 0xC3,
	INST_UPDATE_AXS1_IMAGE_IN_MEMORY_FROM_DEVICE = 0xC4,
	INST_UPDATE_AXS1_DEVICE_FROM_IMAGE_IN_MEMORY = 0xC5,
	INST_UPDATE_ALL_AXS1_IMAGES_IN_MEMORY_FROM_DEVICES = 0xC6,
	INST_UPDATE_ALL_AXS1_DEVICES_FROM_IMAGES_IN_MEMORY = 0xC7
};

#endif /* PROTOCOL_H_ */
