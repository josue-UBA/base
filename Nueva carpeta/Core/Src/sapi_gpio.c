/*
 * sapi_gpio.c
 *
 *  Created on: May 31, 2021
 *      Author: Elias
 */

#include "sapi.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

void gpioToggle(gpioMap_t pin)
{
  switch(pin)
  {
#ifdef OUT_1
    case LEDR:
      HAL_GPIO_TogglePin(GPIOA, OUT_1);break;
#endif
#ifdef OUT_2
    case LEDG:
      HAL_GPIO_TogglePin(GPIOA, OUT_2);break;
#endif
#ifdef OUT_3
    case LEDB:
      HAL_GPIO_TogglePin(GPIOA, OUT_3);break;
#endif
#ifdef OUT_4
    case LED1:
      HAL_GPIO_TogglePin(GPIOA, OUT_4);break;
#endif
#ifdef OUT_5
    case LED2:
      HAL_GPIO_TogglePin(GPIOA, OUT_5);break;
#endif
#ifdef OUT_6
    case LED3:
      HAL_GPIO_TogglePin(GPIOA, OUT_6);break;
#endif
    default:
      break;
  }
}

void gpioWrite(gpioMap_t pin, int sapi_estado)
{
  GPIO_PinState estado = sapi_estado;
  switch(pin){
#ifdef OUT_1
  case LEDR:
	    HAL_GPIO_WritePin(GPIOA, OUT_1, estado);break;
#endif
#ifdef OUT_2
  case LEDG:
	    HAL_GPIO_WritePin(GPIOA, OUT_2, estado);break;
#endif
#ifdef OUT_3
  case LEDB:
	    HAL_GPIO_WritePin(GPIOA, OUT_3, estado);break;
#endif
#ifdef OUT_4
  case LED1:
	    HAL_GPIO_WritePin(GPIOA, OUT_4, estado);break;
#endif
#ifdef OUT_5
  case LED2:
	    HAL_GPIO_WritePin(GPIOA, OUT_5, estado);break;
#endif
#ifdef OUT_6
  case LED3:
	    HAL_GPIO_WritePin(GPIOA, OUT_6, estado);break;
#endif
  default:
	  break;
  }
}


int gpioRead(gpioMap_t pin)
{
  GPIO_PinState value = 0;
  switch(pin)
  {
#ifdef IN_1
    case TEC1:
      value = HAL_GPIO_ReadPin(GPIOA, IN_1);break;
#endif
#ifdef IN_2
    case TEC2:
      value = HAL_GPIO_ReadPin(GPIOA, IN_2);break;
#endif
#ifdef IN_3
    case TEC3:
      value = HAL_GPIO_ReadPin(GPIOA, IN_3);break;
#endif
#ifdef IN_4
    case TEC4:
      value = HAL_GPIO_ReadPin(GPIOA, IN_4);break;
#endif
    default:
      value =  GPIO_PIN_RESET;break;
  }
  /* ************************* */
  if(value == GPIO_PIN_SET)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}
