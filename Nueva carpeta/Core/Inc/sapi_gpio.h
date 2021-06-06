/*
 * sapi_gpio.h
 *
 *  Created on: May 31, 2021
 *      Author: Elias
 */

#ifndef INC_SAPI_GPIO_H_
#define INC_SAPI_GPIO_H_

#include "sapi_peripheral_map.h"
#include "main.h"
void gpioWrite(gpioMap_t, int);
GPIO_PinState gpioRead(gpioMap_t);

#endif /* INC_SAPI_GPIO_H_ */
