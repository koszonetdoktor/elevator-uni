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
volatile unsigned char swi2;

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
#define START_RIGHT		5
#define WAIT_STOP		6

#endif /* SENSORS_H_ */
