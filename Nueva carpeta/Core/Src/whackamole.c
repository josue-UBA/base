/*
 * hola.c
 *
 *  Created on: Jun 25, 2021
 *      Author: Elias
 */

#include "whackamole.h"
#include "random.h"
void whackamole_service_logic(void *taskParmPtr) {
	//debugger_con_printf("-- inicia: tarea_principal\n\r",0);
	/*
	 *
	 * Primero, esperamos a que se presione alguna tecla por al menos 50 ms
	 *
	 * */
	int tiempo_maximo = 50;
	/* flag que nos ayudara a determinar si es que se presiono una tecla por mas de 50 ms o no */

	int si_no = 0;
	while (si_no == 0) {
		int i = 0;
		/* Iterara hasta que la tarea key indique que alguna tecla se presiono mas de 50 ms */

		for (; i < LED_COUNT; i++) {
			if (tiempo_maximo < (int) keys_get_diff(i)) {
				si_no = 1;
				taskENTER_CRITICAL();
				debugger_con_printf("EL JUEGO COMIENZA: %d\n\r",
						(int) keys_get_diff(i));
				taskEXIT_CRITICAL();
			}
		}
	}
	/* Se limpia todos los semaforos por si es que se dio alguno antes del inicio del juego */
	for (int i = 0; i < LED_COUNT; i++) {
		xSemaphoreTake(keys_config[i].sem_btn, 0);
	}
	/*
	 *
	 * Si se presiono alguna tecla, procedemos a crear las tareas topos
	 *
	 * */
	whackamole_init();
	/* Luego preparamos las variables */

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
			debugger_con_printf("su puntaje es %d\n\r", sumatoria);
		}
		/*
		 *
		 * Se verifica si es que el programa esta activo mas de
		 * "DURACION_DEL_PROGRAMA" segundos.
		 *
		 * */
		if (tiempo + WAM_GAMEPLAY_TIMEOUT < (int) xTaskGetTickCount()) {
			/* Si es que se supero el tiempo, se procede a eliminar la tarea key... */
			vTaskDelete(handle_keys_service_task);
			/* ... y todas las taras topo. */
			for (int i = 0; i < LED_COUNT; i++) {
				vTaskDelete(handle_tarea_topo[i]);
			}
			debugger_con_printf("EL JUEGO TERMINO. DURO %d SEGUNDOS\n\r",
					((int) xTaskGetTickCount() - tiempo) / 1000);
			debugger_con_printf("SU PUNTAJE ES %d !!!!\n\r", sumatoria);
			for (;;) {
				vTaskDelay(1000);
			}
		}
	}
}
/* ******************************************************************************** */

/*==================[definiciones de funciones externas]=====================*/

// Implementacion de funcion de la tarea
void mole_service_logic(void *taskParmPtr) {
	uint32_t index = (uint32_t) taskParmPtr;
	int xLastWakeTime = 0;
	int tiempo_arriba;
	int tiempo_abajo;
	int enviar_a_cola = 0;
	for (;;) {
		gpioWrite(leds_t[index], ON);
		tiempo_arriba = random(WAM_MOLE_OUTSIDE_MIN_TIME, WAM_MOLE_OUTSIDE_MAX_TIME); //rand() % (N + 1); // tiene que ser random
		xLastWakeTime = (int) xTaskGetTickCount();
		/* si se presiono cuando el topo salio, entonces... */
		if (xSemaphoreTake(keys_config[index].sem_btn,
				tiempo_arriba) == TRUE) {
			/* ecuacion que determina el puntaje */
			enviar_a_cola = whackamole_points_success(tiempo_arriba, ((int) xTaskGetTickCount() - xLastWakeTime));
			xQueueSend(xQueue1, &enviar_a_cola, 0);
			debugger_con_printf("Se premia con %d puntos!!\n\r", enviar_a_cola);

		}

		/* si NO se presiono cuando el topo salio, entonces se penaliza con -10
		 * puntos. */

		else {
			enviar_a_cola = whackamole_points_miss();
			xQueueSend(xQueue1, &enviar_a_cola, 0);
			debugger_con_printf(
					"el topo %d salio, pero no lo atrapaste... -10 puntos\n\r",
					index);

		}
		gpioWrite(leds_t[index], OFF);
		tiempo_abajo = random(WAM_MOLE_SHOW_MIN_TIME, WAM_MOLE_SHOW_MAX_TIME); // tiene que ser random
		xLastWakeTime = (int) xTaskGetTickCount();
		/* el topo ya se escondio. El jugador no deberia pulsar la tecla.
		 * Si lo hace, entonces se le penaliza con -20 */
		if (xSemaphoreTake(keys_config[index].sem_btn,
				tiempo_abajo) == TRUE) {
			enviar_a_cola = whackamole_points_no_mole();
			xQueueSend(xQueue1, &enviar_a_cola, 0);
			vTaskDelayUntil(&xLastWakeTime, tiempo_abajo);
			debugger_con_printf(
					"no presiones el agujero del topo %d !!! -20 puntos\n\r",
					index);
		}

		/* en caso el jugador no haga nada, entonces continua el flujo sin
		 * sumar ni restar puntos. */
	}
}

void whackamole_init() {

	uint32_t i;
	xQueue1 = xQueueCreate(10, sizeof(int));
	for (i = 0; i < LED_COUNT; i++) {
		debugger_con_printf("- se crea tarea topo %d\n\r", i);
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
/**
 @brief devuelve el puntaje por haber perdido al mole

 @return uint32_t
 */
uint32_t whackamole_points_miss() {
	return -10;
}

/**
 @brief devuelve el puntaje por haber martillado cuando no habia mole

 @return uint32_t
 */
uint32_t whackamole_points_no_mole() {
	return -20;
}

void debugger_con_printf(char *texto, int numero) {
	taskENTER_CRITICAL();
	printf(texto, numero);
	taskEXIT_CRITICAL();
}

uint32_t whackamole_points_success( TickType_t tiempo_afuera,TickType_t tiempo_reaccion_usuario )
{
	return ( WAM_MOLE_OUTSIDE_MAX_TIME*WAM_MOLE_OUTSIDE_MAX_TIME ) /( tiempo_afuera*tiempo_reaccion_usuario );
}
