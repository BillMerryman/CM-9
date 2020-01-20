/*
 * led.h
 *
 *  Created on: Dec 2, 2013
 *      Author: Bill
 */

#ifndef LED_H_
#define LED_H_

#include "common.h"

#define LED_GREEN		0x01

#define LED_OFF			0
#define LED_ON			1
#define LED_TOGGLE		2

void LEDInitialize(void);
void LEDSet(unsigned char LEDno, unsigned char state);

#endif /* LED_H_ */
