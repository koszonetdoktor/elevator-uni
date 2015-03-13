/*
 * motion.c
 *
 *  Created on: 2015.02.17.
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
#include "timers.h"
#include "motion.h"

int desired_v;


void PWMInit(void){ //intitalized all the pwms..(PF0,PF1,PF2,PF3) NEM HASZNÁLOM!!
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	//configure the pins F0, F1, F2, F3
	GPIOPinConfigure(GPIO_PF0_M0PWM0);
	GPIOPinConfigure(GPIO_PF1_M0PWM1);
	GPIOPinConfigure(GPIO_PF2_M0PWM2);
	GPIOPinConfigure(GPIO_PF3_M0PWM3);

	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_0);
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);
	// configure the two generatot that will be in use
	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	// set the 2 generator period time
	PWMGenPeriodSet(PWM0_BASE,  PWM_GEN_0, 65000);
	PWMGenPeriodSet(PWM0_BASE,  PWM_GEN_1, 65000);
	// set the pulse width
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 , 1);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , 1);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , 1);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , 1);

	PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT | PWM_OUT_3_BIT , true);

	// enables the 2 generators
	PWMGenEnable(PWM0_BASE, PWM_GEN_0);
	PWMGenEnable(PWM0_BASE, PWM_GEN_1);

}//end of PWMInit()

void PWMInit0(void){
		// enble the peripharial for the LED
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		// ENABLE GPIO PIN
		GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_0);
		// enable the pwm0 peripherial
		SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
		//configure pwm signal
		PWMGenConfigure(PWM0_BASE,PWM_GEN_0,
								PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
		//enable the pwm signal
		PWMOutputState(PWM0_BASE,PWM_OUT_0_BIT,true);
		//set the period
		PWMGenPeriodSet(PWM0_BASE,PWM_GEN_0,65000);
		//set the puls width
		PWMPulseWidthSet(PWM0_BASE,PWM_OUT_0,1);
		//configure the pin
		GPIOPinConfigure(GPIO_PF0_M0PWM0);
		//set the pit type
		GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_0);
		//start the timer
		PWMGenEnable(PWM0_BASE,PWM_GEN_0);
}
//**********************************************************************
void PWMInit1(void){
		// enble the peripharial for the LED
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		// ENABLE GPIO PIN
		GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_1);
		// enable the pwm0 peripherial
		SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
		PWMGenConfigure(PWM0_BASE,PWM_GEN_0,
								PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
		PWMOutputState(PWM0_BASE,PWM_OUT_1_BIT,true);
		PWMGenPeriodSet(PWM0_BASE,PWM_GEN_0,65000);
		PWMPulseWidthSet(PWM0_BASE,PWM_OUT_1,1);
		GPIOPinConfigure(GPIO_PF1_M0PWM1);
		GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_1);
		PWMGenEnable(PWM0_BASE,PWM_GEN_0);

}

void PWMInit2(void){
		// enble the peripharial for the LED
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		// ENABLE GPIO PIN
		GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_2);
		// enable the pwm0 peripherial
		SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
		PWMGenConfigure(PWM0_BASE,PWM_GEN_1,
								PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
		PWMOutputState(PWM0_BASE,PWM_OUT_2_BIT,true);
		PWMGenPeriodSet(PWM0_BASE,PWM_GEN_1,65000);
		PWMPulseWidthSet(PWM0_BASE,PWM_OUT_2,1);
		GPIOPinConfigure(GPIO_PF2_M0PWM2);
		GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_2);
		PWMGenEnable(PWM0_BASE,PWM_GEN_1);

}

void PWMInit3(void){
		// enble the peripharial for the LED
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		// ENABLE GPIO PIN
		GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_3);
		// enable the pwm0 peripherial
		SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
		PWMGenConfigure(PWM0_BASE,PWM_GEN_1,
								PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
		PWMOutputState(PWM0_BASE,PWM_OUT_3_BIT,true);
		PWMGenPeriodSet(PWM0_BASE,PWM_GEN_1,65000);
		PWMPulseWidthSet(PWM0_BASE,PWM_OUT_3,1);
		GPIOPinConfigure(GPIO_PF3_M0PWM3);
		GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_3);
		PWMGenEnable(PWM0_BASE,PWM_GEN_1);

}

void SlowMotion(unsigned char lift,unsigned char direction){		// lift=1(right),lift=0(left)  direction=0(up),direction=1(down)
	if(lift == 0){ // left
		if(direction == 0){ // up
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 ,1 );
			SysCtlDelay(200);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , 12090); // v= 30 upward
		}
		if(direction == 1){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , 1);
			SysCtlDelay(200);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 , 10000); // v = 30 Downward
		}
		else{
			return -1;
		}
	}
	if(lift == 1){
		if(direction == 0){ // up
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , 15508); // v =~ 330
			SysCtlDelay(200);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , 1);
		}
		if(direction == 1){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , 1);
			SysCtlDelay(200);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , 12000); // v =~ 330
		}
		else{
			return -1;
		}
	}


}// end of SlowMotion(

void Move(unsigned char lift,unsigned char direction, float velocity){
	// azt még lehetne ezen fejleszteni, hogy legyen egy univerzális sebesség, mert így mind a kettő más más tartományban mozog

	if((velocity>60) && (lift == 0)){ // this is for safety,
		velocity = 60;
	}
	if((velocity>700) && (lift == 1)){
		velocity = 700;
	}

	if(lift == 0){  // left
		left_desired_v = velocity;
		left_dir = direction;
		if(direction == 0){  // up
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 ,1 );
			SysCtlDelay(200);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , (10000 * (velocity/30)));
		}
		if(direction == 1){ // down
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , 1);
			SysCtlDelay(200);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 , (10000 * (velocity/30)));
		}
	}
	if(lift == 1){  // right
		right_desired_v = velocity;
		right_dir = direction;
		if(direction == 0){ // up
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , (15508 * (velocity/330)));
			SysCtlDelay(200);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , 1);
		}
		if(direction == 1){ // down
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , 1);
			SysCtlDelay(200);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , (12000 * (velocity/330)));
		}
	}
} // end of move

void StopMotion(unsigned char lift){
	if(lift == 1){ // right
		right_desired_v = 0;
		right_dir = NODIR;
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , 1);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , 1);
	}
	if(lift == 0){ // left
		left_desired_v = 0;
		left_dir = NODIR;
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 , 1);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , 1);
	}
	else{
		return -1;
	}
}


/****************************************************************
 * Under Development
 */
void Acceleration(){


}


int Deceleration(int distance){
	unsigned char a = 2;
	int velocity;

	velocity = sqrt(2*a*distance); // this is the main equation of the deceleration

	return velocity;
}



