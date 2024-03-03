/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : Target/lv_port_indev.c
  * Description        : This file provides input devices port for LVGL library
  *                      using the X-CUBE-DISPLAY KEY APIs
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

/*********************
 *      INCLUDES
 *********************/
#include "main.h"
#include "src/misc/lv_assert.h"
#include "lv_port_indev.h"

/*********************
 *      DEFINES
 *********************/
#define TS_INSTANCE		(0)
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void indev_read(lv_indev_drv_t *drv, lv_indev_data_t *data);

/**********************
 *  STATIC VARIABLES
 **********************/
static TS_Init_t hTS;
static TS_State_t  TS_State;
static lv_indev_drv_t indev_drv;                /*Descriptor of an input device driver*/
static int16_t last_x = 0;
static int16_t last_y = 0;

/**********************
 *      MACROS
 **********************/
/*
** Set assert macro, if it has not been provided by the user.
*/
#define lv_port_indev_assert LV_ASSERT

#if !defined (lv_port_indev_assert)
    #define lv_port_indev_assert assert
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize your input devices here
 */
void indev_init(void)
{
  int32_t ret;
  uint32_t x_size, y_size;
  uint8_t iterations = 10;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  hTS.Width = x_size;
  hTS.Height = y_size;
  hTS.Orientation = TS_SWAP_NONE;
  hTS.Accuracy = 0;

  do
  {
    ret = BSP_TS_Init(TS_INSTANCE, &hTS);
    HAL_Delay(20);
  } while ((ret != BSP_ERROR_NONE) && (iterations-- > 0));
  lv_port_indev_assert((ret == BSP_ERROR_NONE) && "failed to initialize TouchScreen");

  lv_indev_drv_init(&indev_drv);                  /*Basic initialization*/
  indev_drv.type = LV_INDEV_TYPE_POINTER;         /*The touchpad is pointer type device*/
  indev_drv.read_cb = indev_read;

  lv_indev_drv_register(&indev_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Read an input device
 * @param indev_id id of the input device to read
 * @param x put the x coordinate here
 * @param y put the y coordinate here
 * @return true: the device is pressed, false: released
 */
static void indev_read(lv_indev_drv_t *indev, lv_indev_data_t *data)
{
  /* Read your touchpad */
  BSP_TS_GetState(TS_INSTANCE, &TS_State);  /*Get touch state*/

  if(TS_State.TouchDetected)
  {
    data->point.x = TS_State.TouchX;
    data->point.y = TS_State.TouchY;
    last_x = data->point.x;
    last_y = data->point.y;
    data->state = LV_INDEV_STATE_PRESSED;
  }
  else
  {
    data->point.x = last_x;
    data->point.y = last_y;
    data->state = LV_INDEV_STATE_RELEASED;
  }
}
