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



void PWMInit(void){ //intitalized all the pwms..(PF0,PF1,PF2,PF3)
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
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
	PWMGenPeriodSet(PWM0_BASE,  PWM_GEN_0, 64000);
	PWMGenPeriodSet(PWM0_BASE,  PWM_GEN_1, 64000);
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

void SlowMotion(unsigned char lift,unsigned char direction){		// lift=1(right),lift=0(left)  direction=1(up),direction=0(down)
	if(lift == 0){
		if(direction == 0){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 , 1);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , 10000);
		}
		if(direction == 1){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 , 10000);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , 1);
		}
		else{
			return -1;
		}
	}
	if(lift == 1){
		if(direction == 0){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , 1);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , 10000);
		}
		if(direction == 1){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , 10000);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , 1);
		}
		else{
			return -1;
		}
	}


}// end of SlowMotion()

