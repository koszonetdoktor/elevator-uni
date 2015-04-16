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
#include "defines.h"


void FloorLearn(void); //prototype
void LeftDirectionSense(void); // prototype
void RightDirectionSense(void);
void LeftCollector(void);
int value;
int g_ui32SysClock;
int i; // using for the target sensing

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
	PWMInit0();
	PWMInit1();
	PWMInit2();
	PWMInit3();
	UARTInit();
	//UART1Init();
	VelocityTimerInit(); // contorl of velocity timer
	VelocityControlTimer(); // fo the acceleration
	VelocityMeter(); // velocity measure timer
	VelocityControlTimer2(); // timer of deccalaretion
	left.next_target = NO_TARGET;
	right.next_target = NO_TARGET;
	left.a = L_NORMAL_V*1000 / (45*53); // 53 ms is the frequency when the acceleration function is called, 30 is the ISR cycle need for a cabin to start the movement
	right.a = R_NORMAL_V*1000 / (24*53); //53 ms is the frequency when the acceleration function is called, 55 is the ISR cycle need for a cabin to start the movement
/*********************************************************
 *	End of Inits
 ********************************************************/

	//FloorLearn(); // this function will get the number of the floors and the distance between them


	int testarray[] = {2,5,0,4,6,8,5,1,3,NO_TARGET};
	int testarray2[] = {2,5,0,4,6,8,5,1,3,NO_TARGET};
	char test_i,test_i2;



	while(1){


	//PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , 1);
	//PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 , (10000 * 150/375));

	/*PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , 1);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , (10000 * 3000/3398));*/

		switch(main_switch){
			case LEARN:			FloorLearn(); // learn everything
								TimerEnable(TIMER3_BASE, TIMER_A); // lassítás interrupt
								main_switch = DIR_SENSE;
								left.next_target = 6;
								right.next_target = 6;
								break;

			case DIR_SENSE:		if(!(left.direction)){
									LeftDirectionSense();
								}
								if(!(right.direction)){
									RightDirectionSense();
								}
								if(left.direction || right.direction){ // if any of them has direction, that it goes to the moving part
									main_switch =  MOVE_START;
								}
								break;


			case MOVE_START:	if(left.direction && !left.moving){ // if it has direction and not moving yet... I dont want to call the Move() on and on while it it is moving
									Move(LEFT_LIFT,left.direction,L_NORMAL_V);
								}
								if(right.direction && !right.moving){
									Move(RIGHT_LIFT,right.direction,R_NORMAL_V);
								}
								main_switch = COLLECTING;
								break;

			case COLLECTING:	//LeftCollector();
								main_switch = FLOOR_SENSE;
								break;

			case FLOOR_SENSE:	if(left.direction){ // ha mozog  alift
									if(left.position == left.next_target){ // ha szintbe ért
										StopMotion(LEFT_LIFT);
										//left.next_target = testarray[test_i]; // just for TEST
										//UARTprintf("left_target: %d\n",left.next_target);
										//if(test_i <= 9) test_i++; // just for TEST
									}
								}
								if(right.direction){ // ha mozog a lift
									if(right.position == right.next_target){ // ha szintbe ért
										StopMotion(RIGHT_LIFT);
										right.next_target = testarray2[test_i2]; // just for TEST
										//UARTprintf("right_target: %d\n",right.next_target);
										if(test_i2 <= 9) test_i2++; // just for TEST
									}
								}
								if(!(left.direction && right.direction)){ // if one of the lifts doesn't have direction, it jumops back to the DIR_SENSE
									main_switch = DIR_SENSE;
								}
								break;
		}// end of switch-case


	}
	
	return 0;
}

//***********************************************************************
// Learning Cycel
//**********************************************************************


void FloorLearn(void){

	left.position = -1; // correction to be accurate
	right.position = -1; // // correction to be accurate
	left.floor_number = 1;
	right.floor_number = 1;


	while(1){
		switch(floor_switch){
		case START_LEFT:	UARTSend("Slow motion...Down\n",20);
							Move(LEFT_LIFT,DOWNWARD,L_NORMAL_V);
							floor_switch = LEFT_POS;
							break;

		case LEFT_POS:		if(GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_1) && GPIOPinRead(GPIO_PORTP_BASE,GPIO_PIN_1)){ // ha a reed és a photo szintérzékelő érzékel
								StopMotion(LEFT_LIFT);
								floor_switch = CYCLE_LEFT;
								SysCtlDelay(400000);
							}
							break;

		case CYCLE_LEFT:	UARTSend("slow motion up..\n",15);
							GPIOIntEnable(GPIO_PORTP_BASE,GPIO_PIN_1); // enables the interrupt that counts the position of the cabin
							//left.sign_counter = 0; // start the distance counting here
							GPIOIntEnable(GPIO_PORTN_BASE,GPIO_PIN_2); // enable the signal counter interrupt
							Move(LEFT_LIFT, UPWARD, L_NORMAL_V);
							left.position = 0;
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
							left.position++; // beacuse it leaves the 8th floor, so when it comes back from the end state it will miss the Ground frool
							UARTprintf("%d\n",left.position);
							//left.position = left.floor_number;
							floor_switch = START_RIGHT;
							break;

		case START_RIGHT:	UARTprintf("Slow motion...Down\n");
							Move(RIGHT_LIFT,DOWNWARD,R_NORMAL_V);  // this is an ideal slow motion , equal than the 30 at the other side
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
							GPIOIntEnable(GPIO_PORTL_BASE,GPIO_PIN_4); // enables the interrupt that counts the position of the cabin
							//right.sign_counter = 0;
							GPIOIntEnable(GPIO_PORTM_BASE,GPIO_PIN_4); // enable the signal counter interrupt
							right.position = 0;
							Move(RIGHT_LIFT, UPWARD,R_NORMAL_V);
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
							right.position++; // beacuse it leaves the 8th floor, so when it comes back from the end state it will miss the Ground frool
							UARTprintf("p:%d\n",right.position);
							//right.position = right.floor_number;
							return;

		} // end of switch-case
	} // end of while(1)
}// end of FloorLearn

void LeftDirectionSense(void){ //

	if(left.next_target != NO_TARGET){  // if there is a target(-1 == no target)
		if(left.next_target > left.position){
			left.direction = UPWARD;
		}
		else if(left.next_target < left.position){
			left.direction = DOWNWARD;
		}
		else {
			left.direction = NODIR;
		}
		//UARTprintf("next: %d dir: %d\n",left.next_target,left.direction);
	}
}// end of LeftTargetSense

void LeftDirectionSense2(void){

}

void RightDirectionSense(void){ //
	if(right.next_target != NO_TARGET){
		if(right.next_target > right.position){
			right.direction = UPWARD;
		}
		else if(right.next_target < right.position){
			right.direction = DOWNWARD;
		}
		else {
			right.direction = NODIR;
		}
	}
}// end of RightTargetSense

void LeftCollector(void){
	int i;
	int call_found = -1,aim_found = -1;

	if(left.direction == UPWARD){
		for(i=left.position;i<left.floor_number;i++){ // a pozíciótól a magszintig végig pörög
			if(left.call_aims[i])  aim_found = i;
			if(left.upward_calls[i]) call_found = i;

			if((aim_found != -1) || (call_found != -1)){
				if(aim_found == call_found){
					left.next_target = aim_found;
					break;
				}
				else if(aim_found == -1){
					left.next_target = call_found;
					break;
				}
				else{
					left.next_target = aim_found;
					break;
				}
			}
		}
	}

	if(left.direction == DOWNWARD){
		for(i=left.position;i>=0;i--){ // a jelenlegi pozíciótól a földig végigi pörörg
			if(left.call_aims[i])  aim_found = i;
			if(left.upward_calls[i]) call_found = i;

			if((aim_found != -1) || (call_found != -1)){
				if(aim_found == call_found){
					left.next_target = aim_found;
					break;
				}
				else if(aim_found == -1){
					left.next_target = call_found;
					break;
				}
				else{
					left.next_target = aim_found;
					break;
				}
			}
		}
	}
}// end of LeftCollectro()*/

















