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
#include "structs.h"
#include "defines.h"

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
		if(direction == UPWARD){ // up
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 ,1 );
			SysCtlDelay(200);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , 12090); // v= 30 upward
		}
		if(direction == DOWNWARD){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , 1);
			SysCtlDelay(200);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 , 10000); // v = 30 Downward
		}
		else{
			return -1;
		}
	}
	if(lift == 1){
		if(direction == UPWARD){ // up
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , 15508); // v =~ 330
			SysCtlDelay(200);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , 1);
		}
		if(direction == DOWNWARD){
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , 1);
			SysCtlDelay(200);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , 12000); // v =~ 330
		}
		else{
			return -1;
		}
	}


}// end of SlowMotion(

void Move(unsigned char lift,unsigned char direction, float velocity){ // this moves the cabin, with acceleration movement at the begining
	// azt még lehetne ezen fejleszteni, hogy legyen egy univerzális sebesség, mert így mind a kettő más más tartományban mozog
/*******************************
* Acceleration part
*****************************/

	if((velocity>560) && (lift == 0)){ // this is for safety,
		velocity = 560;
	}
	if((velocity>7500) && (lift == 1)){
		velocity = 7500;
	}

	if(lift == LEFT_LIFT){
		left.direction = direction;
		//left.desired_velocity = velocity;
		left.final_velocity = velocity;
		left.acceleration = 1;
		left.moving = 1;
	}
	if(lift == RIGHT_LIFT){
		right.direction = direction;
		right.desired_velocity = velocity;
		right.final_velocity = velocity;
		right.acceleration = 1;
		right.moving = 1;
	}

} // end of Move()

void StopMotion(unsigned char lift){
	if(lift == 1){ // right
		right.desired_velocity = 0;
		right.direction = NODIR;
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , 1);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , 1);
		right.moving = 0;
		right.next_target = NO_TARGET;
		/*right.dec_v =  0;
		SysCtlDelay(200);*/
	}
	if(lift == 0){ // left
		left.desired_velocity = 0;
		left.direction = NODIR;
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 , 1);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , 1);
		left.moving = 0;
		left.next_target = NO_TARGET;
		left.dec_v = 0;
		/*left.dec_v = 0;
		SysCtlDelay(200);*/
	}
	else{
		return -1;
	}
}


/****************************************************************
 * Under Development
 */



void Deceleration(unsigned char lift,unsigned char direction,float s, char velocity){ // ebbe nem rakok ciklust mert ezt a szintérzékelő lednek kell majd kiütni szerintem. (s == visszamaradt távolság, v = cabin sebesség)
	// lehet kne bele tenni egy kis csúszást, hogy ha el is érte a nulla sebességet de a szintet még nem , akkor is guruljon.
	// le kell tezstelni jobb oldalra is
	float a =  1.0000005;
	float v;

	v = sqrt(2*a*s); // this is the main equation of the deceleration
	/*if(velocity < v){
		v = velocity;
	}*/
	if(v != 0){UARTprintf("%d  %d\n",v,left.test);}
	if(lift == LEFT_LIFT){
		left.desired_velocity = v;
		if(direction == UPWARD){  // up
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 ,1 );
			SysCtlDelay(200);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , (10000 * (v/30)));
		}
		if(direction == DOWNWARD){ // down
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1 , 1);
			SysCtlDelay(200);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0 , (10000 * (v/30)));
		}
	}
	if(lift == RIGHT_LIFT){
		right.desired_velocity = v;
		if(direction == UPWARD){ // up
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , (15508 * (v/330)));
			SysCtlDelay(200);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , 1);
		}
		if(direction == DOWNWARD){ // down
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , 1);
			SysCtlDelay(200);
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , (12000 * (v/330)));
		}
	}
} // end of Decelartion()

int Round(float myfloat)
{
  double integral;
  float fraction = (float)modf(myfloat, &integral);

  if (fraction >= 0.5)
    integral += 1;
  if (fraction <= -0.5)
    integral -= 1;

  return (int)integral;
}

int DecEquation(float s, float a){
	int v;
	float tmp;

	v=(int)sqrt(2*a*s);

	return v;
}


