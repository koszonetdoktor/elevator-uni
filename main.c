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
#include "driverlib/timer.h"
#include "math.h"
#include "motion.h"
#include "sensors.h"
#include "uartfunc.h"
#include "uartstdio.h"
#include "timers.h"
#include "structs.h"


void FloorLearn(void); //prototype
int value;
int g_ui32SysClock;

struct CABIN left;
struct CABIN right;


//volatile unsigned char floor_switch;


int main(void) {
	g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
		                                     SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
		                                     SYSCTL_CFG_VCO_480), 120000000);
/*********************************************************
 *	Inits
 *********************************************************/
	SensorsInit();  //initialize all the sensors
	VelocityTimerInit(); // initialize the velocity measure timer
	//VelocityControlTimer();
	PWMInit0();
	PWMInit1();
	PWMInit2();
	PWMInit3();
	UARTInit();
	UART1Init();
/*********************************************************
 *	End of Inits
 ********************************************************/


	FloorLearn(); // this function will get the number of the floors and the distance between them

	while(1){

	}
	
	return 0;
}

//***********************************************************************
// Learning Cycel
//**********************************************************************
// még meg kell írnia a fügvény kiléptetést, taléna végén?
// struct használata


void FloorLearn(void){

	while(1){
		switch(floor_switch){
		case START_LEFT:	UARTprintf("Slow motion...Down\n");
							Move(LEFT_LIFT,DOWNWARD,30);
							floor_switch = LEFT_POS;
							break;

		case LEFT_POS:		if(GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_1) && GPIOPinRead(GPIO_PORTP_BASE,GPIO_PIN_1)){ // ha a reed és a photo szintérzékelő érzékel
								StopMotion(LEFT_LIFT);
								floor_switch = CYCLE_LEFT;
								UARTprintf("switch to the CYCLE\n");
								SysCtlDelay(400000);
							}
							break;

		case CYCLE_LEFT:	UARTprintf("slow motion up..\n");
							left.sign_counter = 0;
							Move(LEFT_LIFT, UPWARD, 30);
							floor_switch = WAIT_LEFT;
							break;

		case LEARNING_LEFT:	if(GPIOPinRead(GPIO_PORTP_BASE,GPIO_PIN_1)){  // nézi a bal oldali lift szintérzékelőjét
								left.floor_distance[left.floor_number] = left.sign_counter; // elmenti az értéket
								UARTprintf("%d: %d\n",left.floor_number,left.floor_distance[left.floor_number]);
								left.floor_number++; // tovább lép a következő szintre
								floor_switch = WAIT_LEFT;
							}
							left_reed_int_enable = 1;
							break;

		case WAIT_LEFT:		if(!(GPIOPinRead(GPIO_PORTP_BASE,GPIO_PIN_1))){ // vár arra hogy elhagyja a szint jeladót
								//counter_left = 0;
								floor_switch = LEARNING_LEFT;
							}
							break;

		case END_LEFT: 		UARTprintf("end, so stop!\n");
							StopMotion(LEFT_LIFT);
							left_reed_int_enable = 0;
							floor_switch = START_RIGHT;
							break;

		case START_RIGHT:	UARTprintf("Slow motion...Down\n");
							Move(RIGHT_LIFT,DOWNWARD,400);  // this is an ideal slow motion , equal than the 30 at the other side
							floor_switch = RIGHT_POS;
							break;

		case RIGHT_POS:		if(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_4) && GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_0)){ // ha a reed és a photo szintérzékelő érzékel
								StopMotion(RIGHT_LIFT);
								floor_switch = CYCLE_RIGHT;
								UARTprintf("switch to the CYCLE\n");
								SysCtlDelay(400000);
							}
							break;

		case CYCLE_RIGHT:	UARTprintf("slow motion up..\n");
							Move(RIGHT_LIFT, UPWARD,400);
							right.sign_counter = 0;
							floor_switch = WAIT_RIGHT;
							break;

		case LEARNING_RIGHT:if(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_4)){  // nézi a bal oldali lift szintérzékelőjét
								right.floor_distance[right.floor_number] = right.sign_counter; // elmenti az értéket
								UARTprintf("%d: %d\n",right.floor_number,right.floor_distance[right.floor_number]);
								right.floor_number++; // tovább lép a következő szintre
								floor_switch = WAIT_RIGHT;
							}
							right_reed_int_enable = 1;
							break;

		case WAIT_RIGHT:	if(!(GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_4))){ // vár arra hogy elhagyja a szint jeladót
								//counter_right = 0;
								floor_switch = LEARNING_RIGHT;
							}
							break;

		case END_RIGHT:		StopMotion(RIGHT_LIFT);
							floor_switch == LEARN_END;
							return;

		} // end of switch-case
	} // end of while(1)
}// end of FloorLearn

