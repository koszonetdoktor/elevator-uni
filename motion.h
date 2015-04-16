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
#define LEFT_LIFT 	0
#define RIGHT_LIFT	1
#define UPWARD 		1
#define DOWNWARD	2
#define NODIR		0


extern void SlowMotion(unsigned char lift, unsigned char direction); // lift=1(right),lift=0(left)  direction=1(up),direction=0(down)
// on the right lift 10000 PWM = left lift 8000 PWM
extern void PWMInit(void);
extern void StopMotion(unsigned char lift); // lift=1 right, lift=0 left
extern void PWMInit0(void);
extern void PWMInit1(void);
extern void PWMInit2(void);
extern void PWMInit3(void);
extern void Deceleration(unsigned char lift, unsigned char direction, float s, char velocity);
extern int DecEquation(float s, float a);
extern void Move(unsigned char lift,unsigned char direction, float velocity); // this is a velocity based cabin mover function


#endif /* MOTION_H_ */
