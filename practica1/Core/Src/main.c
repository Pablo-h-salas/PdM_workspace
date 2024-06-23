/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
	* @autor 					: Pablo Horacio Salas (pablo.salas.94@gmail.com)
	*	@Descripcion 		: Ejercicio 1 PdM
	* @Dependencias		: STM32F4xx_HAL_Driver
	* @Comentarios		: Este proyecto contiene los puntos 1 y 2. El punto 1 se usa
											en el punto 2. 

  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
/* Defines- ------------------------------------------------------------------*/
#define PORT_LED 			GPIOB
#define PORT_BUTTON 	GPIOC
#define TIMEoN 				200
#define TIMEoFF				200
#define TIMEdEBOUNCE	30

/*----------------------------------------------------------------------------*/
/*												DEFINICION DE VARIABLES Y TIPOS 						  			*/
/*----------------------------------------------------------------------------*/
uint16_t LED[] = {GPIO_PIN_0, GPIO_PIN_7, GPIO_PIN_14};
uint16_t BUTTON = GPIO_PIN_13;
uint8_t mode = 0;

const uint8_t MAXLED = sizeof(LED)/sizeof(LED[0]);
const uint8_t MAXMODE = 2;
/*----------------------------------------------------------------------------*/
/*														PROTOTIPOS DE FUNCIONES 						  					*/
/*----------------------------------------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void LED_parpadeo(uint16_t led, uint32_t timeOn, uint32_t timeOff);
void read_button (uint16_t button, uint16_t timeDebounce);

/*----------------------------------------------------------------------------*/
/*																CUERPO DEL MAIN 							  						*/
/*----------------------------------------------------------------------------*/
int main(void)
{

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();


/*----------------------------------------------------------------------------*/
/*															LOOP INFINITO  							  								*/
/*											codigo principal del programa					  							*/
/*----------------------------------------------------------------------------*/

  while (1)
  {
			read_button(BUTTON,TIMEdEBOUNCE);
			
			switch (mode){
				case 0 	:
								/*PUNTO 1*/
								for(int i=0; i<MAXLED; i++){
								LED_parpadeo(LED[i], TIMEoN, TIMEoFF);
								}	
								break;
				case 1 	:
								for(int i=MAXLED-1; i>= 0; --i){
								LED_parpadeo(LED[i], TIMEoN, TIMEoFF);
								}
								break;
				//default:
			}
		
  }
}

/*----------------------------------------------------------------------------*/
/*														DEFINICION DE FUNCIONES 						  					*/
/*----------------------------------------------------------------------------*/
/**
	*@brief parpadeo de salida digital
	*@param led 		:pin del led 
	*@param timeOn 	:tiempo de encendido
	*@param timeOff :tiempo de apagado
	*/
void LED_parpadeo(uint16_t led, uint32_t timeOn, uint32_t timeOff){
			HAL_GPIO_WritePin(PORT_LED, led, GPIO_PIN_SET);
			HAL_Delay(timeOn);
			HAL_GPIO_WritePin(PORT_LED, led, GPIO_PIN_RESET);	
			HAL_Delay(timeOff);
}

/**
	*@brief Lectura de un pulsador
	*@param button 				:pin conectado al pulsador  
	*@param timeDebounce 	:tiempo de duracion del rebote
	*/
void read_button (uint16_t button, uint16_t timeDebounce){
			GPIO_PinState buttonState = HAL_GPIO_ReadPin(PORT_BUTTON, button);
			if (buttonState == GPIO_PIN_SET){
				HAL_Delay(timeDebounce);
				if (HAL_GPIO_ReadPin(PORT_BUTTON, BUTTON)== GPIO_PIN_SET){
					if (mode == MAXMODE-1){
						mode = 0;
					}else{
						mode ++;
					}
				}
			}else{
				// sin condicion
			}
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_14|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB14 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_14|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
