/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CLK_CTRL1_Pin GPIO_PIN_11
#define CLK_CTRL1_GPIO_Port GPIOF
#define CLK_CTRL2_Pin GPIO_PIN_12
#define CLK_CTRL2_GPIO_Port GPIOF
#define AMPCTRL0_Pin GPIO_PIN_0
#define AMPCTRL0_GPIO_Port GPIOG
#define AMPCTRL2_Pin GPIO_PIN_7
#define AMPCTRL2_GPIO_Port GPIOE
#define AMPCTRL3_Pin GPIO_PIN_8
#define AMPCTRL3_GPIO_Port GPIOE
#define AMPCTRL1_Pin GPIO_PIN_9
#define AMPCTRL1_GPIO_Port GPIOE
#define AMPCTRL4_Pin GPIO_PIN_10
#define AMPCTRL4_GPIO_Port GPIOE
#define CLK_CTRL0_Pin GPIO_PIN_11
#define CLK_CTRL0_GPIO_Port GPIOE
#define VCC_T50K_Pin GPIO_PIN_13
#define VCC_T50K_GPIO_Port GPIOE
#define VCC_T30K_Pin GPIO_PIN_14
#define VCC_T30K_GPIO_Port GPIOE
#define VCC_T20K_Pin GPIO_PIN_10
#define VCC_T20K_GPIO_Port GPIOB
#define VCC_T10K_Pin GPIO_PIN_11
#define VCC_T10K_GPIO_Port GPIOB
#define ERROR_Pin GPIO_PIN_12
#define ERROR_GPIO_Port GPIOB
#define LOCAL_Pin GPIO_PIN_13
#define LOCAL_GPIO_Port GPIOB
#define VCC_CAPAMP3_Pin GPIO_PIN_8
#define VCC_CAPAMP3_GPIO_Port GPIOD
#define DATA_Pin GPIO_PIN_12
#define DATA_GPIO_Port GPIOD
#define VCC_CAPAMP0_Pin GPIO_PIN_3
#define VCC_CAPAMP0_GPIO_Port GPIOD
#define VCC_CAPAMP1_Pin GPIO_PIN_4
#define VCC_CAPAMP1_GPIO_Port GPIOD
#define VCC_PA8_CTRL_Pin GPIO_PIN_6
#define VCC_PA8_CTRL_GPIO_Port GPIOD
#define VCC_CAPAMP2_Pin GPIO_PIN_7
#define VCC_CAPAMP2_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */
/* Define one bit mask */
#define BIT0                    ((uint32_t)0x00000001)
#define BIT1                    ((uint32_t)0x00000002)
#define BIT2                    ((uint32_t)0x00000004)
#define BIT3                    ((uint32_t)0x00000008)
#define BIT4                    ((uint32_t)0x00000010)
#define BIT5                    ((uint32_t)0x00000020)
#define BIT6                    ((uint32_t)0x00000040)
#define BIT7                    ((uint32_t)0x00000080)
#define BIT8                    ((uint32_t)0x00000100)
#define BIT9                    ((uint32_t)0x00000200)
#define BIT10                   ((uint32_t)0x00000400)
#define BIT11                   ((uint32_t)0x00000800)
#define BIT12                   ((uint32_t)0x00001000)
#define BIT13                   ((uint32_t)0x00002000)
#define BIT14                   ((uint32_t)0x00004000)
#define BIT15                   ((uint32_t)0x00008000)
#define BIT16                   ((uint32_t)0x00010000)
#define BIT17                   ((uint32_t)0x00020000)
#define BIT18                   ((uint32_t)0x00040000)
#define BIT19                   ((uint32_t)0x00080000)
#define BIT20                   ((uint32_t)0x00100000)
#define BIT21                   ((uint32_t)0x00200000)
#define BIT22                   ((uint32_t)0x00400000)
#define BIT23                   ((uint32_t)0x00800000)
#define BIT24                   ((uint32_t)0x01000000)
#define BIT25                   ((uint32_t)0x02000000)
#define BIT26                   ((uint32_t)0x04000000)
#define BIT27                   ((uint32_t)0x08000000)
#define BIT28                   ((uint32_t)0x10000000)
#define BIT29                   ((uint32_t)0x20000000)
#define BIT30                   ((uint32_t)0x40000000)
#define BIT31                   ((uint32_t)0x80000000)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
