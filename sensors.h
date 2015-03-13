/*
 * sensors.h
 *
 *  Created on: 2015.02.17.
 *      Author: Elek
 */

#ifndef SENSORS_H_
#define SENSORS_H_

int counter_left;
int counter_right;
volatile unsigned char floor_switch;
volatile unsigned char left_reed_int_enable;
volatile unsigned char right_reed_int_enable;

extern void LeftEncoderIntHandler(void);
extern void RigthEncoderIntHandler(void);
extern void LeftEncoderInit(void);
extern void RigthEncoderInit(void);
extern void LeftEndStateIntHandler(void);
extern void RigthEndStateIntHandler(void);
extern void HbridgeErrorIntHandler(void);
extern void LeftHbridgeErrorIntHandler(void);
extern void SensorsInit(void);

//********************************************
// defined values for the Learning Switch Case
//********************************************
#define START_LEFT 		0
#define CYCLE_LEFT		1
#define LEARNING_LEFT	2
#define WAIT_LEFT		3
#define END_LEFT		4
#define LEFT_POS		6

#define START_RIGHT		5
#define CYCLE_RIGHT		8
#define LEARNING_RIGHT	9
#define WAIT_RIGHT		10
#define END_RIGHT		11
#define RIGHT_POS		13

#define LEARN_END		14

#endif /* SENSORS_H_ */
