/*
 * main.c
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

int main(void) {
	PWMInit();  // initialized the pwm signals used by the motors
	LeftEncoderInit();
	RigthEncoderInit();

	//PWM test
	/*
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 , 20000); // 1 Volt
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , 30000); // 1,5 Volt
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , 40000); // 2 Volt
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , 50000); // 2,5 Volt
	*/
	while(1){

	}
	
	return 0;
}
