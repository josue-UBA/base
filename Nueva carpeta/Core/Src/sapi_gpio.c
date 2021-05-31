/*
 * sapi_gpio.c
 *
 *  Created on: May 31, 2021
 *      Author: Elias
 */

#include "sapi_gpio.h"
#include "main.h"
void gpioWrite(gpioMap_t pin, GPIO_PinState estado)
{
  if( pin == LEDB )
  {
    HAL_GPIO_WritePin(GPIOA, OUT_3, estado);
  }
}


GPIO_PinState gpioRead(gpioMap_t pin)
{
  if( pin == TEC1 )
  {
    return HAL_GPIO_ReadPin(GPIOA, IN_1);
  }
  else
  {
    return GPIO_PIN_RESET;
  }
}
