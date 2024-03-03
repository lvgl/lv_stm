/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : Target/lv_port_disp.c
  * Description        : This file provides display port for LVGL library using
  *                      the X-CUBE-DISPLAY LCD APIs
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
#include <assert.h>
#include "src/misc/lv_assert.h"
#include "lv_port_disp.h"
#include "main.h"

/*********************
 *      DEFINES
 *********************/
#define DISP_WIDTH                        480
#define DISP_HEIGHT                       272
#define LCD_BUF_SIZE                      (DISP_WIDTH * DISP_HEIGHT)
#define DISP_BUF_SIZE                     (DISP_WIDTH * DISP_HEIGHT)
#define DISP_ROTATED                      0

/* Use DMA value */
#define DISP_USE_DMA                      (DISP_ROTATED ? 1 : 0)
#define DISP_MIN_DMA_SIZE                 250

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  GLOBAL PROTOTYPES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_clean_dcache(lv_disp_drv_t *drv);
static void monitor_cb(struct _lv_disp_drv_t * disp_drv, uint32_t time, uint32_t px);
static void flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color);
#if (DISP_USE_DMA) && (!BSP_LCD_USE_MDMA)
static void DMA_TransferComplete(DMA_HandleTypeDef *hdma);
static void DMA_TransferError(DMA_HandleTypeDef *hdma);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/
__attribute__ ((aligned (32)))
static lv_color_t buf1[DISP_BUF_SIZE];

__attribute__ ((aligned (32)))
static lv_color_t buf2[DISP_BUF_SIZE];

#if (DISP_ROTATED)
__attribute__ ((aligned (32)))
static lv_color_t lcd_buf[LCD_BUF_SIZE];
#endif

static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t main_disp_drv;
static lv_disp_t *main_disp = NULL;
static uint32_t disp_pix_sz = 0;
static uint32_t disp_xsize = 0;
static uint32_t disp_ysize = 0;
static volatile bool disp_flush_enabled = true;
static volatile bool display_enabled = false;
static volatile bool drawing_allowed = true;
#if (DISP_USE_DMA == 1)
static int32_t x1_flush;
static int32_t y1_flush;
static int32_t x2_flush;
static int32_t y2_flush;
static int32_t y_fill_act;
static const lv_color_t * buf_to_flush;
#endif

/**********************
 *      MACROS
 **********************/
/*
** Set assert macro, if it has not been provided by the user.
*/
#define lv_port_disp_assert LV_ASSERT

#if !defined (lv_port_disp_assert)
  #define lv_port_disp_assert assert
#endif

#if defined(VSYNC_FREQ_Pin)
#define LCD_VSYNC_FREQ_LOW()                       WRITE_REG(VSYNC_FREQ_GPIO_Port->BSRR, VSYNC_FREQ_Pin)
#define LCD_VSYNC_FREQ_HIGH()                      WRITE_REG(VSYNC_FREQ_GPIO_Port->BSRR, (uint32_t)VSYNC_FREQ_Pin << 16)
#else
#define LCD_VSYNC_FREQ_LOW()
#define LCD_VSYNC_FREQ_HIGH()
#endif /* VSYNC_FREQ_Pin */

#if defined(FRAME_RATE_Pin)
#define LCD_FRAME_RATE_LOW()                       WRITE_REG(FRAME_RATE_GPIO_Port->BSRR, FRAME_RATE_Pin)
#define LCD_FRAME_RATE_HIGH()                      WRITE_REG(FRAME_RATE_GPIO_Port->BSRR, (uint32_t)FRAME_RATE_Pin << 16)
#else
#define LCD_FRAME_RATE_LOW()
#define LCD_FRAME_RATE_HIGH()
#endif /* VSYNC_FREQ_Pin */

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
  disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
  disp_flush_enabled = false;
}

/**
  * @brief  Signal transfert event.
  * @param  Instance LCD Instance
  * @param  Line counter
  * @retval None
  */
void BSP_LCD_SignalTearingEffectEvent(uint32_t Instance, uint8_t state, uint16_t Line)
{
  if (Instance == 0)
  {
    if(state)
    {
      /* Entering Active Area : de-allow drawing */
      drawing_allowed = false;
      LCD_VSYNC_FREQ_HIGH();
    }
    else
    {
      /* Exiting Active Area : allow drawing */
      drawing_allowed = true;
      LCD_VSYNC_FREQ_LOW();
    }
  }
}

void disp_init(void)
{
  display_enabled = false;
  disp_flush_enabled = true;
  disp_pix_sz = sizeof(lv_color_t);
  int32_t ret = BSP_LCD_Init(0, 0);
  lv_port_disp_assert((ret == BSP_ERROR_NONE) && "failed to initialize LCD");
  ret = BSP_LCD_GetXSize(0, &disp_xsize);
  lv_port_disp_assert((ret == BSP_ERROR_NONE) && "failed to get display XSize");
  ret = BSP_LCD_GetYSize(0, &disp_ysize);
  lv_port_disp_assert((ret == BSP_ERROR_NONE) && "failed to get display YSize");

#if (DISP_USE_DMA == 1) && (!BSP_LCD_USE_MDMA)
  ret = HAL_DMA_RegisterCallback(&hLCDDMA, HAL_DMA_XFER_CPLT_CB_ID, DMA_TransferComplete);
  lv_port_disp_assert((ret == HAL_OK) && "failed to registed DMA Complete callback");
  ret = HAL_DMA_RegisterCallback(&hLCDDMA, HAL_DMA_XFER_ERROR_CB_ID, DMA_TransferError);
  lv_port_disp_assert((ret == HAL_OK) && "failed to registed DMA Complete callback");
#endif /* DISP_USE_DMA */

#if (DISP_ROTATED)
  LTDC_Layer1->CFBAR = (uint32_t)lcd_buf;
#else
  LTDC_Layer1->CFBAR = (uint32_t)buf1;
#endif
  LTDC->SRCR = (uint32_t)LTDC_SRCR_IMR;

  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, DISP_BUF_SIZE);
  lv_disp_drv_init(&main_disp_drv);
  main_disp_drv.draw_buf = &draw_buf;
  main_disp_drv.hor_res = disp_xsize;
  main_disp_drv.ver_res = disp_ysize;
  main_disp_drv.rotated = (DISP_ROTATED ? LV_DISP_ROT_90 : LV_DISP_ROT_NONE);
  main_disp_drv.sw_rotate = (DISP_ROTATED ? 1 : 0);
  main_disp_drv.full_refresh = (DISP_ROTATED ? 0 : 1);
  main_disp_drv.direct_mode = (DISP_ROTATED ? 0 : 1);
  main_disp_drv.flush_cb = flush_cb;
  main_disp_drv.clean_dcache_cb = disp_clean_dcache;
  main_disp_drv.monitor_cb = monitor_cb;
  main_disp = lv_disp_drv_register(&main_disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void disp_clean_dcache(lv_disp_drv_t *drv)
{
  /*  Invalidate data cache */
  L1C_CleanInvalidateDCacheAll();
}

static void flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color)
{
  LCD_FRAME_RATE_HIGH();

  if(!disp_flush_enabled)
  {
    lv_disp_flush_ready(disp_drv);
    LCD_FRAME_RATE_LOW();
    return;
  }

  if(disp_drv->clean_dcache_cb)
  {
    disp_drv->clean_dcache_cb(disp_drv);
  }

  /* Wait until drawing is allowed */
  while (display_enabled && !drawing_allowed) { };

  if(disp_drv->full_refresh)
  {
    LTDC_Layer1->CFBAR = (uint32_t)color;
    /* Reload LTDC Configuration */
    LTDC->SRCR = (uint32_t)LTDC_SRCR_IMR;
    lv_disp_flush_ready(disp_drv);
    LCD_FRAME_RATE_LOW();
  }
  else
  {
#if (DISP_USE_DMA == 1)
    if ((disp_pix_sz * (area->x2 - area->x1)) >= DISP_MIN_DMA_SIZE)
    {
      HAL_StatusTypeDef ret = HAL_OK;
      lv_color_t * wp = (lv_color_t *) LTDC_Layer1->CFBAR;
      int32_t x1 = area->x1;
      int32_t x2 = area->x2;
      int32_t y1 = area->y1;
      int32_t y2 = area->y2;
      uint32_t length;
      /*Return if the area is out the screen*/

      if(x2 < 0) return;
      if(y2 < 0) return;
      if(x1 > disp_xsize - 1) return;
      if(y1 > disp_ysize - 1) return;

      /*Truncate the area to the screen*/
      int32_t act_x1 = x1 < 0 ? 0 : x1;
      int32_t act_y1 = y1 < 0 ? 0 : y1;
      int32_t act_x2 = x2 > disp_xsize - 1 ? disp_xsize - 1 : x2;
      int32_t act_y2 = y2 > disp_ysize - 1 ? disp_ysize - 1 : y2;

      x1_flush = act_x1;
      y1_flush = act_y1;
      x2_flush = act_x2;
      y2_flush = act_y2;
      buf_to_flush = color;
      y_fill_act = act_y1;
      length = (x2_flush - x1_flush + 1);

#if (BSP_LCD_USE_MDMA)
      ret = HAL_MDMA_Start_IT(&hLCDDMA, (uint32_t)buf_to_flush, (uint32_t)&wp[y_fill_act * disp_xsize + x1_flush], length, 1);
#else
      ret = HAL_DMA_Start_IT(&hLCDDMA,(uint32_t)buf_to_flush, (uint32_t)&wp[y_fill_act * disp_xsize + x1_flush], length);
#endif
      lv_port_disp_assert((ret == HAL_OK) && "failed to transfer data to LCD");
    }
    else
#endif
    {
      int32_t y;
      lv_color_t * rp = color;
      lv_color_t * wp = (lv_color_t *) LTDC_Layer1->CFBAR;
      lv_coord_t w = lv_area_get_width(area);

      wp += area->y1 * disp_drv->hor_res;
      wp += area->x1;
      for(y = area->y1; y <= area->y2; y++)
      {
        lv_memcpy(wp, rp, w * disp_pix_sz);
        wp += disp_drv->hor_res;
        rp += w;
      }
      lv_disp_flush_ready(disp_drv);
      LCD_FRAME_RATE_LOW();
    }
  }
}

/*
 * This callback is used to enable the display only after having the first frame drawn.
 */
static void monitor_cb(struct _lv_disp_drv_t * disp_drv, uint32_t time, uint32_t px)
{
  int32_t ret;
  if (!display_enabled)
  {
    BSP_LCD_WaitForTransferToBeDone(0);
    ret = BSP_LCD_DisplayOn(0);
    lv_port_disp_assert((ret == BSP_ERROR_NONE) && "failed to set display On");
    display_enabled = true;
  }
  /* Reset monitor_cb so this will not be called */
  disp_drv->monitor_cb = NULL;
}

#if (DISP_USE_DMA)
#if (BSP_LCD_USE_MDMA)
/**
  * @brief  Signal Transfer Event.
  * @param  Instance:     LCD Instance.
  */
void BSP_LCD_SignalTransferDone(uint32_t Instance)
{
  if (Instance == 0)
  {
	int32_t ret = BSP_ERROR_NONE;

	if(buf_to_flush == 0)
	  return;

	y_fill_act ++;

	if(y_fill_act > y2_flush) {
	  buf_to_flush = 0;
	  if(main_disp_drv.clean_dcache_cb)
	  {
		main_disp_drv.clean_dcache_cb(&main_disp_drv);
	  }
	  lv_disp_flush_ready(&main_disp_drv);
	  LCD_FRAME_RATE_LOW();
	}
	else
	{
	  uint32_t length = (x2_flush - x1_flush + 1);
	  buf_to_flush += length;
	  ret = HAL_MDMA_Start_IT(&hLCDDMA, (uint32_t)buf_to_flush, (uint32_t)&lcd_buf[y_fill_act * disp_xsize + x1_flush], length, 1);
	  lv_port_disp_assert((ret == HAL_OK) && "failed to transfer data to LCD");
	}
  }
}
#else
/**
  * @brief  DMA conversion complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
static void DMA_TransferComplete(DMA_HandleTypeDef *hdma)
{
  if (hdma)
  {
	int32_t ret = BSP_ERROR_NONE;
	lv_color_t * wp = (lv_color_t *) LTDC_Layer1->CFBAR;

	if(buf_to_flush == 0)
	  return;

	y_fill_act ++;

	if(y_fill_act > y2_flush) {
	  buf_to_flush = 0;
      if(main_disp_drv.clean_dcache_cb)
      {
        main_disp_drv.clean_dcache_cb(&main_disp_drv);
      }
	  lv_disp_flush_ready(&main_disp_drv);
	  LCD_FRAME_RATE_LOW();
	}
	else
	{
	  uint32_t length = (x2_flush - x1_flush + 1);
	  buf_to_flush += length;
	  ret = HAL_DMA_Start_IT(hdma,(uint32_t)buf_to_flush, (uint32_t)&wp[y_fill_act * disp_xsize + x1_flush], length);
	  lv_port_disp_assert((ret == HAL_OK) && "failed to transfer data to LCD");
	}
  }
}

/**
  * @brief  DMA conversion error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during DMA transfer
  * @retval None
  */
static void DMA_TransferError(DMA_HandleTypeDef *hdma)
{
  lv_port_disp_assert(false && "DMA transfer failed !!");
}
#endif
#endif
