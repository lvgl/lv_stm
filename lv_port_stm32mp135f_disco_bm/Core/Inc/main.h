/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Core/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32mp13xx_hal.h"
#include "stm32mp13xx_ll_etzpc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32mp13xx_disco.h"
#include "stm32mp13xx_disco_ts.h"
#include "stm32mp13xx_disco_stpmic1.h"
#include "lcd_io.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define MX_MODEL                "STM32MP13XX"
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_RESET_Pin GPIO_PIN_2
#define LCD_RESET_GPIO_Port GPIOH
#define LCD_BL_CTRL_Pin GPIO_PIN_12
#define LCD_BL_CTRL_GPIO_Port GPIOE
#define LCD_DISP_Pin GPIO_PIN_7
#define LCD_DISP_GPIO_Port GPIOI

#if 0 /* Enable this if you want to do Vsync and Frame Rate measurements */
#define VSYNC_FREQ_Pin GPIO_PIN_10	/* EXP_GPIO7: CN8, Pin 26 / GND: CN8, Pin 20 */
#define VSYNC_FREQ_GPIO_Port GPIOF
#define FRAME_RATE_Pin GPIO_PIN_5	/* EXP_GPIO12: CN8, Pin 32 / GND: CN8, Pin 30 */
#define FRAME_RATE_GPIO_Port GPIOE
#endif
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

