/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "MLX90621_API.h"
#include "MLX90621_I2C_Driver.h"
#include "can_interface.hpp"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

static constexpr float emissivity = 0.98f;
static constexpr float tr = 15.0f;

static volatile bool mlx_receive_flag = true;
static volatile bool process_and_send_flag = false;

static uint8_t mlx90621ToAverage[8] = {0};
static uint8_t eeMLX90621[256];
static paramsMLX90621 mlx90621;
static uint16_t mlx90621Frame[66];
static float mlx90621To[64];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void led_reset(){
	HAL_GPIO_WritePin(LED_OK_GPIO_Port, LED_OK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_WAR1_GPIO_Port, LED_WAR1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_WAR2_GPIO_Port, LED_WAR2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_ERR_GPIO_Port, LED_ERR_Pin, GPIO_PIN_RESET);
}

static void mlx_receive()
{
	if(MLX90621_GetFrameData(mlx90621Frame) != HAL_OK){
		  HAL_GPIO_WritePin(LED_ERR_GPIO_Port, LED_ERR_Pin, GPIO_PIN_SET);
		  return;
	}
}

static void process_and_send()
{
	MLX90621_I2C_Seq_Interpret_Data();
	MLX90621_CalculateTo(mlx90621Frame, &mlx90621, emissivity, tr, mlx90621To);
	if(MLX90621_AverageTo(mlx90621To, mlx90621ToAverage)){
		HAL_GPIO_WritePin(LED_WAR2_GPIO_Port, LED_WAR2_Pin, GPIO_PIN_SET);
	}

	PUTM_CAN::WheelTemp_main ttsMessage{
		  .wheelTemp = {
				  mlx90621ToAverage[0],
				  mlx90621ToAverage[1],
				  mlx90621ToAverage[2],
				  mlx90621ToAverage[3],
				  mlx90621ToAverage[4],
				  mlx90621ToAverage[5],
				  mlx90621ToAverage[6],
				  mlx90621ToAverage[7]
		  }
	};
	auto ttsMainFrame = PUTM_CAN::Can_tx_message<PUTM_CAN::WheelTemp_main>(ttsMessage, PUTM_CAN::can_tx_header_WHEELTEMP_MAIN);

	if(ttsMainFrame.send(hcan1) != HAL_OK){
		  HAL_GPIO_WritePin(LED_ERR_GPIO_Port, LED_ERR_Pin, GPIO_PIN_SET);
	}
	else{
		  HAL_GPIO_WritePin(LED_OK_GPIO_Port, LED_OK_Pin, GPIO_PIN_SET);
	}
}

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
  MX_CAN1_Init();
  MX_I2C2_Init();
  MX_TIM1_Init();

  /* USER CODE BEGIN 2 */

  if(HAL_CAN_Start(&hcan1) != HAL_OK ||
     HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK ||
     MLX90621_DumpEE(eeMLX90621)!= HAL_OK ||
	 MLX90621_Configure(eeMLX90621) != HAL_OK ||
	 MLX90621_ExtractParameters(eeMLX90621, &mlx90621) != HAL_OK ||
	 HAL_TIM_Base_Start_IT(&htim1) != HAL_OK)
  {
	  HAL_GPIO_WritePin(LED_ERR_GPIO_Port, LED_ERR_Pin, GPIO_PIN_SET);
	  return HAL_ERROR;
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1){

	  if(mlx_receive_flag == true){
		  led_reset();
		  mlx_receive();
		  mlx_receive_flag = false;
	  }

	  if(process_and_send_flag == true){
		  process_and_send();
		  process_and_send_flag = false;
	  }

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 30;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim1)
  {
	  mlx_receive_flag = true;
  }
}

void HAL_I2C_MasterTxCpltCallback (I2C_HandleTypeDef * hi2c)
{
	if(MLX90621_I2C_Seq_Receive_IT() != HAL_OK){
		HAL_GPIO_WritePin(LED_ERR_GPIO_Port, LED_ERR_Pin, GPIO_PIN_SET);
	}
}

void HAL_I2C_MasterRxCpltCallback (I2C_HandleTypeDef * hi2c)
{
	process_and_send_flag = true;
}

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
