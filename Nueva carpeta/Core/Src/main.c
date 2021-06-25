/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "keys.h"
#include "sapi.h"
#include "task.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RATE 1000
#define LED_RATE pdMS_TO_TICKS(RATE)

#define WELCOME_MSG  "Ejercicio D_6.\r\n"
#define USED_UART UART_USB
#define UART_RATE 115200
#define MALLOC_ERROR "Malloc Failed Hook!\n"
#define MSG_ERROR_SEM "Error al crear los semaforos.\r\n"
#define LED_ERROR LEDR
#define DURACION_DEL_PROGRAMA 9000000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define LED_COUNT   sizeof(leds_t)/sizeof(leds_t[0])
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Definitions for defaultTask */

/* USER CODE BEGIN PV */
gpioMap_t leds_t[] = {LEDB,LED1,LED2,LED3};
gpioMap_t gpio_t[] = {GPIO7,GPIO5,GPIO3,GPIO1};
extern t_key_config keys_config[];
QueueHandle_t xQueue1;
TaskHandle_t handle_keys_service_task;
TaskHandle_t handle_tarea_topo[4];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
TickType_t keys_get_diff();
void keys_clear_diff();
void tarea_topo( void* taskParmPtr );
void tarea_principal( void* taskParmPtr );
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  printf( WELCOME_MSG );
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  keys_init();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  xQueue1 = xQueueCreate( 10, sizeof( int ) );
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  taskENTER_CRITICAL();
  printf("- se crea tarea principal \n\r");
  taskEXIT_CRITICAL();
  BaseType_t res = xTaskCreate(
    tarea_principal,                     // Funcion de la tarea a ejecutar
    ( const char * )"tarea_principal",   // Nombre de la tarea como String amigable para el usuario
    configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
    (void *)0,                          // Parametros de tarea
    tskIDLE_PRIORITY+1,         // Prioridad de la tarea
    0                           // Puntero a la tarea creada en el sistema
  );
  // Gestion de errores
  configASSERT( res == pdPASS );
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  vTaskStartScheduler();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA9 PA10 PA11 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* ******************************************************************************** */
void tarea_principal( void* taskParmPtr )
{
  int tiempo_maximo=50;
  int si_no = 0;
  while(si_no == 0){
    int i=0;
    for(;i<LED_COUNT;i++){
      if(tiempo_maximo<(int)keys_get_diff(i)){
		si_no=1;
        taskENTER_CRITICAL();
        printf("EL JUEGO COMIENZA: %d\n\r",(int)keys_get_diff(i));
        taskEXIT_CRITICAL();
      }
    }
  }
	//-----------------------------
  uint32_t i;
  for ( i = 0 ; i < LED_COUNT ; i++ )
  {
    taskENTER_CRITICAL();
    printf("- se crea tarea topo i\n\r");
    taskEXIT_CRITICAL();
    BaseType_t res = xTaskCreate(
      tarea_topo,                     // Funcion de la tarea a ejecutar
      ( const char * )"tarea_topo",   // Nombre de la tarea como String amigable para el usuario
      configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
      (void *)i,                          // Parametros de tarea
      tskIDLE_PRIORITY+1,         // Prioridad de la tarea
      &handle_tarea_topo[i]           // Puntero a la tarea creada en el sistema
    );
    // Gestion de errores
    configASSERT( res == pdPASS );
  }

  taskENTER_CRITICAL();
  printf("-- inicia: tarea_principal - %d\n\r",(int)taskParmPtr);
  taskEXIT_CRITICAL();
  int sumatoria=0;
  int elemento_de_cola=0;
  int tiempo = (int)xTaskGetTickCount();
  for(;;){
    taskENTER_CRITICAL();
    //printf("tamaño de cola: %d\tsumatoria: %d\n\r",(int)uxQueueSpacesAvailable(xQueue1),sumatoria);
    taskEXIT_CRITICAL();
    while((int)uxQueueSpacesAvailable(xQueue1)<10)
    {
      xQueueReceive(xQueue1,&elemento_de_cola,0);
      sumatoria = sumatoria + elemento_de_cola;
      taskENTER_CRITICAL();
      //printf("--- sumatoria: %d\n\r",sumatoria);
      taskEXIT_CRITICAL();
    }
    if(tiempo + DURACION_DEL_PROGRAMA < (int)xTaskGetTickCount())
    {
      vTaskDelete(handle_keys_service_task);
      for(int i=0;i<LED_COUNT;i++)
      {
        vTaskDelete(handle_tarea_topo[i]);
      }
      for(;;)
      {
        taskENTER_CRITICAL();
        printf("EL JUEGO TERMINO: %d\n\r",(int)xTaskGetTickCount());
        taskEXIT_CRITICAL();
        vTaskDelay(1000);
      }
    }
  }
}
/* ******************************************************************************** */

/*==================[definiciones de funciones externas]=====================*/

// Implementacion de funcion de la tarea
void tarea_topo( void* taskParmPtr )
{
  taskENTER_CRITICAL();
  printf("-- inicia: tarea_topo - %d\n\r",(int)taskParmPtr);
  taskEXIT_CRITICAL();
  uint32_t index = ( uint32_t ) taskParmPtr;
  int xLastWakeTime = 0;
  int tiempo_random_arriba = 2000;
  int tiempo_random_abajo = 2000;
  int enviar_a_cola = 0;
  int N = 2000; // numero aleatorio maximo
  for(;;){
    gpioWrite(leds_t[index], ON);
    tiempo_random_arriba = rand() % (N+1); // tiene que ser random
    xLastWakeTime = (int)xTaskGetTickCount();
    if(xSemaphoreTake(keys_config[index].sem_btn, tiempo_random_arriba) == TRUE)
    {
      enviar_a_cola = tiempo_random_arriba + xLastWakeTime - (int)xTaskGetTickCount();
      xQueueSend(xQueue1, &enviar_a_cola, 0);
      taskENTER_CRITICAL();
      printf("--- premio: %d\n\r",enviar_a_cola);
      taskEXIT_CRITICAL();
    }
    else
    {
      enviar_a_cola = -10;
      xQueueSend(xQueue1, &enviar_a_cola, 0);
      taskENTER_CRITICAL();
      //printf("--- castigo: %d\n\r",enviar_a_cola);
      taskEXIT_CRITICAL();
    }
    gpioWrite(leds_t[index], OFF);
    //tiempo_random_abajo  = rand() % (N+1);// tiene que ser random
    xLastWakeTime = (int)xTaskGetTickCount();
    if(xSemaphoreTake(keys_config[index].sem_btn, tiempo_random_abajo) == TRUE)
    {
      enviar_a_cola = -20;
      xQueueSend(xQueue1, &enviar_a_cola, 0);
      vTaskDelayUntil(&xLastWakeTime, tiempo_random_abajo);
      taskENTER_CRITICAL();
      //printf("--- castigo: %d\n\r",enviar_a_cola);
      taskEXIT_CRITICAL();
    }
    else
    {
      //nada
    }
  }
}

/* hook que se ejecuta si al necesitar un objeto dinamico, no hay memoria disponible */
void vApplicationMallocFailedHook()
{
  printf( MALLOC_ERROR );
  configASSERT( 0 );
}
/*==================[fin del archivo]========================================*/
int __io_putchar(int ch)
{
  uint8_t c[1];
  c[0] = ch & 0x00FF;
  HAL_UART_Transmit(&huart2, &*c, 1, 10);
  return ch;
}

int _write(int file,char *ptr, int len)
{
  int DataIdx;
  for(DataIdx= 0; DataIdx< len; DataIdx++)
  {
    __io_putchar(*ptr++);
  }
  return len;
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
