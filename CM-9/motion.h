/*
 * motion.h
 *
 *  Created on: Dec 22, 2013
 *      Author: Bill
 */

#ifndef MOTION_H_
#define MOTION_H_

#include "common.h"

#define FLASH_PAGE_SIZE_IN_MOTION_PAGES		2

#define MOTION_PAGE_SECTION_COUNT			8
#define MOTION_PAGE_SECTION_LENGTH			64
//#define MOTION_PAGE_SECTION_COUNT			16
//#define MOTION_PAGE_SECTION_LENGTH		32

#define MOTION_PAGES_BASE_ADDRESS			(0x8013000) //Adjusted for 128K CM9 flash, may want to adjust further
#define	MOTION_POSE_0_OFFSET				(0x00040)
#define MOTION_POSE_SIZE					(0x00040)
#define MOTION_POSES_PER_PAGE				7

// within a page
#define MOTION_PAGE_SIZE					(0x200)
#define MOTION_FAST_FLAG					(0x00010)
#define MOTION_PAGE_COUNT					(0x0000F)
#define MOTION_NUM_OF_MOTIONS				(0x00014)
#define MOTION_SPEED						(0x00016)
#define MOTION_ACCEL_TIME					(0x00018)
#define MOTION_NEXT_PAGE					(0x00019)
#define MOTION_EXIT_PAGE					(0x0001A)

// Within a pose
#define POSE_PAUSE_ADR						0x0003E
#define POSE_SPEED_ADR						0x0003F

#define	SERVO_NOT_CONNECTED					9999

#define MOTION_UPDATES_PER_SECOND			128
#define MOTION_MAX_SECONDS_PER_POSE			112
#define MOTION_MAX_PAGE_SPEED				255
#define MOTION_MAX_POSE_SPEED				255
#define MOTION_MAX_UPDATES_PER_POSE			MOTION_UPDATES_PER_SECOND*MOTION_MAX_SECONDS_PER_POSE

#define MOTION_MAX_SECONDS_PER_POSE_TO_PAGE_POSE_SPEED_RATIO (((float)MOTION_MAX_SECONDS_PER_POSE/(float)MOTION_MAX_POSE_SPEED)/(float)MOTION_MAX_PAGE_SPEED)
#define MOTION_MAX_UPDATES_PER_POSE_TO_PAGE_POSE_SPEED_RATIO (((float)MOTION_MAX_UPDATES_PER_POSE/(float)MOTION_MAX_POSE_SPEED)/(float)MOTION_MAX_PAGE_SPEED)

typedef struct{
	uint16_t posData[31];
	uint8_t delay;
	uint8_t speed;
}MOTION_POSE;

typedef struct{
	char name[14];
	uint8_t unidentifiedByte0;
	uint8_t playCount;
	uint8_t schedule;
	uint8_t res1[3];
	uint8_t poseCount;
	uint8_t unidentifiedByte1;
	uint8_t pageSpeed;
	uint8_t dxlSetup;
	uint8_t accelTime;
	uint8_t nextPage;
	uint8_t exitPage;
	uint8_t linkedPage1;
	uint8_t linkedPage1PlayCode;
	uint8_t linkedPage2;
	uint8_t linkedPage2PlayCode;
	uint8_t checkSum;
	uint8_t slope[31];
	uint8_t unidentifiedByte2;
} MOTION_PAGE_HEADER;

typedef struct{
	MOTION_PAGE_HEADER header;
	MOTION_POSE poses[7];
} MOTION_PAGE;

typedef struct{
	unsigned short startingPositionPlayingPose;
    unsigned short targetAnglePlayingPose;
    short int totalPoseOffset;
    short int mainSectionOffset;
    short int accelerationSectionOffset;
    short int movementUPU;
    short int LastSectionCompletedUPU;
    short int inLoopRecordedUPU;
    unsigned char bpFinishType;
} motionComponents;

typedef enum{
	PRE_ACCELERATION_SECTION,
	MAIN_SECTION,
	POST_ACCELERATION_SECTION,
	PAUSE_SECTION
} sectionType;

typedef enum{
	ZERO_FINISH,
	NON_ZERO_FINISH
} finishType;

typedef enum{
	SPEED_BASE_SCHEDULE = 0,
	TIME_BASE_SCHEDULE = 0x0a
} scheduleType;

typedef enum{
	INVALID_BIT_MASK	= 0x4000,
	TORQUE_OFF_BIT_MASK	= 0x2000
} servoState;

void motionInitialize(void);

bool motionWritePageSectionToPageBuffer(uint8_t sectionNumber, uint8_t *motionSection);
bool motionReadPageSectionFromPageBuffer(uint8_t sectionNumber, uint8_t *motionSection);

//these 4 functions handle all of the transfers that need to happen internally,
//writing a buffer to flash, writing flash to a buffer, writing flash to the motion
//page, and writing the page buffer to the motion page.
bool motionTransferPageBufferToFlash(uint8_t pageNumber);
bool motionTransferFlashToPageBuffer(uint8_t pageNumber);
bool motionTransferFlashToMotionPage(uint8_t pageNumber);
bool motionTransferPageBufferToMotionPage(void);

bool motionGetFlashMotionPage(uint8_t pageNumber, MOTION_PAGE *page);

bool motionExecuteMotion(uint8_t pageNumber, uint8_t startPose, uint8_t endPose);

bool motionScenePlaying(void);
void motionSceneStop(void);
void motionSceneBreak(void);
void motionProcess(void);

void displayPageStats(MOTION_PAGE *motionPage);

#endif /* MOTION_H_ */
