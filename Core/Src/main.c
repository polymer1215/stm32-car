/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../Bsp/Inc/motor.h"
#include "../../Bsp/Inc/timer.h"
#include "../../App/Inc/MoveManager_C.h"
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
#define PTO_BUF_LEN_MAX 50
uint8_t RxBuffer[PTO_BUF_LEN_MAX];
float vision_error = 0;
int x, y, w, h;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch) {
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

int _write(int file, char *ptr, int len) {
  HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, 0xFFFF);
  return len;
}

void Process_K230_Data(uint8_t *data, uint16_t len) {
    uint8_t *header = NULL;
    for (uint16_t i = 0; i < len; i++) {
        if (data[i] == '$') {
            header = &data[i];
            break;
        }
    }

    if (header == NULL) {
        return;
    }

    uint8_t *tail = NULL;
    int remaining = (data + len) - header;
    for (uint16_t i = 0; i < remaining; i++) {
        if (header[i] == '#') {
            tail = &header[i];
            break;
        }
    }

    if (tail == NULL) {
        return;
    }

    uint16_t frame_len = (tail - header) + 1;
    char parse_buf[PTO_BUF_LEN_MAX];

    if (frame_len >= PTO_BUF_LEN_MAX) {
        return;
    }

    memcpy(parse_buf, header, frame_len);
    parse_buf[frame_len - 1] = '\0'; // 将 '#' 替换为结束符以供 strtok 使用

    char *token = strtok(parse_buf + 1, ","); // 跳过 '$'
    if (token == NULL) {
        return;
    }
    int pto_len = atoi(token);

    if (pto_len != frame_len) {
    }

    // 5. 解析 ID 字段
    token = strtok(NULL, ",");
    if (token == NULL) {
        return;
    }

    int pto_id = atoi(token);
    if (pto_id != 11) {
        return;
    }

    // 6. 解析坐标数据 (x, y, w, h)
    char *sx = strtok(NULL, ",");
    char *sy = strtok(NULL, ",");
    char *sw = strtok(NULL, ",");
    char *sh = strtok(NULL, ",");

    if (sx && sy && sw && sh) {
        x = atoi(sx);
        y = atoi(sy);
        w = atoi(sw);
        h = atoi(sh);

        vision_error = (float)((x + w/2) - 320);

    } else {
        return;
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
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  uint32_t last_tick = 0;
  MoveManager_Init();

  HAL_UART_DMAStop(&huart1);
  HAL_StatusTypeDef status = HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxBuffer, PTO_BUF_LEN_MAX);
  if (status != HAL_OK) {
    printf("UART DMA Init Failed! Status: %d\n", status);
    __HAL_UART_CLEAR_OREFLAG(&huart1);
  }

  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxBuffer, PTO_BUF_LEN_MAX);
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (timerMillis - last_tick >= 1000) {
      // printf("L\ndeg:%d\npwm:%d\nR\ndeg:%d\npwm:%d\n", (int32_t)leftMotorDeg, leftMotorPwm,
      //   (int32_t)rightMotorDeg, rightMotorPwm);
      printf("target_center_x: %d\n", x);
      last_tick += 1000;
    }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
  if (htim->Instance == TIM4) {
    updateTimer();
    updateAllMotor();
    // MoveManager_Update();
  }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if (huart->Instance == USART1) {
    // 直接处理接收到的整包数据
    Process_K230_Data(RxBuffer, Size);

    // 重新开启接收
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxBuffer, PTO_BUF_LEN_MAX);
    __HAL_DMA_DISABLE_IT(huart->hdmarx, DMA_IT_HT);
  }
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
#ifdef USE_FULL_ASSERT
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
