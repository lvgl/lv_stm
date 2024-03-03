/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : Target/lcd_conf.h
  * Description        : This file provides code for the configuration
  *                      of the LCD instances.
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
#ifndef __LCD_CONF_H__
#define __LCD_CONF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* DMA Instance handlers */
extern  MDMA_HandleTypeDef                  hmdma_memtomem;
extern  DMA_HandleTypeDef                   hdma_memtomem_dma1_stream0;
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* Number of LCD instances */
#define LCD_INSTANCES_NBR                   1U

/* DMA Instance handlers */
#define BSP_LCD_USE_MDMA					0
#if (BSP_LCD_USE_MDMA)
#define hLCDDMA                             hmdma_memtomem
#else
#define hLCDDMA                             hdma_memtomem_dma1_stream0
#endif

/* RESET Pin mapping */
#define LCD_RESET_GPIO_PORT                 GPIOH
#define LCD_RESET_GPIO_PIN                  GPIO_PIN_2

/* DISP Pin mapping */
#define LCD_DISP_GPIO_PORT                  GPIOI
#define LCD_DISP_GPIO_PIN                   GPIO_PIN_7

/* BL CTRL Pin mapping */
#define LCD_BL_CTRL_GPIO_PORT               GPIOE
#define LCD_BL_CTRL_GPIO_PIN                GPIO_PIN_12

/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* Chip Reset macro definition */
#define LCD_RST_LOW()                       WRITE_REG(GPIOH->BSRR, (uint32_t)GPIO_PIN_2 << 16)
#define LCD_RST_HIGH()                      WRITE_REG(GPIOH->BSRR, GPIO_PIN_2)
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif
#endif /* __LCD_CONF_H__ */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
