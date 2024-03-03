/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file
  * @author  MCD Application Team
  * @version V2.0.0
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
  /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  __RTE_COMPONENTS_H__
#define  __RTE_COMPONENTS_H__

/* Defines ------------------------------------------------------------------*/
/* Device Header File */
#define CMSIS_device_header "stm32mp13xx_hal.h"
/* LVGL.LVGL.8.3.11 */
/*! \brief Enable LVGL */
#define RTE_GRAPHICS_LVGL
/*! \brief use extra themes, widgets and layouts */
#define RTE_GRAPHICS_LVGL_USE_EXTRA_THEMES
/*! \brief enable the demo official bencharmk */
#define LV_USE_DEMO_BENCHMARK 1
/*! \brief enable demo:widgets support */
#define LV_USE_DEMO_WIDGETS 1
/*! \brief enable the demo stress */
#define LV_USE_DEMO_STRESS 1
/*! \brief enable the demo music */
#define LV_USE_DEMO_MUSIC 1
/*! \brief enable the demo keypad_encoder support */
#define LV_USE_DEMO_KEYPAD_AND_ENCODER 1

#endif /* __RTE_COMPONENTS_H__ */
