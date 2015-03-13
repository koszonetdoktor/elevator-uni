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
	int				position;  // ez fogja a JÖVŐBEN!! jelképezni azt hogy az encoder alapján hol is áll a kabin
	unsigned char	targetlist[10]; // contains the targets,
};

extern struct CABIN left;
extern struct CABIN right;


#endif /* STRUCTS_H_ */
