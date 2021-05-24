/*=============================================================================
 * Copyright (c) 2021, Franco Bucafusco <franco_bucafusco@yahoo.com.ar>
 * 					   Martin N. Menendez <mmenendez@fi.uba.ar>
 * All rights reserved.
 * License: Free
 * Date: 2021/10/03
 * Version: v1.2
 *===========================================================================*/

/*==================[ Inclusions ]============================================*/
#include "FreeRTOS.h"
#include "task.h"
#include "sapi_button.h"
#include "keys.h"
#include "main.h"

/*=====[ Definitions of private data types ]===================================*/
t_key_config  keys_config;

/*=====[Definition macros of private constants]==============================*/
#define DEBOUNCE_TIME   40
#define DEBOUNCE_TIME_MS pdMS_TO_TICKS(DEBOUNCE_TIME)
/*=====[Prototypes (declarations) of private functions]======================*/

static void keys_ButtonError();
static void buttonPressed();
static void buttonReleased();

/*=====[Definitions of private global variables]=============================*/

/*=====[Definitions of public global variables]==============================*/
t_key_data keys_data;

/*=====[prototype of private functions]=================================*/
void task_tecla( void* taskParmPtr );

/*=====[Implementations of public functions]=================================*/
TickType_t get_diff( uint32_t index )
{
    TickType_t tiempo;
    tiempo = keys_data.time_diff;

    return tiempo;
}

void clear_diff()
{
    keys_data.time_diff = KEYS_INVALID_TIME;
}

void keys_Init( void )
{
	keys_config.tecla = TEC1;
    BaseType_t res;

    keys_data.state          = STATE_BUTTON_UP;  // Set initial state
    keys_data.time_down      = KEYS_INVALID_TIME;
    keys_data.time_up        = KEYS_INVALID_TIME;
    keys_data.time_diff      = KEYS_INVALID_TIME;

    // Crear tareas en freeRTOS
    res = xTaskCreate (
              task_tecla,					// Funcion de la tarea a ejecutar
              ( const char * )"task_tecla",	// Nombre de la tarea como String amigable para el usuario
              configMINIMAL_STACK_SIZE*2,	// Cantidad de stack de la tarea
              0,							// Parametros de tarea
              tskIDLE_PRIORITY+1,			// Prioridad de la tarea
              0							// Puntero a la tarea creada en el sistema
          );

    // Gestión de errores
    configASSERT( res == pdPASS );
}

// keys_ Update State Function
void keys_Update()
{
    switch( keys_data.state )
    {
        case STATE_BUTTON_UP:
            /* CHECK TRANSITION CONDITIONS */

            if( HAL_GPIO_ReadPin(GPIOA, keys_config.tecla) == GPIO_PIN_SET )
            {
                keys_data.state = STATE_BUTTON_FALLING;
            }
            break;

        case STATE_BUTTON_FALLING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */
            if( HAL_GPIO_ReadPin(GPIOA, keys_config.tecla) == GPIO_PIN_SET )
            {
                keys_data.state = STATE_BUTTON_DOWN;

                /* ACCION DEL EVENTO !*/
                buttonPressed();
            }
            else
            {
                keys_data.state = STATE_BUTTON_UP;
            }

            /* LEAVE */
            break;

        case STATE_BUTTON_DOWN:
            /* CHECK TRANSITION CONDITIONS */
            if( HAL_GPIO_ReadPin(GPIOA, keys_config.tecla) != GPIO_PIN_SET )
            {
                keys_data.state = STATE_BUTTON_RISING;
            }
            break;

        case STATE_BUTTON_RISING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */

            if( HAL_GPIO_ReadPin(GPIOA, keys_config.tecla) != GPIO_PIN_SET )
            {
                keys_data.state = STATE_BUTTON_UP;

                /* ACCION DEL EVENTO ! */
                buttonReleased();
            }
            else
            {
                keys_data.state = STATE_BUTTON_DOWN;
            }

            /* LEAVE */
            break;

        default:
            keys_ButtonError();
            break;
    }
}

/*=====[Implementations of private functions]================================*/

/* accion de el evento de tecla pulsada */
static void buttonPressed()
{
    TickType_t current_tick_count = xTaskGetTickCount();

    keys_data.time_down = current_tick_count;
}

/* accion de el evento de tecla liberada */
static void buttonReleased()
{
    TickType_t current_tick_count = xTaskGetTickCount();

    keys_data.time_up    = current_tick_count;
    keys_data.time_diff  = keys_data.time_up - keys_data.time_down;
}

static void keys_ButtonError()
{
    keys_data.state = BUTTON_UP;
}

/*=====[Implementations of private functions]=================================*/
void task_tecla( void* taskParmPtr )
{
    while( pdTRUE )
    {
        keys_Update();
        vTaskDelay( DEBOUNCE_TIME_MS );
    }
}
