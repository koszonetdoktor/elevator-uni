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
#include "motion.h"
#include "sensors.h"
#include "uartfunc.h"
#include "uartstdio.h"

//void FloorLearn(void);
int value;
int g_ui32SysClock;

//volatile unsigned char floor_switch;


int main(void) {
	g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
		                                     SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
		                                     SYSCTL_CFG_VCO_480), 120000000);
	SensorsInit();  //initialize all the sensors
	PWMInit0();
	PWMInit1();
	UARTInit();


	SlowMotion(LEFT_LIFT,DOWNWARD);

	//FloorLearn();
	//PWM test
	/*
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 , 20000); // 1 Volt
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , 30000); // 1,5 Volt
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , 40000); // 2 Volt
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , 50000); // 2,5 Volt
	*/
	while(1){
		UARTprintf("%d\n",counter_left);
		SysCtlDelay(2000);

	}
	
	return 0;
}

//***********************************************************************
// Learning Cycel
//**********************************************************************
void FloorLearn(void){
	unsigned char floor_counter = 0;
	int floor_distance[10];

	while(1){
		switch(floor_switch){
		case START_LEFT:	UARTprintf("Slow motion...Down\n");
							SlowMotion(LEFT_LIFT,DOWNWARD);
							floor_switch = WAIT_STOP;
							break;

		case WAIT_STOP:		UARTprintf("wait\n");
							if(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_0) && GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_1)){ // ha a reed és a photo szintérzékelő érzékel
								floor_switch = END_LEFT;
								UARTprintf("switch to the END_LEFT\n");
							}
							break;

		case CYCLE_LEFT:	//UARTprintf("slow motion up..\n");
							SlowMotion(LEFT_LIFT, UPWARD);
							counter_left = 0;
							floor_switch = WAIT_LEFT;
							break;

		case LEARNING_LEFT:	UARTprintf("Counting the floors and the distances....\n");
							if(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_1)){  // nézi a bal oldali lift szintérzékelőjét
								floor_distance[floor_counter] = counter_left; // elmenti az értéket
								floor_counter++; // tovább lép a következő szintre
								floor_switch = WAIT_LEFT;
							}
							break;

		case WAIT_LEFT:		UARTprintf("wait to leave the led..\n");
							if(!(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_1))){ // vár arra hogy elhagyja a szint jeladót
								counter_left = 0;
								swi2 = 1;
								floor_switch = LEARNING_LEFT;
							}
							break;

		case END_LEFT: 		UARTprintf("end, so stop!\n");
							StopMotion(LEFT_LIFT);
							floor_switch = START_RIGHT;
							break;

		case START_RIGHT:	break;

		}


	}
}// end of FloorLearn

