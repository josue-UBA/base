/*
 * sapi_gpio.c
 *
 *  Created on: May 31, 2021
 *      Author: Elias
 */

#include "sapi.h"
#include "main.h"
void gpioWrite(gpioMap_t pin, int sapi_estado)
{
  GPIO_PinState estado = sapi_estado;
  if( pin == LEDB )
  {
    HAL_GPIO_WritePin(GPIOA, OUT_3, estado);
  }
  else if( pin == LED1 )
  {
    HAL_GPIO_WritePin(GPIOA, OUT_4, estado);
  }
  else if( pin == LED2 )
  {
    HAL_GPIO_WritePin(GPIOA, OUT_5, estado);
  }
  else if( pin == LED3 )
  {
    HAL_GPIO_WritePin(GPIOA, OUT_6, estado);
  }
}


GPIO_PinState gpioRead(gpioMap_t pin)
{
  if( pin == TEC1 )
  {
    return HAL_GPIO_ReadPin(GPIOA, IN_1);
  }
  else if( pin == TEC2 )
  {
	return HAL_GPIO_ReadPin(GPIOA, IN_2);
  }
  else if( pin == TEC3 )
  {
	return HAL_GPIO_ReadPin(GPIOA, IN_3);
  }
  else if( pin == TEC4 )
  {
	return HAL_GPIO_ReadPin(GPIOA, IN_4);
  }
  else
  {
    return GPIO_PIN_RESET;
  }
}
