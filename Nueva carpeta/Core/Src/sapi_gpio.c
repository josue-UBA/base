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


int gpioRead(gpioMap_t pin)
{
  GPIO_PinState value = 0;
  if( pin == TEC1 )
  {
    value = HAL_GPIO_ReadPin(GPIOA, IN_1);
  }
  else if( pin == TEC2 )
  {
    value = HAL_GPIO_ReadPin(GPIOA, IN_2);
  }
  else if( pin == TEC3 )
  {
    value = HAL_GPIO_ReadPin(GPIOA, IN_3);
  }
  else if( pin == TEC4 )
  {
    value = HAL_GPIO_ReadPin(GPIOA, IN_4);
  }
  else
  {
    value =  GPIO_PIN_RESET;
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

void mi_printf(char* texto, char* par)
{
	uint8_t buffer[40]="";
	//snprintf((char *)dataT, m, texto);
	int a = 10, b = 20, c;
    c = a + b;
    sprintf(buffer, texto, par);
	int m = sizeof(buffer) / sizeof(buffer[0]);
	HAL_UART_Transmit(&huart2, (uint8_t *)buffer, m, HAL_MAX_DELAY);
}
