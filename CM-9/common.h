/*
 * common.h
 *
 * Created: 11/19/2013 5:38:10 PM
 *  Author: Bill
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include "stm32f10x_lib.h"

#define CONTROLLER_DEBUG

#ifdef CONTROLLER_DEBUG
#define MAIN_DEBUG
#define MOTION_DEBUG
#define RS485_DEBUG
#define RS485_ENABLE_WARNINGS
#define DYNAMIXEL_DEBUG
#define DYNAMIXEL_ENABLE_WARNINGS
#define DYNAMIXEL_ENABLE_MISSING_WARNING
#define BUTTONS_DEBUG
#define CLOCK_DEBUG
#endif

#define HOST_ID								200

#endif /* COMMON_H_ */
