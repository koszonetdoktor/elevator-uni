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
#include "timers.h"
#include "defines.h"

float left_v,right_v;
int left_distance,right_distance;
int test_l,test_r,v;
int l_a,r_a;
int test1,test2,test3,test4,test5;
float dec_a_l,dec_a_r,dec2_a_r;

int a_cnt;




void VelocityMeterInt(void){
	l_signalcnt = left.sign_counter;
	left_distance = l_signalcnt-l_prev_signalcnt; // thsi determines the velocity from the previous and the actual velocity
	left.velocity = (left_distance*1000)/80;
	l_prev_signalcnt = l_signalcnt;


	r_signalcnt = right.sign_counter;
	right_distance = r_signalcnt-r_prev_signalcnt; // thsi determines the velocity from the previous and the actual velocity
	right.velocity = (right_distance*1000)/80;
	r_prev_signalcnt = r_signalcnt;

	if(left.direction == DOWNWARD) left.velocity = left.velocity *(-1);
	if(right.direction == DOWNWARD) right.velocity = right.velocity *(-1);


	//test1 = (int)left.velocity;
	//test2 = (int)right.velocity;
	//UARTprintf("left: %d right %d\n",test1,test2);

	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
}


void Timer0IntHandler(void){
	float x;


	/* *********************************************************************
	 * Control, This part contorls the velocity to keep the desired level
	 ***********************************************************************/
	if(left.velocity > 0){ // this eliminates the possibility of devide with zero

		if(left.desired_velocity > left.velocity) x = 1.01; // thsi is the P, but not so good, when it has some weight on it
		else if(left.desired_velocity < left.velocity) x = 0.99;
		else x = 1;

		if(left.direction == UPWARD){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 ,(PWMPulseWidthGet(PWM0_BASE, PWM_OUT_1) * x));
		}
		if(left.direction == DOWNWARD){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 ,(PWMPulseWidthGet(PWM0_BASE, PWM_OUT_0) * x));
		}
	}
	if(right.velocity > 0){

		if(right.desired_velocity > right.velocity) x = 1.01;
		else if(right.desired_velocity < right.velocity) x = 0.99;
		else x = 1;


		if(right.direction == UPWARD){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 ,(PWMPulseWidthGet(PWM0_BASE, PWM_OUT_2) * x));
		}
		if(right.direction == DOWNWARD){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 ,(PWMPulseWidthGet(PWM0_BASE, PWM_OUT_3) * x));
		}
	}
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
} // enf of Timer0IntHandler()

void VelocityControlInt(void){ // gyorsítás


	l_a = 5; // acceleration rate
	r_a = 55; // acceleration rate


/*************************************
 	 	 	 PRE testers
*************************************/
/*	if(left.acceleration){
		if(left.desired_velocity <= left.velocity){
			UARTprintf("belp...\n");
			left.acceleration = 0;
		}
	}
	if(right.acceleration){
		if(right.desired_velocity <= right.velocity){
			right.acceleration = 0;
		}
	}*/

//*************************************

	if(left.acceleration){
		left_v += l_a;
		left.desired_velocity = left_v;
		left.astep_counter++;
		if(left.direction == UPWARD){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 ,1 );
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , (10000 * left_v/375)); // 10 000 PWM means 112.5 jel/s velocity
		}
		if(left.direction == DOWNWARD){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , 1);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 , (10000 * (left_v/375)));
		}
		//if(left_v == 150) UARTprintf("elérte : %d\n",left.astep_counter);
		if(left_v >= left.final_velocity){
			left.astep_counter = 0;
			left.acceleration = 0;
			left_v = 0;
		}
	} // end of left acceleration

	if(right.acceleration){
		right_v += r_a;
		right.desired_velocity = right_v;
		right.astep_counter++;
		if(right.direction == UPWARD){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , (10000 * right_v/3398));
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , 1);
		}
		if(right.direction == DOWNWARD){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , 1);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , (10000 * right_v/3398));
		}
		//if(right_v >= 3000) UARTprintf("elérte: %d\n",right.astep_counter);
		if(right_v >= right.final_velocity){
			right.astep_counter = 0;
			right.acceleration = 0;
			right_v = 0;
		}
	} // end of right acceleration

	//UARTprintf("dec: %d\n",DecEquation(right.sign_counter,right.a));
	//UARTprintf("sign: %d\n",right.sign_counter);


	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}// end of void VelocityControlInt(void)


void VelocityControlInt2(void){ // lassítás

	dec_a_l = 2.5;
	dec_a_r = 6.875;
	dec2_a_r = 13.75;


	a_cnt++;
	if(a_cnt == 4){
	if(left.direction){
		if(left.direction == UPWARD){
			if(left.velocity >= DecEquation((left.floor_distance[left.next_target] - left.sign_counter),left.a)){
				left.acceleration = 0;
				if(!(left.dec_v)) left.dec_v = left.velocity;
				else{
					left.dec_v -= dec_a_l;
					if(left.dec_v <= 210) left.dec_v = 210; // correction to not reach the zero velocity, becasue the cabin has to stop when it reaches the floor sensor
					left.desired_velocity = left.dec_v;
					//UARTprintf("ez ez: %d\n",left.dec_v);
					PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 ,1 );
					PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , (10000 * (left.dec_v/375)));
				}
			}
		} // end of Upward
		if(left.direction == DOWNWARD){
			/*test5 = (int)left.velocity;
			test4 = DecEquation((left.floor_distance[left.next_target] - left.sign_counter),left.a);
			test3 = (int)left.a;*/
			//UARTprintf("vel: %d equ: %d\n",test5,test4);
			//UARTprintf("dist: %d sign: %d  equal = %d ... a: %d\n",left.floor_distance[left.next_target],left.sign_counter,(left.sign_counter - left.floor_distance[left.next_target]),test3);
			if((left.velocity) >= DecEquation((left.sign_counter - left.floor_distance[left.next_target]),left.a)){
				left.acceleration = 0; // id kéne egy sebességmegállítás
				if(!(left.dec_v)) left.dec_v = left.velocity;
				left.dec_v -= dec_a_l;
				if(left.dec_v < 170) left.dec_v = 170; // correction to not reach the zero velocity, becasue the cabin has to stop when it reaches the floor sensor
				left.desired_velocity = left.dec_v;
				PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , 1);
				PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 , (10000 * (left.dec_v/375)));
			}
		}// end of DOWNWARD
	}// end of left.dir
	a_cnt = 0;
	}
	if(right.direction){
		if(right.direction == UPWARD){
			if(right.velocity >= DecEquation((right.floor_distance[right.next_target] - right.sign_counter),right.a)){
				right.acceleration = 0; // id kéne egy sebességmegállítás
				if(!(right.dec_v)) right.dec_v = right.velocity;
				right.dec_v -= dec_a_r;
				if(right.dec_v <= 3500) right.dec_v = 3500; // correction to not reach the zero velocity, becasue the cabin has to stop when it reaches the floor sensor
				right.desired_velocity = left.dec_v;
				//UARTprintf("ez ez: %d\n",left.dec_v);
				PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , (10000 * right.dec_v/3398));
				PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , 1);
			}
		}

		if(right.direction == DOWNWARD){
			if(right.velocity >= DecEquation((right.sign_counter - right.floor_distance[right.next_target]),right.a)){
				right.acceleration = 0; // id kéne egy sebességmegállítás
				if(!(right.dec_v)) right.dec_v = right.velocity;
				right.dec_v -= dec2_a_r;
				if(right.dec_v <= 1800) right.dec_v = 1800; // correction to not reach the zero velocity, becasue the cabin has to stop when it reaches the floor sensor
				right.desired_velocity = left.dec_v;
				//UARTprintf("ez ez: %d\n",left.dec_v);
				PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , (10000 * right.dec_v/3398));
				PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , 1);
			}
		}
	}


	TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
} // end of right.dir




void VelocityTimerInit(void){
	timer_period = ((SysCtlClockGet()/1));

	TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER0_BASE, TIMER_A, timer_period/1.5); // 80/1.5 ms
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	//Enable Timer
	TimerEnable(TIMER0_BASE, TIMER_A);
}

void VelocityMeter(void){
	timer_period = ((SysCtlClockGet()/1));

	TimerIntRegister(TIMER2_BASE, TIMER_A, VelocityMeterInt);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER2_BASE, TIMER_A, timer_period); // 80 ms
	IntEnable(INT_TIMER2A);
	TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	//Enable Timer
	TimerEnable(TIMER2_BASE, TIMER_A);
}

void VelocityControlTimer(void){ // for the acceleration
	int time;
	time = ((SysCtlClockGet()/1));  //

	TimerIntRegister(TIMER1_BASE, TIMER_A, VelocityControlInt);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER1_BASE, TIMER_A, time/2); // this means 40 ms
	IntEnable(INT_TIMER1A);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	//Enable Timer
	TimerEnable(TIMER1_BASE, TIMER_A);
}

void VelocityControlTimer2(void){ // thsi si for the decceleration
	int time;
	time = ((SysCtlClockGet()/1));  //

	TimerIntRegister(TIMER3_BASE, TIMER_A, VelocityControlInt2);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
	TimerConfigure(TIMER3_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER3_BASE, TIMER_A, time/16); // this means 10 ms
	IntEnable(INT_TIMER3A);
	TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	//Enable Timer
	//TimerEnable(TIMER3_BASE, TIMER_A);
}



//void Velocity

