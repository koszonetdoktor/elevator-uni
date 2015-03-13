/*
 * timers.h
 *
 *  Created on: 2015.03.02.
 *      Author: Elek
 */

#ifndef TIMERS_H_
#define TIMERS_H_

extern void Timer0IntHandler(void);
extern void VelocityTimerInit(void);

float left_desired_v;
float right_desired_v;
unsigned char left_dir,right_dir;


#endif /* TIMERS_H_ */
