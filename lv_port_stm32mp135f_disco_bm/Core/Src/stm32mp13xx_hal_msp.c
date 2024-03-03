/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         Src/stm32mp13xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/**
* @brief LTDC MSP Initialization
* This function configures the hardware resources used in this example
* @param hltdc: LTDC handle pointer
* @retval None
*/
void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
{
  GPIO_InitTypeDef GPIO_Init_Structure;

  /* Enable the LTDC clocks */
  __HAL_RCC_LTDC_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /*
   * CLK  ----> D9  (AF13)
   * HSYNC----> C6  (AF14)
   * VSYNC----> G4  (AF11)
   * DE   ----> H9  (AF11)
   *
   * R2   ----> G7  (AF14)
   * R3   ----> B12 (AF13)
   * R4   ----> D14 (AF14)
   * R5   ----> E7  (AF14)
   * R6   ----> E13 (AF14)
   * R7   ----> E9  (AF14)
   *
   * G2   ----> H3  (AF14)
   * G3   ----> F3  (AF14)
   * G4   ----> D5  (AF14)
   * G5   ----> G0  (AF14)
   * G6   ----> C7  (AF14)
   * G7   ----> A15 (AF11)
   *
   * B2   ----> D10 (AF14)
   * B3   ----> F2  (AF14)
   * B4   ----> H14 (AF11)
   * B5   ----> E0  (AF14)
   * B6   ----> B6  (AF7)
   * B7   ----> F1  (AF13)
   * */

  /*** LTDC Pins configuration ***/
  /* ('D', 9, AF13) LCD_CLK */
  GPIO_Init_Structure.Pin       = GPIO_PIN_9;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_LCD;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /* ('C', 6, AF14) LCD_HSYNC */
  GPIO_Init_Structure.Pin       = GPIO_PIN_6;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOC, &GPIO_Init_Structure);

  /* ('G',  4, AF11) LCD_VSYNC */
  GPIO_Init_Structure.Pin       = GPIO_PIN_4;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF11_LCD;
  HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);

  /* ('H',  9, AF11) LCD_DE */
  GPIO_Init_Structure.Pin       = GPIO_PIN_9;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF11_LCD;
  HAL_GPIO_Init(GPIOH, &GPIO_Init_Structure);

  /* ('G', 7, AF14) LCD_R2 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_7;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);

  /* ('B', 12, AF13) LCD_R3 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_12;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_LCD;
  HAL_GPIO_Init(GPIOB, &GPIO_Init_Structure);

  /* ('D', 14, AF14) LCD_R4 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_14;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /* ('E',  7, AF14) LCD_R5 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_7;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

  /* ('E',  13, AF14) LCD_R6 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_13;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

  /* ('E',  9, AF14) LCD_R7 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_9;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

  /* ('H', 13, AF14) LCD_G2 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_13;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOH, &GPIO_Init_Structure);

  /* ('F',  3, AF14) LCD_G3 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_3;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOF, &GPIO_Init_Structure);

  /* ('D',  5, AF14) LCD_G4 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_5;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /* ('G',  0, AF14) LCD_G5 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_0;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);

  /* ('C',  7, AF14) LCD_G6 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_7;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOC, &GPIO_Init_Structure);

  /* ('A', 15, AF11) LCD_G7 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_15;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF11_LCD;
  HAL_GPIO_Init(GPIOA, &GPIO_Init_Structure);

  /* ('D',  10, AF14) LCD_B2 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_10;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /* ('F',  2, AF14) LCD_B3 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_2;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOF, &GPIO_Init_Structure);

  /* ('H',  14, AF11) LCD_B4 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_14;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF11_LCD;
  HAL_GPIO_Init(GPIOH, &GPIO_Init_Structure);

  /* ('E',  0, AF14) LCD_B5 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_0;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF14_LCD;
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

  /* ('B',  6, AF7)  LCD_B6 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_6;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOB, &GPIO_Init_Structure);

  /* ('F',  1, AF13) LCD_B7 */
  GPIO_Init_Structure.Pin       = GPIO_PIN_1;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_LCD;
  HAL_GPIO_Init(GPIOF, &GPIO_Init_Structure);

  /* GIC configuration for LTDC interrupt */
  IRQ_SetMode(LTDC_IRQn, IRQ_MODE_DOMAIN_NONSECURE);
  IRQ_SetPriority(LTDC_IRQn, 5);
  IRQ_Enable(LTDC_IRQn);

  /* Enable LTDC reset state */
  __HAL_RCC_LTDC_FORCE_RESET();

  /* Release LTDC from reset state */
  __HAL_RCC_LTDC_RELEASE_RESET();

}

/**
* @brief LTDC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hltdc: LTDC handle pointer
* @retval None
*/
void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* hltdc)
{
  /* Disable LTDC block */
  __HAL_LTDC_DISABLE(hltdc);

  /*
   * CLK  ----> D9  (AF13)
   * HSYNC----> C6  (AF14)
   * VSYNC----> G4  (AF11)
   * DE   ----> H9  (AF11)
   *
   * R2   ----> G7  (AF14)
   * R3   ----> B12 (AF13)
   * R4   ----> D14 (AF14)
   * R5   ----> E7  (AF14)
   * R6   ----> E13 (AF14)
   * R7   ----> E9  (AF14)
   *
   * G2   ----> H3  (AF14)
   * G3   ----> F3  (AF14)
   * G4   ----> D5  (AF14)
   * G5   ----> G0  (AF14)
   * G6   ----> C7  (AF14)
   * G7   ----> A15 (AF11)
   *
   * B2   ----> D10 (AF14)
   * B3   ----> F2  (AF14)
   * B4   ----> H4  (AF11)
   * B5   ----> E0  (AF14)
   * B6   ----> B6  (AF7)
   * B7   ----> F1  (AF13)
   * */

  /* ('D', 9, AF13) LCD_CLK */
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_9);

  /* ('C', 6, AF14) LCD_HSYNC */
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6);

  /* ('G',  4, AF11) LCD_VSYNC */
  HAL_GPIO_DeInit(GPIOG, GPIO_PIN_4);

  /* ('H',  9, AF11) LCD_DE */
  HAL_GPIO_DeInit(GPIOH, GPIO_PIN_9);

  /* ('H',  8, AF13) LCD_R2 */
  HAL_GPIO_DeInit(GPIOG, GPIO_PIN_7);

  /* ('B', 12, AF14) LCD_R3 */
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12);

  /* ('D',  14, AF14) LCD_R4 */
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_14);

  /* ('E',  7, AF14) LCD_R5 */
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7);

  /* ('E',  13, AF14) LCD_R6 */
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_13);

  /* ('E',  9, AF14) LCD_R7 */
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_9);

  /* ('H', 13, AF14) LCD_G2 */
  HAL_GPIO_DeInit(GPIOH, GPIO_PIN_13);

  /* ('F',  3, AF14) LCD_G3 */
  HAL_GPIO_DeInit(GPIOF, GPIO_PIN_3);

  /* ('D',  5, AF11) LCD_G4 */
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5);

  /* ('G',  0, AF14) LCD_G5 */
  HAL_GPIO_DeInit(GPIOG, GPIO_PIN_0);

  /* ('C',  7, AF14) LCD_G6 */
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_7);

  /* ('A', 15, AF11) LCD_G7 */
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);

  /* ('D',  10, AF14) LCD_B2 */
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_10);

  /* ('F',  2, AF14) LCD_B3 */
  HAL_GPIO_DeInit(GPIOF, GPIO_PIN_2);

  /* ('H',  14, AF14) LCD_B4 */
  HAL_GPIO_DeInit(GPIOH, GPIO_PIN_14);

  /* ('E',  0, AF14) LCD_B5 */
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_0);

  /* ('B',  6, AF7)  LCD_B6 */
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

  /* ('F',  1, AF13) LCD_B7 */
  HAL_GPIO_DeInit(GPIOF, GPIO_PIN_1);

  /* Disable IRQ */
  IRQ_Disable(LTDC_IRQn);

  /* Disable LTDC clock */
  __HAL_RCC_LTDC_CLK_DISABLE();
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
