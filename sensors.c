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
#include "utils/uartstdio.h"
#include "sensors.h"
#include "motion.h"
#include "structs.h"

unsigned char swi = 0;
int counter_left, counter_right;
volatile unsigned char left_reed_int_enable,right_reed_int_enable;


// átíroma szint fototranyóját interruptosrA


//*******************************************************************************
//Interrupt Handlers
//*******************************************************************************
void  ReedIntHandler(void){ // ez csak ideiglenes, amjd ki lesz cserélve a végállással, csak nincs még ledem
	if(left_reed_int_enable == 1){
		floor_switch = END_LEFT;
		//StopMotion(LEFT_LIFT);
		GPIOIntDisable(GPIO_PORTD_BASE,GPIO_PIN_1);
	}
	if(right_reed_int_enable == 1){
		floor_switch = END_RIGHT;
		GPIOIntDisable(GPIO_PORTD_BASE,GPIO_PIN_0);
	}
	GPIOIntClear(GPIO_PORTD_BASE,GPIO_PIN_1);
	GPIOIntClear(GPIO_PORTD_BASE,GPIO_PIN_0);
}


void LeftEncoderIntHandler(void){
	if(left.direction == UPWARD) left.sign_counter++;
	if(left.direction == DOWNWARD) left.sign_counter--;
	GPIOIntClear(GPIO_PORTN_BASE,GPIO_PIN_2); // have to do this, because it wont be called again
}

void RigthEncoderIntHandler(void){
	if(right.direction == UPWARD) right.sign_counter++;
	if(right.direction == DOWNWARD) right.sign_counter--;
	GPIOIntClear(GPIO_PORTM_BASE,GPIO_PIN_4); // have to do this, because it wont be called again
}

void LeftLevelSensorIntHandler(void){
	if((left.direction == UPWARD) && GPIOPinRead(GPIO_PORTP_BASE,GPIO_PIN_1)){ // I puted the GPIOPinRead here, because sometimes the reed sensor changed the .position variable too
		left.position++;
		//UARTprintf("l_pos: %d\n", left.position);
	}
	if((left.direction == DOWNWARD) && GPIOPinRead(GPIO_PORTP_BASE,GPIO_PIN_1)){
		left.position--;
		//UARTprintf("l_pos: %d\n", left.position);
	}
	GPIOIntClear(GPIO_PORTP_BASE,GPIO_PIN_1);
} // end of LeftLevelSensorIntHandler

void RightLevelSensorIntHandler(void){
	if((right.direction == UPWARD) && GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_4)){
		right.position++;
		//UARTprintf("r_pos: %d\n", right.position);
	}
	if((right.direction == DOWNWARD) && GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_4)){
		right.position--;
		//UARTprintf("r_pos: %d\n", right.position);
	}
	GPIOIntClear(GPIO_PORTL_BASE,GPIO_PIN_4);
}	// end of RightLevelSensorIntHandler

void HbridgeErrorIntHandler(void){

}// end of RightHbridgeErrorIntHandler

void LeftHbridgeErrorIntHandler(void){

}// end of LeftHbridgeErrorIntHandler(

void RigthEndStateIntHandler(void){

} // end of RightEndStateInthand

void LeftEndStateIntHandler(void){

}// end of LeftEndStateInthand
//*******************************************************************************
//					END of Interrupt Handler
//*******************************************************************************

//*******************************************************************************
// Init Functions
//*******************************************************************************

void LeftEncoderInit(void){ //N3, N2
	// init of GPIO port
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
	//init of the pin
	GPIOPinTypeGPIOInput(GPIO_PORTN_BASE,GPIO_PIN_3);
	GPIOPinTypeGPIOInput(GPIO_PORTN_BASE,GPIO_PIN_2);

	GPIOIntRegister(GPIO_PORTN_BASE,LeftEncoderIntHandler);
	// this set the interrrupt to risign edge
	GPIOIntTypeSet(GPIO_PORTN_BASE,GPIO_PIN_2,GPIO_RISING_EDGE);
	// enable the interrrupt
	//GPIOIntEnable(GPIO_PORTN_BASE,GPIO_PIN_2);
} // end of LeftEncoderInit()

void RigthEncoderInit(void){ // M3, M2
	// init of GPIO port
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
	//init of the pin
	GPIOPinTypeGPIOInput(GPIO_PORTM_BASE,GPIO_PIN_4);
	GPIOPinTypeGPIOInput(GPIO_PORTM_BASE,GPIO_PIN_5);

	GPIOIntRegister(GPIO_PORTM_BASE,RigthEncoderIntHandler);
	// this set the interrrupt to risign edge
	GPIOIntTypeSet(GPIO_PORTM_BASE,GPIO_PIN_4,GPIO_RISING_EDGE);
	// enable the interrrupt
	//GPIOIntEnable(GPIO_PORTM_BASE,GPIO_PIN_4);
} //end of RigthEncoderInit()

void LeftReedInit(void){ // PD1
	// init of GPIO port
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	//init of the pin
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,GPIO_PIN_1);
	// EZ a rész csak ideiglenes, majd ki lesz cserélve a végállással
	GPIOIntRegister(GPIO_PORTD_BASE,ReedIntHandler);
	// this set the interrrupt to risign edge
	GPIOIntTypeSet(GPIO_PORTD_BASE,GPIO_PIN_1,GPIO_RISING_EDGE);
	// enable the interrrupt
	GPIOIntEnable(GPIO_PORTD_BASE,GPIO_PIN_1);
}// end of RightReedInit()

void RightReedInit(void){ // PD0
	// init of GPIO port
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	//init of the pin
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,GPIO_PIN_0);
	// EZ a rész csak ideiglenes, majd ki lesz cserélve a végállással
	GPIOIntRegister(GPIO_PORTD_BASE,ReedIntHandler);
	// this set the interrrupt to risign edge
	GPIOIntTypeSet(GPIO_PORTD_BASE,GPIO_PIN_0,GPIO_RISING_EDGE);
	// enable the interrrupt
	GPIOIntEnable(GPIO_PORTD_BASE,GPIO_PIN_0);

} // end of RightReedInit()

void LeftLevelSensorInit(void){ // PP1
	// init of GPIO port
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
	//init of the pin
	GPIOPinTypeGPIOInput(GPIO_PORTP_BASE,GPIO_PIN_1);

	GPIOIntRegister(GPIO_PORTP_BASE,LeftLevelSensorIntHandler);
	// this set the interrrupt to risign edge
	GPIOIntTypeSet(GPIO_PORTP_BASE,GPIO_PIN_1,GPIO_RISING_EDGE);

} // end of LeftLevelSensorInit()

void RightLevelSensorInit(void){ // PL4
	// init of GPIO port
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
	//init of the pin
	GPIOPinTypeGPIOInput(GPIO_PORTL_BASE,GPIO_PIN_4);
	GPIOIntRegister(GPIO_PORTL_BASE,RightLevelSensorIntHandler);
	// this set the interrrupt to risign edge
	GPIOIntTypeSet(GPIO_PORTL_BASE,GPIO_PIN_4,GPIO_RISING_EDGE);
}// end of RightLEvelSensor()

void LeftEndState(void){ // PD2 with interrupt , disturb the reed interrupt , because they are on the same port
	// init of GPIO port
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	//init of the pin
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,GPIO_PIN_2);

	GPIOIntRegister(GPIO_PORTD_BASE,LeftEndStateIntHandler);
	// this set the interrrupt to risign edge
	GPIOIntTypeSet(GPIO_PORTD_BASE,GPIO_PIN_2,GPIO_RISING_EDGE);
	// enable the interrrupt
	GPIOIntEnable(GPIO_PORTD_BASE,GPIO_PIN_2);
}// end of LeftEndState()

void RightEndState(void){ // PK6 with interrupt
	// init of GPIO port
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	//init of the pin
	GPIOPinTypeGPIOInput(GPIO_PORTK_BASE,GPIO_PIN_6);

	GPIOIntRegister(GPIO_PORTK_BASE,RigthEndStateIntHandler);
	// this set the interrrupt to risign edge
	GPIOIntTypeSet(GPIO_PORTK_BASE,GPIO_PIN_6,GPIO_RISING_EDGE);
	// enable the interrrupt
	GPIOIntEnable(GPIO_PORTK_BASE,GPIO_PIN_6);
}// end of RightEndState()

void RightHbridgeErrorInit(void){ //PL5
	// init of GPIO port
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
	//init of the pin
	GPIOPinTypeGPIOInput(GPIO_PORTL_BASE,GPIO_PIN_5);

	/*GPIOIntRegister(GPIO_PORTL_BASE,HbridgeErrorIntHandler);
	// this set the interrrupt to risign edge
	GPIOIntTypeSet(GPIO_PORTL_BASE,GPIO_PIN_5,GPIO_RISING_EDGE);
	// enable the interrrupt
	GPIOIntEnable(GPIO_PORTL_BASE,GPIO_PIN_5);*/
} // end of RightHbridgeErrorFlag

void LeftHbridgeErrorInit(void){ // PJ0
	// init of GPIO port
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
	//init of the pin
	GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE,GPIO_PIN_0);

	/*GPIOIntRegister(GPIO_PORTJ_BASE,HbridgeErrorIntHandler);
	// this set the interrrupt to risign edge
	GPIOIntTypeSet(GPIO_PORTJ_BASE,GPIO_PIN_0,GPIO_RISING_EDGE);
	// enable the interrrupt
	GPIOIntEnable(GPIO_PORTJ_BASE,GPIO_PIN_0);*/
}// end of LeftHbridgeErrorFlag

void SensorsInit(void){
	LeftEncoderInit(); 	//szar!  Baztaja az interrupt?? levettem!
	RigthEncoderInit(); 	//jó!
	LeftReedInit();		//jó
	RightReedInit();		//jó
	LeftLevelSensorInit();//jó
	RightLevelSensorInit();//jó
	//LeftEndState();		//jó összekavarja a reed interruptot mert ugyan azona porton van
	//RightEndState();		//jó
	//RightHbridgeErrorInit();//jó not in use, beacusse the Right Level Sensor uses the same interrupt
	LeftHbridgeErrorInit();	//jó
}
//*******************************************************************************
//					END of Init Functions
//*******************************************************************************

