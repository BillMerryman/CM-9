/*
 * clock.h
 *
 * Created: 11/19/2013 4:48:19 PM
 *  Author: Bill
 */ 


#ifndef CLOCK_H_
#define CLOCK_H_

#include "common.h"

#define CLOCK_2_PRESCALER				56249	 //scale down to 1280 Hz
#define CLOCK_2_PERIOD					10		 //for 128 interrupts per second
#define CLOCK_2_PREEMPTION_PRIORITY		2
#define CLOCK_2_SUB_PRIORITY			2
#define CLOCK_3_PRESCALER				7200	 //scale down to 10000 Hz
#define CLOCK_3_PERIOD					100		 //for 100 milliseconds
#define CLOCK_3_PREEMPTION_PRIORITY		1
#define CLOCK_3_SUB_PRIORITY			1

void clocksInitialize(void);
void clock1Initialize(void);
void clock2Initialize(void);
void clock3Initialize(void);
void clock4Initialize(void);

void clock2Start(void);
void clock2Stop(void);
bool clock2IsExpired(void);
void clock3Start(void);
void clock3Stop(void);
bool clock3IsExpired(void);
void clock4Start(void);
void clock4Stop(void);
bool clock4IsExpired(void);

#endif /* CLOCK_H_ */
