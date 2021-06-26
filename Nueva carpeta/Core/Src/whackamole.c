/*
 * hola.c
 *
 *  Created on: Jun 25, 2021
 *      Author: Elias
 */

#include "whackamole.h"

void whackamole_service_logic(void *taskParmPtr) {
	int tiempo_maximo = 50;
	int si_no = 0;
	while (si_no == 0) {
		int i = 0;
		for (; i < LED_COUNT; i++) {
			if (tiempo_maximo < (int) keys_get_diff(i)) {
				si_no = 1;
				taskENTER_CRITICAL();
				printf("EL JUEGO COMIENZA: %d\n\r", (int) keys_get_diff(i));
				taskEXIT_CRITICAL();
			}
		}
	}
	//-----------------------------
	whackamole_init();
	taskENTER_CRITICAL();
	printf("-- inicia: tarea_principal - %d\n\r", (int) taskParmPtr);
	taskEXIT_CRITICAL();
	int sumatoria = 0;
	int elemento_de_cola = 0;
	int tiempo = (int) xTaskGetTickCount();
	for (;;) {
		taskENTER_CRITICAL();
		//printf("tamaño de cola: %d\tsumatoria: %d\n\r",(int)uxQueueSpacesAvailable(xQueue1),sumatoria);
		taskEXIT_CRITICAL();
		while ((int) uxQueueSpacesAvailable(xQueue1) < 10) {
			xQueueReceive(xQueue1, &elemento_de_cola, 0);
			sumatoria = sumatoria + elemento_de_cola;
			taskENTER_CRITICAL();
			//printf("--- sumatoria: %d\n\r",sumatoria);
			taskEXIT_CRITICAL();
		}
		if (tiempo + DURACION_DEL_PROGRAMA < (int) xTaskGetTickCount()) {
			vTaskDelete(handle_keys_service_task);
			for (int i = 0; i < LED_COUNT; i++) {
				vTaskDelete(handle_tarea_topo[i]);
			}
			for (;;) {
				taskENTER_CRITICAL();
				printf("EL JUEGO TERMINO: %d\n\r", (int) xTaskGetTickCount());
				taskEXIT_CRITICAL();
				vTaskDelay(1000);
			}
		}
	}
}
/* ******************************************************************************** */

/*==================[definiciones de funciones externas]=====================*/

// Implementacion de funcion de la tarea
void mole_service_logic(void *taskParmPtr) {
	taskENTER_CRITICAL();
	printf("-- inicia: tarea_topo - %d\n\r", (int) taskParmPtr);
	taskEXIT_CRITICAL();
	uint32_t index = (uint32_t) taskParmPtr;
	int xLastWakeTime = 0;
	int tiempo_random_arriba = 2000;
	int tiempo_random_abajo = 2000;
	int enviar_a_cola = 0;
	int N = 2000; // numero aleatorio maximo
	for (;;) {
		gpioWrite(leds_t[index], ON);
		tiempo_random_arriba = random( 1000,6000 );//rand() % (N + 1); // tiene que ser random
		xLastWakeTime = (int) xTaskGetTickCount();
		if (xSemaphoreTake(keys_config[index].sem_btn,
				tiempo_random_arriba) == TRUE) {
			enviar_a_cola = tiempo_random_arriba + xLastWakeTime
					- (int) xTaskGetTickCount();
			xQueueSend(xQueue1, &enviar_a_cola, 0);
			taskENTER_CRITICAL();
			printf("--- premio: %d\n\r", enviar_a_cola);
			taskEXIT_CRITICAL();
		} else {
			enviar_a_cola = -10;
			xQueueSend(xQueue1, &enviar_a_cola, 0);
			taskENTER_CRITICAL();
			//printf("--- castigo: %d\n\r",enviar_a_cola);
			taskEXIT_CRITICAL();
		}
		gpioWrite(leds_t[index], OFF);
		//tiempo_random_abajo  = rand() % (N+1);// tiene que ser random
		xLastWakeTime = (int) xTaskGetTickCount();
		if (xSemaphoreTake(keys_config[index].sem_btn,
				tiempo_random_abajo) == TRUE) {
			enviar_a_cola = -20;
			xQueueSend(xQueue1, &enviar_a_cola, 0);
			vTaskDelayUntil(&xLastWakeTime, tiempo_random_abajo);
			taskENTER_CRITICAL();
			//printf("--- castigo: %d\n\r",enviar_a_cola);
			taskEXIT_CRITICAL();
		} else {
			//nada
		}
	}
}


void whackamole_init(){

	uint32_t i;
	xQueue1 = xQueueCreate(10, sizeof(int));
	for (i = 0; i < LED_COUNT; i++) {
		taskENTER_CRITICAL();
		printf("- se crea tarea topo i\n\r");
		taskEXIT_CRITICAL();
		BaseType_t res = xTaskCreate(mole_service_logic, // Funcion de la tarea a ejecutar
				(const char*) "mole_service_logic", // Nombre de la tarea como String amigable para el usuario
				configMINIMAL_STACK_SIZE * 2, // Cantidad de stack de la tarea
				(void*) i,                          // Parametros de tarea
				tskIDLE_PRIORITY + 1,         // Prioridad de la tarea
				&handle_tarea_topo[i] // Puntero a la tarea creada en el sistema
				);
		// Gestion de errores
		configASSERT(res == pdPASS);
	}

}

