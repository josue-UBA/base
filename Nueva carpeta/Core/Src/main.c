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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define GPIO1 GPIO_PIN_7
#define GPIO5 GPIO_PIN_5
#define GPIO7 GPIO_PIN_6
#define GPIO3 GPIO_PIN_6
#define LEDB GPIO_PIN_1
#define LED_USED GPIO_PIN_0
#define GPIO_DEBUG GPIO_PIN_4

#define LED_RATE_MS 500 / portTICK_RATE_MS
#define LOADING_RATE_MS 250

#define LED_RATE pdMS_TO_TICKS(LED_RATE_MS)
#define LOADING_RATE pdMS_TO_TICKS(LOADING_RATE_MS)

#define MODE 1
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
/* USER CODE BEGIN PV */
TaskHandle_t taskHandle1;
TaskHandle_t taskHandle2;
TaskHandle_t taskHandle3;
TaskHandle_t taskHandle4;
int bandera = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
void delay_con_while(uint32_t);
void mi_funcion(uint16_t, int);
void heart_beat( void* taskParmPtr ); // Prendo A - Apago A
void loading_1( void* taskParmPtr );  // Prendo A - Apago A - Prendo B - Apago B .... hasta D
void loading_2( void* taskParmPtr ); // Prendo A - Prendo B - ... - Apago A - Apago B - ...
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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

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
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  #if MODE == 1
  BaseType_t res = xTaskCreate(heart_beat, ( const char * )"heart_beat", configMINIMAL_STACK_SIZE*2, 0, tskIDLE_PRIORITY+1, 0);
  #endif
  #if MODE == 2
  BaseType_t res = xTaskCreate(loading_1, ( const char * )"loading_1", configMINIMAL_STACK_SIZE*2, 0, tskIDLE_PRIORITY+1, 0);
  #endif
  #if MODE == 3
  BaseType_t res = xTaskCreate(loading_2, ( const char * )"loading_2", configMINIMAL_STACK_SIZE*2, 0, tskIDLE_PRIORITY+1, 0);
  #endif
  if(res == pdFAIL)
  {
    while(pdTRUE)
    {
    }
  }
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  vTaskStartScheduler();
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
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
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA4 PA5 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void delay_con_while(uint32_t ms)
{
  volatile uint32_t dummy;
  /* obtengo el tick absoluto */
  TickType_t base = xTaskGetTickCount();
  /* calculo el tick absoluto para destrabar el while */
  TickType_t target = base + ms; /* no esta contemplado el wrap arraond */
  while (xTaskGetTickCount() < target)
  {
  	dummy++;
  }
}

void mi_funcion(uint16_t a, int b)
{
  for(int i=0; i < b*2; i++)
  {
  	HAL_GPIO_TogglePin(GPIOA, a);
  	delay_con_while(500);
  }
}
// Implementacion de funcion de la tarea
void heart_beat( void* taskParmPtr )
{
  // ---------- CONFIGURACIONES ------------------------------
  HAL_GPIO_WritePin( GPIOA, GPIO_DEBUG, GPIO_PIN_SET );
  // ---------- REPETIR POR SIEMPRE --------------------------
  while( pdTRUE )
  {
    HAL_GPIO_WritePin(GPIOA, LED_USED, GPIO_PIN_SET );
    HAL_GPIO_WritePin(GPIOA, GPIO_DEBUG, GPIO_PIN_SET );
    vTaskDelay( LED_RATE_MS );

    HAL_GPIO_WritePin(GPIOA, LED_USED, GPIO_PIN_RESET );
    HAL_GPIO_WritePin(GPIOA, GPIO_DEBUG, GPIO_PIN_RESET );
    vTaskDelay( LED_RATE_MS ); //NO USAR!!
  }
}

// Implementacion de funcion de la tarea
void loading_1( void* taskParmPtr )
{
  // ---------- CONFIGURACIONES ------------------------------
  uint16_t led = LEDB;
  uint16_t gpio = GPIO7;

  HAL_GPIO_WritePin(GPIOA, GPIO7, GPIO_PIN_SET );
  HAL_GPIO_WritePin(GPIOA, GPIO5, GPIO_PIN_SET );
  HAL_GPIO_WritePin(GPIOA, GPIO3, GPIO_PIN_SET );
  HAL_GPIO_WritePin(GPIOA, GPIO1, GPIO_PIN_SET );
  // ---------- REPETIR POR SIEMPRE --------------------------
  while( pdTRUE )
  {
	HAL_GPIO_WritePin(GPIOA, led, GPIO_PIN_SET );
	HAL_GPIO_WritePin(GPIOA, gpio, GPIO_PIN_SET );
    vTaskDelay( LOADING_RATE );

	HAL_GPIO_WritePin(GPIOA, led, GPIO_PIN_RESET );
	HAL_GPIO_WritePin(GPIOA, gpio, GPIO_PIN_RESET );
    /*
    if ( led == LED3 )
    {
      led = LEDB;
      gpio = GPIO7;
    }
    else
    {
      led = led + 1;
      gpio ++;
    }
    */
    vTaskDelay( LOADING_RATE ); //NO USAR!!
  }
}

// Implementacion de funcion de la tarea
void loading_2( void* taskParmPtr )
{
  // ---------- CONFIGURACIONES ------------------------------
  uint16_t led = LEDB;
  uint16_t gpio = GPIO7;

  HAL_GPIO_WritePin(GPIOA, GPIO7, GPIO_PIN_SET );
  HAL_GPIO_WritePin(GPIOA, GPIO5, GPIO_PIN_SET );
  HAL_GPIO_WritePin(GPIOA, GPIO3, GPIO_PIN_SET );
  HAL_GPIO_WritePin(GPIOA, GPIO1, GPIO_PIN_SET );
  // ---------- REPETIR POR SIEMPRE --------------------------
  while( pdTRUE )
  {
	HAL_GPIO_TogglePin(GPIOA, led );
    HAL_GPIO_TogglePin(GPIOA, gpio );
    /*
    if ( led == LED3 )
    {
      led = LEDB;
      gpio = GPIO7;
    }
    else
    {
      led = led + 1;
      gpio ++;
    }
    */
    vTaskDelay( LOADING_RATE );
  }
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
