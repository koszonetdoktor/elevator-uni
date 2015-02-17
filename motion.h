/*
 * motion.h
 *
 *  Created on: 2015.02.17.
 *      Author: Elek
 */

#ifndef MOTION_H_
#define MOTION_H_

//*****************************************
//PWM signals
//*****************************************
#define PWM11	0x00050006  //PF0
#define PWM12	0x00050406	//PF1
#define PWM21	0x00050806	//PF2
#define PWM22 	0x00050C06	//PF3

extern void SlowMotion(unsigned char lift, unsigned char direction); // lift=1(right),lift=0(left)  direction=1(up),direction=0(down)
extern void PWMInit(void);


#endif /* MOTION_H_ */
