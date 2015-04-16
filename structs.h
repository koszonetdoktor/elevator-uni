/*
 * structs.h
 *
 *  Created on: 2015.03.12.
 *      Author: Elek
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_

struct CABIN{
	unsigned char	floor_number;  // how man floor does a cabin have
	int				floor_distance[10]; // distance of the floors from the ground
	int 			sign_counter; // encoder signal counter
	unsigned char 	direction;	// direction of the cabin
	char			position ;  // ez fogja a JÖVŐBEN!! jelképezni azt hogy az encoder alapján hol is áll a kabin, lehet nem az encoder alapján hanme inkább, a szint érzékelőnek lesz interruptja(ez már így működik)
	unsigned char	targetlist[10]; // contains the targets,
	unsigned char	upward_calls[10];  // contains the upward calls
	unsigned char	downward_calls[10]; // containing the downward calls
	unsigned char	call_aims[10];  // containing the aims of calls
	float			desired_velocity;
	int 			final_velocity; // this is the final value of the velocity ina movment cycle
	unsigned char 	acceleration; // it is 1 when need acceleration, and 0 if not, so it is a switch used in the acceleration interrupt
	float 			velocity; // thsi is the real velocity
	unsigned char	next_target; // it it -1 when there is no targer
	float				a; // this is the acceleration
	unsigned char 	moving; // this indicates, whetehr it moving or not
	float 			dec_v;
	int 			test;
	int				astep_counter;  // this counts how many steps needs to accelerat to the desired level
};

#define UP 1
#define DOWN 2

extern struct CABIN left;
extern struct CABIN right;


#endif /* STRUCTS_H_ */
