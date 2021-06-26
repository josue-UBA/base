/*
 * whackamole.h
 *
 *  Created on: Jun 25, 2021
 *      Author: Elias
 */

#ifndef INC_WHACKAMOLE_H_
#define INC_WHACKAMOLE_H_

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include "whackamole.h"
#include "random.h"
#include "sapi.h"
#include "keys.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#define WAM_GAMEPLAY_TIMEOUT        20000   //gameplay time
#define WAM_MOLE_SHOW_MAX_TIME      6000
#define WAM_MOLE_SHOW_MIN_TIME      1000
#define WAM_MOLE_OUTSIDE_MAX_TIME   2000
#define WAM_MOLE_OUTSIDE_MIN_TIME   500
#define DURACION_DEL_PROGRAMA 20000
#define LED_COUNT   4

QueueHandle_t xQueue1;
TaskHandle_t handle_keys_service_task;
TaskHandle_t handle_tarea_topo[4];
extern gpioMap_t leds_t[];
extern gpioMap_t gpio_t[];
extern t_key_config keys_config[];

typedef struct {
	gpioMap_t led;          //led asociado al mole otros recursos para cada mole
} mole_t;

void whackamole_init();
uint32_t whackamole_points_success(TickType_t tiempo_afuera,
		TickType_t tiempo_reaccion_usuario);
uint32_t whackamole_points_miss();
uint32_t whackamole_points_no_mole();
void whackamole_service_logic(void *pvParameters);
void mole_service_logic(void *pvParameters);
void debugger_con_printf(char *texto, int numero);
uint32_t whackamole_points_success( TickType_t tiempo_afuera,TickType_t tiempo_reaccion_usuario );


#endif /* INC_WHACKAMOLE_H_ */
