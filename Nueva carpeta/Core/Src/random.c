/*
 * random.c
 *
 *  Created on: Jun 25, 2021
 *      Author: Elias
 */
#include "random.h"
#include "main.h"

uint32_t random(int32_t min, int32_t max) {
	if (min >= max) {
		return min;
	}

	//printf( "%d %d %d\n", min, max, ( max-min ) + min );

	return rand() % (max - min) + min;
}
