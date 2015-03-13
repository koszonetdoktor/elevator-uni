/*
 * timers.c
 *
 *  Created on: 2015.03.02.
 *      Author: Elek
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "math.h"
#include "motion.h"
#include "sensors.h"
#include "structs.h"

int l_prev_signalcnt,l_signalcnt,left_v;
int r_prev_signalcnt,r_signalcnt,right_v;
int timer_period;
float left_desired_v,right_desired_v;
unsigned char left_dir, right_dir ;

void Timer0IntHandler(void){
	float x;
	//left velocity
	l_signalcnt = left.sign_counter;
	left_v = l_signalcnt-l_prev_signalcnt; // thsi determines the velocity from the previous and the actual velocity
	//if(left_v != 0){UARTprintf("left_v: %d\n",left_v);}
	l_prev_signalcnt = l_signalcnt;

	//right velocity
	r_signalcnt = right.sign_counter;
	right_v = r_signalcnt-r_prev_signalcnt;
	//if(right_v != 0){UARTprintf("right_v: %d\n",right_v);}
	r_prev_signalcnt = r_signalcnt;

	/* *********************************************************************
	 * Control, This part contorls the velocity to keep the desired level (kell pid....)
	 ***********************************************************************/
	if(left_v > 0){ // this eliminates the possibility of devide with zero
		x = (left_desired_v/left_v); // this gives the magnitude of the difference between the desired and the real velocity
		if(x > 2){ // thsi is for smoothen the accelaration (avoid jumping)
			x = 1.1;
		}
		if(left_dir == UPWARD){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 ,(PWMPulseWidthGet(PWM0_BASE, PWM_OUT_1) * x));
		}
		if(left_dir == DOWNWARD){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 ,(PWMPulseWidthGet(PWM0_BASE, PWM_OUT_0) * x));
		}
	}
	if(right_v > 0){
		x = (right_desired_v/right_v); // this gives the magnitude of the difference between the desired and the real velocity
		if(x > 2){ // thsi is for smoothen the accelaration (avoid jumping)
			x = 1.1;
		}
		if(right_dir == UPWARD){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 ,(PWMPulseWidthGet(PWM0_BASE, PWM_OUT_2) * x));
		}
		if(right_dir == DOWNWARD){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 ,(PWMPulseWidthGet(PWM0_BASE, PWM_OUT_3) * x));
		}
	}
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
} // enf of Timer0IntHandler()

void VelocityControlInt(void){
	UARTprintf("d\n");
}



void VelocityTimerInit(void){
	timer_period = ((SysCtlClockGet()/1));  // this means 100 ms, 10 Hz

	TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER0_BASE, TIMER_A, timer_period);
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	//Enable Timer
	TimerEnable(TIMER0_BASE, TIMER_A);
}

void VelocityControlTimer(void){
	timer_period = ((SysCtlClockGet()*2));  // this means 100 ms, 10 Hz

	TimerIntRegister(TIMER1_BASE, TIMER_A, VelocityControlInt);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER1_BASE, TIMER_A, timer_period);
	IntEnable(INT_TIMER1A);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	//Enable Timer
	TimerEnable(TIMER1_BASE, TIMER_A);
}


