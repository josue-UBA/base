/*=============================================================================
 * Copyright (c) 2021, Franco Bucafusco <franco_bucafusco@yahoo.com.ar>
 * 					   Martin N. Menendez <mmenendez@fi.uba.ar>
 * All rights reserved.
 * License: Free
 * Date: 2021/10/03
 * Version: v1.2
 *===========================================================================*/

/*==================[ Inclusions ]============================================*/
#include "keys.h"

#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"

#include "queue.h"

/*=====[ Definitions of private data types ]===================================*/

/*=====[Definition macros of private constants]==============================*/
#define DEBOUNCE_TIME   40
#define DEBOUNCE_TIME_TICKS pdMS_TO_TICKS(DEBOUNCE_TIME)
/*=====[Prototypes (declarations) of private functions]======================*/

static void keys_reset( uint32_t index );
static void keys_event_handler_button_pressed( uint32_t index );
static void keys_event_handler_button_release( uint32_t index );

/*=====[Definitions of private global variables]=============================*/
const t_key_config  keys_config[] = { TEC1 };

#define KEY_COUNT   sizeof(keys_config)/sizeof(keys_config[0])

t_key_data keys_data[KEY_COUNT];

/*=====[Definitions of public global variables]==============================*/

extern QueueHandle_t queue_tec_pulsada;

/*=====[prototype of private functions]=================================*/
void keys_service_task( void* taskParmPtr );

/*=====[Implementations of public functions]=================================*/

/**
   @brief Obtiene el tiempo de la ultima pulsacion de la tecla indicada.

   @param index
   @return TickType_t
 */
TickType_t keys_get_diff( uint32_t index )
{
	TickType_t tiempo;

	taskENTER_CRITICAL();
	tiempo = keys_data[index].time_diff;
	taskEXIT_CRITICAL();

	return tiempo;
}

/**
   @brief Borra el tiempo de la ultima pulsacion de la tecla indicada.

   @param index
 */
void keys_clear_diff( uint32_t index )
{
	taskENTER_CRITICAL();
	keys_data[index].time_diff = KEYS_INVALID_TIME;
	taskEXIT_CRITICAL();
}


/**
   @brief Inicializa el driver

 */
void keys_init( void )
{
	BaseType_t res;
	uint32_t i;

	for ( i = 0 ; i < KEY_COUNT ; i++ )
	{
		keys_data[i].state          = BUTTON_UP;  // Set initial state
		keys_data[i].time_down      = KEYS_INVALID_TIME;
		keys_data[i].time_up        = KEYS_INVALID_TIME;
		keys_data[i].time_diff      = KEYS_INVALID_TIME;
	}
	// Crear tareas en freeRTOS
	res = xTaskCreate (
			  keys_service_task,					// Funcion de la tarea a ejecutar
			  ( const char * )"keys_service_task",	// Nombre de la tarea como String amigable para el usuario
			  configMINIMAL_STACK_SIZE*2,	// Cantidad de stack de la tarea
			  0,							// Parametros de tarea
			  tskIDLE_PRIORITY+1,			// Prioridad de la tarea
			  0							// Puntero a la tarea creada en el sistema
		  );

	// Gestión de errores
	configASSERT( res == pdPASS );
}

// keys_ Update State Function

/**
   @brief Keys SM for polled operation

   @param index
 */
void keys_Update( uint32_t index )
{
	switch( keys_data[index].state )
	{
		case STATE_BUTTON_UP:
			/* CHECK TRANSITION CONDITIONS */
			if( !gpioRead( keys_config[index].tecla ) )
			{
				keys_data[index].state = STATE_BUTTON_FALLING;
			}
			break;

		case STATE_BUTTON_FALLING:
			/* ENTRY */

			/* CHECK TRANSITION CONDITIONS */
			if( !gpioRead( keys_config[index].tecla ) )
			{
				keys_data[index].state = STATE_BUTTON_DOWN;

				/* ACCION DEL EVENTO !*/
				keys_event_handler_button_pressed( index );
			}
			else
			{
				keys_data[index].state = STATE_BUTTON_UP;
			}

			/* LEAVE */
			break;

		case STATE_BUTTON_DOWN:
			/* CHECK TRANSITION CONDITIONS */
			if( gpioRead( keys_config[index].tecla ) )
			{
				keys_data[index].state = STATE_BUTTON_RISING;
			}
			break;

		case STATE_BUTTON_RISING:
			/* ENTRY */

			/* CHECK TRANSITION CONDITIONS */

			if( gpioRead( keys_config[index].tecla ) )
			{
				keys_data[index].state = STATE_BUTTON_UP;

				/* ACCION DEL EVENTO ! */
				keys_event_handler_button_release( index );
			}
			else
			{
				keys_data[index].state = STATE_BUTTON_DOWN;
			}

			/* LEAVE */
			break;

		default:
			keys_reset( index );
			break;
	}
}

/*=====[Implementations of private functions]================================*/

/**
   @brief   Manejador del evento de tecla pulsada

   @param event_data
 */
static void keys_event_handler_button_pressed( uint32_t index )
{
	TickType_t current_tick_count = xTaskGetTickCount();

	taskENTER_CRITICAL();
	keys_data[index].time_down = current_tick_count;
	taskEXIT_CRITICAL();
}

/**
   @brief   Manejador del evento de tecla liberada

   @param event_data
 */
static void keys_event_handler_button_release( uint32_t index )
{
	TickType_t current_tick_count = xTaskGetTickCount();

	taskENTER_CRITICAL();
	keys_data[index].time_up    = current_tick_count;
	keys_data[index].time_diff  = keys_data[index].time_up - keys_data[index].time_down;
	taskEXIT_CRITICAL();

	if ( keys_data[index].time_diff > 0 )
	{
		xQueueSend( queue_tec_pulsada, &( keys_data[index].time_diff ),  portMAX_DELAY  );
	}

}

/**
   @brief Restarts the button SM

   @param index
 */
static void keys_reset( uint32_t index )
{
	taskENTER_CRITICAL();
	keys_data[index].state = BUTTON_UP;
	taskEXIT_CRITICAL();
}

/*=====[Implementations of private functions]=================================*/

/**
   @brief Implementacion de tarea que le da servicio al driver

   @param taskParmPtr
 */
void keys_service_task( void* taskParmPtr )
{
	uint32_t i;
	while( TRUE )
	{
		for ( i = 0 ; i < KEY_COUNT ; i++ )
		{
			keys_Update( i );
		}
		vTaskDelay( DEBOUNCE_TIME_TICKS );
	}
}
