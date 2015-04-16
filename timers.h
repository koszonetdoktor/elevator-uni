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
extern void VelocityControlTimer(void);
extern void VelocityMeter(void);
extern void VelocityControlInt2(void);


int l_prev_signalcnt,l_signalcnt;
int r_prev_signalcnt,r_signalcnt;
int timer_period;
float left_v,right_v;



#endif /* TIMERS_H_ */
