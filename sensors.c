/*
 * sensors.c
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

unsigned char swi = 0;

void LeftEncoderIntHandler(void){
	if(swi==0){
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 , 60000);
	}
	if(swi==1){
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 , 1);
	}
	swi^=1;
	GPIOIntClear(GPIO_PORTN_BASE,GPIO_PIN_2); // have to do this, because it wont be called again
}

void RigthEncoderIntHandler(void){
	if(swi==0){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 , 60000);
	}
	if(swi==1){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 , 1);
	}
	swi^=1;
	GPIOIntClear(GPIO_PORTM_BASE,GPIO_PIN_4); // have to do this, because it wont be called again
}

void LeftEncoderInit(void){
	// init of GPIO port
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
	//init of the pin
	GPIOPinTypeGPIOInput(GPIO_PORTN_BASE,GPIO_PIN_3);
	GPIOPinTypeGPIOInput(GPIO_PORTN_BASE,GPIO_PIN_2);

	GPIOIntRegister(GPIO_PORTN_BASE,LeftEncoderIntHandler);
	// this set the interrrupt to risign edge
	GPIOIntTypeSet(GPIO_PORTN_BASE,GPIO_PIN_1,GPIO_RISING_EDGE);
	// enable the interrrupt
	GPIOIntEnable(GPIO_PORTN_BASE,GPIO_PIN_2);
} // end of LeftEncoderInit()

void RigthEncoderInit(void){
	// init of GPIO port
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
	//init of the pin
	GPIOPinTypeGPIOInput(GPIO_PORTM_BASE,GPIO_PIN_4);
	GPIOPinTypeGPIOInput(GPIO_PORTM_BASE,GPIO_PIN_5);

	GPIOIntRegister(GPIO_PORTM_BASE,RigthEncoderIntHandler);
	// this set the interrrupt to risign edge
	GPIOIntTypeSet(GPIO_PORTM_BASE,GPIO_PIN_4,GPIO_RISING_EDGE);
	// enable the interrrupt
	GPIOIntEnable(GPIO_PORTN_BASE,GPIO_PIN_4);
} //end of RigthEncoderInit()
