/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : Target/lcd_io.c
  * Description        : This file provides code for the configuration
  *                      of the LCD Driver instances.
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
#include "lcd_os.h"
#include "lcd_io.h"

/** @addtogroup DISPLAY
  * @brief      DISPLAY Software Expansion Pack.
  * @{
  */
/** @defgroup LCD_Driver LCD Driver
  * @brief    LCD Driver API.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

#define BSP_LCD_CHECK_PARAMS(Instance)

/* Exported variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */
static uint16_t lcd_offset = 0;
/* USER CODE END EV */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern LTDC_HandleTypeDef hltdc;
static uint32_t lcd_int_active_line;
static uint32_t lcd_int_porch_line;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
__STATIC_INLINE void LCD_CleanInvalidateDCache(void);
#if (BSP_LCD_USE_MDMA)
static void DMA_TxCpltCallback(MDMA_HandleTypeDef *hdma);
static void DMA_TxErrorCallback(MDMA_HandleTypeDef *hdma);
__STATIC_INLINE HAL_StatusTypeDef MDMA_List_Start_IT(MDMA_HandleTypeDef *hmdma, uint8_t node_cnt, uint8_t *src, uint8_t *dst, uint32_t data_size);
#endif
/* USER CODE END PFP */

int32_t LCD_IO_GetTick(void);
int32_t LCD_IO_Delay(uint32_t Delay);

/* Deprecated functions ------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @defgroup LCD_Exported_Functions LCD Exported Functions
  * @brief    LCD Drivers API.
  * @{
  */
/**
  * @brief  Initializes the LCD Controller.
  * @param  Instance:     LCD Instance.
  * @param  Orientation:  LCD Orientation.
  *         This parameter can be one of the following values:
  *          @arg @ref LCD_ORIENTATION_PORTRAIT           Portrait orientation choice of LCD screen
  *          @arg @ref LCD_ORIENTATION_LANDSCAPE          Landscape orientation choice of LCD screen
  *          @arg @ref LCD_ORIENTATION_PORTRAIT_ROT180    Portrait rotated 180° orientation choice of LCD screen
  *          @arg @ref LCD_ORIENTATION_LANDSCAPE_ROT180   Landscape rotated 180° orientation choice of LCD screen
  * @retval int32_t:      BSP status.
  */
int32_t BSP_LCD_Init(uint32_t Instance, uint32_t Orientation)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  if ((Instance >= LCD_INSTANCES_NBR ) || (Orientation > LCD_ORIENTATION_LANDSCAPE_ROT180))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* LCD OS Initialize */
    if(LCD_OS_Initialize(0) != LCD_OS_ERROR_NONE)
    {
      ret = BSP_ERROR_NO_INIT;
    }
    else
    {
      /* USER CODE BEGIN BSP_LCD_Init */
      LCD_RST_LOW();
      LCD_IO_Delay(1);
      LCD_RST_HIGH();
      LCD_IO_Delay(10);
      lcd_int_active_line = ((uint32_t )(hltdc.Instance->BPCR & 0x7FF) - 1);
      lcd_int_porch_line = ((hltdc.Instance->AWCR & 0x7FF) - 1);
#if (BSP_LCD_USE_MDMA)
      HAL_MDMA_RegisterCallback(&hLCDDMA, HAL_MDMA_XFER_CPLT_CB_ID, DMA_TxCpltCallback);
      HAL_MDMA_RegisterCallback(&hLCDDMA, HAL_MDMA_XFER_ERROR_CB_ID, DMA_TxErrorCallback);
#endif
      ret = BSP_ERROR_NONE;
      /* USER CODE END BSP_LCD_Init */
    }
  }

  return ret;
}

/**
  * @brief  De-Initializes the LCD resources.
  * @param  Instance:     LCD Instance.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_LCD_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_LCD_CHECK_PARAMS(Instance);

  /* USER CODE BEGIN BSP_LCD_DeInit */

  /* USER CODE END BSP_LCD_DeInit */

  return ret;
}

/**
  * @brief  Sets the LCD Orientation.
  * @param  Instance:     LCD Instance.
  * @param  Orientation:  New LCD Orientation.
  *         This parameter can be one of the following values:
  *          @arg @ref LCD_ORIENTATION_PORTRAIT           Portrait orientation choice of LCD screen.
  *          @arg @ref LCD_ORIENTATION_LANDSCAPE          Landscape orientation choice of LCD screen.
  *          @arg @ref LCD_ORIENTATION_PORTRAIT_ROT180    Portrait rotated 180° orientation choice of LCD screen.
  *          @arg @ref LCD_ORIENTATION_LANDSCAPE_ROT180   Landscape rotated 180° orientation choice of LCD screen.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_LCD_SetOrientation(uint32_t Instance, uint32_t Orientation)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_LCD_CHECK_PARAMS(Instance);

  if(LCD_OS_TryLock(Instance, LCD_OS_TIMEOUT_BUSY) != LCD_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    /* USER CODE BEGIN BSP_LCD_SetOrientation */

    /* USER CODE END BSP_LCD_SetOrientation */

    LCD_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Gets the LCD Orientation.
  * @param  Instance:     LCD Instance.
  * @param  pOrientation: Pointer to Current LCD Orientation.
  *         This parameter will be set to one of the following values:
  *          @arg @ref LCD_ORIENTATION_PORTRAIT           Portrait orientation choice of LCD screen.
  *          @arg @ref LCD_ORIENTATION_LANDSCAPE          Landscape orientation choice of LCD screen.
  *          @arg @ref LCD_ORIENTATION_PORTRAIT_ROT180    Portrait rotated 180° orientation choice of LCD screen.
  *          @arg @ref LCD_ORIENTATION_LANDSCAPE_ROT180   Landscape rotated 180° orientation choice of LCD screen.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_LCD_GetOrientation(uint32_t Instance, uint32_t *pOrientation)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_LCD_CHECK_PARAMS(Instance);

  if(LCD_OS_TryLock(Instance, LCD_OS_TIMEOUT_BUSY) != LCD_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    /* USER CODE BEGIN BSP_LCD_GetOrientation */
    *pOrientation = (hltdc.LayerCfg[Instance].ImageHeight > hltdc.LayerCfg[Instance].ImageWidth ? LCD_ORIENTATION_PORTRAIT : LCD_ORIENTATION_LANDSCAPE);
    ret = BSP_ERROR_NONE;
    /* USER CODE END BSP_LCD_GetOrientation */

    LCD_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Gets the LCD X size.
  * @param  Instance:     LCD Instance.
  * @param  pXSize:       Pointer to Used LCD X size.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_LCD_GetXSize(uint32_t Instance, uint32_t *pXSize)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_LCD_CHECK_PARAMS(Instance);

  if(LCD_OS_TryLock(Instance, LCD_OS_TIMEOUT_BUSY) != LCD_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    /* USER CODE BEGIN BSP_LCD_GetXSize */
    *pXSize = hltdc.LayerCfg[Instance].ImageWidth;
    ret = BSP_ERROR_NONE;
    /* USER CODE END BSP_LCD_GetXSize */

    LCD_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Gets the LCD Y size.
  * @param  Instance:     LCD Instance.
  * @param  pYSize:       Pointer to Used LCD Y size.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_LCD_GetYSize(uint32_t Instance, uint32_t *pYSize)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_LCD_CHECK_PARAMS(Instance);

  if(LCD_OS_TryLock(Instance, LCD_OS_TIMEOUT_BUSY) != LCD_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    /* USER CODE BEGIN BSP_LCD_GetYSize */
    *pYSize = hltdc.LayerCfg[Instance].ImageHeight;
    ret = BSP_ERROR_NONE;
    /* USER CODE END BSP_LCD_GetYSize */

    LCD_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Switch On the display.
  * @param  Instance:     LCD Instance.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_LCD_DisplayOn(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_LCD_CHECK_PARAMS(Instance);

  if(LCD_OS_TryLock(Instance, LCD_OS_TIMEOUT_BUSY) != LCD_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    /* USER CODE BEGIN BSP_LCD_DisplayOn */
    __HAL_LTDC_ENABLE(&hltdc);
    HAL_GPIO_WritePin(LCD_DISP_GPIO_PORT, LCD_DISP_GPIO_PIN, GPIO_PIN_SET);        /* Assert LCD_DISP pin */
    HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_GPIO_PIN, GPIO_PIN_SET);  /* Assert LCD_BL_CTRL pin */
    HAL_LTDC_ProgramLineEvent(&hltdc, lcd_int_active_line);
    ret = BSP_ERROR_NONE;
    /* USER CODE END BSP_LCD_DisplayOn */

    LCD_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Switch Off the display.
  * @param  Instance:     LCD Instance.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_LCD_DisplayOff(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_LCD_CHECK_PARAMS(Instance);

  if(LCD_OS_TryLock(Instance, LCD_OS_TIMEOUT_BUSY) != LCD_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    /* USER CODE BEGIN BSP_LCD_DisplayOff */
    __HAL_LTDC_DISABLE(&hltdc);
    HAL_GPIO_WritePin(LCD_DISP_GPIO_PORT, LCD_DISP_GPIO_PIN, GPIO_PIN_RESET);      /* De-assert LCD_DISP pin */
    HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_GPIO_PIN, GPIO_PIN_RESET);/* De-assert LCD_BL_CTRL pin */ 
    ret = BSP_ERROR_NONE;
    /* USER CODE END BSP_LCD_DisplayOff */

    LCD_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Send data to the selected the LCD GRAM.
  * @param  Instance:     LCD Instance.
  * @param  pData:        Pointer to data to write to LCD GRAM.
  * @param  Length:       Length of data to write to LCD GRAM.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_LCD_WriteData(uint32_t Instance, uint8_t *pData, uint32_t Length)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_LCD_CHECK_PARAMS(Instance);

  if(LCD_OS_TryLock(Instance, LCD_OS_TIMEOUT_BUSY) != LCD_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    /* USER CODE BEGIN BSP_LCD_WriteData */
    uint32_t i;
    uint16_t __IO *data = (uint16_t *)pData;
    uint16_t __IO *fb = (uint16_t *)LTDC_LAYER(&hltdc, Instance)->CFBAR + lcd_offset;

    for (i = 0; i < (Length / 2); i++)
    {
      fb[i] = data[i];
    }
    ret = BSP_ERROR_NONE;
    /* USER CODE END BSP_LCD_WriteData */

    LCD_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Send data to select the LCD GRAM.
  * @param  Instance:     LCD Instance.
  * @param  pData:        Pointer to data to write to LCD GRAM.
  * @param  Length:       Length of data to write to LCD GRAM.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_LCD_WriteDataDMA(uint32_t Instance, uint8_t *pData, uint32_t Length)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_LCD_CHECK_PARAMS(Instance);

  if(LCD_OS_TryLock(Instance, LCD_OS_TIMEOUT_BUSY) != LCD_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    /* USER CODE BEGIN BSP_LCD_WriteDataDMA */
#if (BSP_LCD_USE_MDMA)
    uint16_t __IO *fb = (uint16_t *)(LTDC_LAYER(&hltdc, Instance)->CFBAR + lcd_offset);
    uint8_t node_cnt = (Length / (64 * 1024));

    if(Length % (64 * 1024))
    {
      node_cnt++;
    }

    if(MDMA_List_Start_IT(&hLCDDMA, node_cnt, pData, (uint8_t *)fb, Length) != HAL_OK)
    {
      /* Transfer Error */
      ret = BSP_ERROR_BUS_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
#endif
    /* USER CODE END BSP_LCD_WriteDataDMA */
  }

  return ret;
}

/**
  * @brief  Sets a display window.
  * @param  Instance:     LCD Instance.
  * @param  Xpos:         Specifies the X position.
  * @param  Ypos:         Specifies the Y position.
  * @param  Height:       Specifies the height of the rectangle to fill.
  * @param  Width:        Specifies the width of the rectangle to fill.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_LCD_SetDisplayWindow(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_LCD_CHECK_PARAMS(Instance);

  if(LCD_OS_TryLock(Instance, LCD_OS_TIMEOUT_BUSY) != LCD_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    /* USER CODE BEGIN BSP_LCD_SetDisplayWindow */
	lcd_offset = ((Ypos * 2 * hltdc.LayerCfg[Instance].ImageWidth) + Xpos);
	ret = BSP_ERROR_NONE;
    /* USER CODE END BSP_LCD_SetDisplayWindow */

    LCD_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Send RGB data to select the LCD GRAM.
  * @param  Instance:     LCD Instance.
  * @param  UseDMA:       Specifies if DMA will be used for data Transferring.
  * @param  pData:        Pointer to data to write to LCD GRAM.
  * @param  Xpos:         Specifies the X position.
  * @param  Ypos:         Specifies the Y position.
  * @param  Height:       Specifies the height of the rectangle to fill.
  * @param  Width:        Specifies the width of the rectangle to fill.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_LCD_FillRGBRect(uint32_t Instance, uint8_t UseDMA, uint8_t *pData, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  /* USER CODE BEGIN BSP_LCD_FillRGBRect */

  /* USER CODE END BSP_LCD_FillRGBRect */

  return ret;
}

/**
  * @brief  Get the status of the LCD Transfer.
  * @param  Instance:     LCD Instance.
  * @retval uint8_t:      Zero if no Transfer, Transfer Operation code otherwise.
  */
uint8_t BSP_LCD_GetTransferStatus(uint32_t Instance)
{
  BSP_LCD_CHECK_PARAMS(Instance);

  return LCD_OS_IsLocked(Instance);
}

/**
  * @brief  Wait for until complete LCD Transfer.
  * @param  Instance:     LCD Instance.
  */
void BSP_LCD_WaitForTransferToBeDone(uint32_t Instance)
{
  if (Instance < LCD_INSTANCES_NBR)
  {
    LCD_OS_WaitForTransferToBeDone(Instance);
  }
}

/**
  * @brief  Signal Transfer Event.
  * @param  Instance:     LCD Instance.
  */
__WEAK void BSP_LCD_SignalTransferDone(uint32_t Instance)
{
  if (Instance < LCD_INSTANCES_NBR)
  {
    /* This is the user's Callback to be implemented at the application level */
  }
}

/**
  * @brief  Signal Transfer Event.
  * @param  Instance:     LCD Instance.
  * @param  State:        Event value.
  * @param  Line:         Line counter.
  */
__WEAK void BSP_LCD_SignalTearingEffectEvent(uint32_t Instance, uint8_t State, uint16_t Line)
{
  /* Prevent unused argument(s) compilation warning */;
  UNUSED(Line);

  if (Instance < LCD_INSTANCES_NBR)
  {
    /* This is the user's Callback to be implemented at the application level */
    if(State)
    {
      /* TE event is done : de-allow display refresh */
    }
    else
    {
      /* TE event is received : allow display refresh */
    }
  }
}
/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/* USER CODE BEGIN PF */
/**
  * @brief  Clean Invalidate DCache
  */
__STATIC_INLINE void LCD_CleanInvalidateDCache(void)
{
  /* USER CODE BEGIN LCD_CleanInvalidateDCache */
#if defined(CORE_CA7)
  L1C_CleanInvalidateDCacheAll();
#endif
  /* USER CODE END LCD_CleanInvalidateDCache */
}

#if (BSP_LCD_USE_MDMA)
/**
  * @brief  DMA transfer complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
static void DMA_TxCpltCallback(MDMA_HandleTypeDef *hdma)
{
  if(hdma == &hLCDDMA)
  {
    /* Reset the MDMA first/last linkedlist node addresses and node counter */
    hdma->FirstLinkedListNodeAddress  = 0;
    hdma->LastLinkedListNodeAddress   = 0;
    hdma->LinkedListNodeCounter  = 0;

    /* Unlock LCD ressources */
    LCD_OS_UnlockFromISR(0);

    /* Signal Transfer Done Event */
    BSP_LCD_SignalTransferDone(0);
  }
}

/**
  * @brief  DMA transfer error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during DMA transfer
  * @retval None
  */
static void DMA_TxErrorCallback(MDMA_HandleTypeDef *hdma)
{
/*
  __disable_irq();
  while (1)
  {
  }
*/
  Error_Handler();
}

/**
  * @brief  DMA Linked-list Queue configuration
  * @param  None
  * @retval None
  */
__STATIC_INLINE HAL_StatusTypeDef MDMA_List_Start_IT(MDMA_HandleTypeDef *hmdma, uint8_t node_cnt, uint8_t *src, uint8_t *dst, uint32_t data_size)
{
  HAL_StatusTypeDef ret = HAL_OK;
  /* DMA node configuration declaration */
  MDMA_LinkNodeConfTypeDef pNodeConfig;
  uint8_t i;
  uint32_t SrcAddress;
  uint32_t FirstBlockDataLength;
  uint32_t div;
  ALIGN_32BYTES(static MDMA_LinkNodeTypeDef LCD_MDMA_Nodes[(((480 * 272 * 2) / (64 * 1024)) + 1)]);

  switch(hLCDDMA.Init.SourceInc)
  {
    case MDMA_SRC_INC_WORD :
      div = 4;
    break;
    case MDMA_SRC_INC_HALFWORD :
      div = 2;
    break;
    default:
    case MDMA_SRC_INC_BYTE :
      div = 1;
    break;
  }

  FirstBlockDataLength = (data_size / div);

  if(node_cnt > 1)
  {
    /* Set node configuration ################################################*/
    pNodeConfig.Init.Request              = hmdma->Init.Request;
    pNodeConfig.Init.TransferTriggerMode  = hmdma->Init.TransferTriggerMode;
    pNodeConfig.Init.Priority             = hmdma->Init.Priority;
    pNodeConfig.Init.Endianness           = hmdma->Init.Endianness;
    pNodeConfig.Init.SourceInc            = hmdma->Init.SourceInc;
    pNodeConfig.Init.DestinationInc       = hmdma->Init.DestinationInc;
    pNodeConfig.Init.SourceDataSize       = hmdma->Init.SourceDataSize;
    pNodeConfig.Init.DestDataSize         = hmdma->Init.DestDataSize;
    pNodeConfig.Init.DataAlignment        = hmdma->Init.DataAlignment;
    pNodeConfig.Init.BufferTransferLength = hmdma->Init.BufferTransferLength;
    pNodeConfig.Init.SourceBurst          = hmdma->Init.SourceBurst;
    pNodeConfig.Init.DestBurst            = hmdma->Init.DestBurst;
    pNodeConfig.Init.SourceBlockAddressOffset  = hmdma->Init.SourceBlockAddressOffset;
    pNodeConfig.Init.DestBlockAddressOffset    = hmdma->Init.DestBlockAddressOffset;
    pNodeConfig.PostRequestMaskAddress = 0;
    pNodeConfig.PostRequestMaskData = 0;
    pNodeConfig.DstAddress = (uint32_t) dst;
    pNodeConfig.BlockDataLength = ((64 * 1024) / div);
    pNodeConfig.BlockCount = 1;

    SrcAddress = (uint32_t) src;
    for(i = 0; ((ret == HAL_OK) && (i < node_cnt)); i++)
    {
      pNodeConfig.SrcAddress = SrcAddress;
      if(i == 0)
      {
        FirstBlockDataLength = pNodeConfig.BlockDataLength;
        /* Build Node first Node */
        ret |= HAL_MDMA_LinkedList_CreateNode(&LCD_MDMA_Nodes[i], &pNodeConfig);
        /* Add Node to List */
        ret |= HAL_MDMA_LinkedList_AddNode(hmdma, &LCD_MDMA_Nodes[i], 0);
      }
      else
      {
        /* Adjust size of last node */
        if((node_cnt > 1) && (i == (node_cnt - 1)))
        {
          pNodeConfig.BlockDataLength = ((data_size - (i * (64 * 1024))) / div);
        }
        /* Build Node Node */
        ret |= HAL_MDMA_LinkedList_CreateNode(&LCD_MDMA_Nodes[i], &pNodeConfig);
        /* Add Node to List */
        ret |= HAL_MDMA_LinkedList_AddNode(hmdma, &LCD_MDMA_Nodes[i], &LCD_MDMA_Nodes[i-1]);

        //SCB_CleanDCache_by_Addr((uint32_t*)&LCD_MDMA_Nodes[i], sizeof(MDMA_LinkNodeTypeDef));
      }
      SrcAddress += ((64 * 1024) / div);
    }
    /*
      As the MDMA Nodes descriptors are located in the SRAM which
      is cacheable, it is necessary to clean the data cache after creating the nodes
      in order to make sure that the MDMA will load up-to-date data from the linked-list nodes
    */
    LCD_CleanInvalidateDCache();
  }

  if(ret == HAL_OK)
  {
    ret = HAL_MDMA_Start_IT(hmdma, (uint32_t)src, (uint32_t)dst, FirstBlockDataLength, 1);
  }

  return ret;
}
#endif

void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef* hltdc)
{
  if (hltdc->Instance->LIPCR == lcd_int_active_line)
  {
    /* Entering Active Area */
    HAL_LTDC_ProgramLineEvent(hltdc, lcd_int_porch_line);
    /* Call BSP_LCD_SignalTearingEffectEvent() */
    BSP_LCD_SignalTearingEffectEvent(0, 1, 0);
  }
  else
  {
    /* Exiting Active Area */
    HAL_LTDC_ProgramLineEvent(hltdc, lcd_int_active_line);
    /* Call BSP_LCD_SignalTearingEffectEvent() */
    BSP_LCD_SignalTearingEffectEvent(0, 0, 0);
  }
}
/* USER CODE END PF */

/**
  * @brief  Provide a tick value in millisecond.
  * @retval int32_t:  Tick value.
  */
int32_t LCD_IO_GetTick(void)
{
  uint32_t ret;
  ret = HAL_GetTick();
  return (int32_t)ret;
}

/**
  * @brief  LCD IO delay
  * @param  Delay:    Delay in ms
  * @retval int32_t:  Error status
  */
int32_t LCD_IO_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
  return BSP_ERROR_NONE;
}
/**
  * @}
  */
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
