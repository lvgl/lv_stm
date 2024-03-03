/**
  ******************************************************************************
  * @file    gt911_reg.c
  * @author  MCD Application Team
  * @brief   This file provides unitary register function to control the gt911 Touch
  *
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

/* Includes ------------------------------------------------------------------*/
#include "gt911_reg.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Component
  * @{
  */

/** @defgroup GT911 GT911
  * @{
  */
/**
  * @brief  Read GT911 registers.
  * @param  ctx Component context
  * @param  reg Component reg to read from
  * @param  pdata pointer to data to be read
  * @param  length Length of data to read
  * @retval Component status
  */
int32_t gt911_read_reg(gt911_ctx_t *ctx, uint16_t reg, uint8_t* pdata, uint16_t length)
{
  return ctx->ReadReg(ctx->handle, reg, pdata, length);
}

/**
  * @brief  Write GT911 registers.
  * @param  ctx Component context
  * @param  reg Component reg to write to
  * @param  pdata pointer to data to be written
  * @param  length Length of data to write
  * @retval Component status
  */
int32_t gt911_write_reg(gt911_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length)
{
  return ctx->WriteReg(ctx->handle, reg, pdata, length);
}

/**
  * @brief  Set GT911 working mode
  * @param  ctx Component context
  * @param  value Value to write to GT911_DEV_MODE_REG register
  * @retval Component status
  */
int32_t  gt911_dev_mode_w(gt911_ctx_t *ctx, uint8_t value)
{
  int32_t ret;
  uint8_t tmp;

  ret = gt911_read_reg(ctx, GT911_DEV_MODE_REG, &tmp, 1);

  if(ret == 0)
  {
    tmp &= ~GT911_DEV_MODE_BIT_MASK;
    tmp |= value << GT911_DEV_MODE_BIT_POSITION;

    ret = gt911_write_reg(ctx, GT911_DEV_MODE_REG, &tmp, 1);
  }

  return ret;
}

/**
  * @brief  Get GT911 working mode
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_DEV_MODE_REG register
  * @retval Component status
  */
int32_t  gt911_dev_mode_r(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_DEV_MODE_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_DEV_MODE_BIT_MASK;
    *value = *value >> GT911_DEV_MODE_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 gesture ID
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_GEST_ID_REG register
  * @retval Component status
  */
int32_t  gt911_gest_id(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_GEST_ID_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Get GT911 Touch Data Status
  * @param  ctx Component context
  * @param  value pointer to the value of Touch Data Status register
  * @retval Component status
  */
int32_t  gt911_td_status(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;
  do
  {
    ret = gt911_read_reg(ctx, GT911_TD_STAT_REG, (uint8_t *)value, 1);
    if( ret==0 )
    {
      // If the BUFFER_STATUS bit[7] is only set, write 0 on the register
      if( *value==GT911_TD_STATUS_BIT_BUFFER_STAT )
      {
        *value = 0;
        ret = gt911_write_reg(ctx, GT911_TD_STAT_REG, (uint8_t *)value, 1);
      }
      else
      {
        *value &= GT911_TD_STATUS_BITS_NBTOUCHPTS;
        break;
      }
    }
  }while( ret==0 );
  return ret;
}

/**
  * @brief  Get GT911 Clear Interrupt/Buffer Status
  * @param  ctx Component context
  * @retval Component status
  */
int32_t  gt911_clr_int(gt911_ctx_t *ctx)
{
  int32_t ret;
  uint8_t tmp;

  tmp = 0;
  ret = gt911_write_reg(ctx, GT911_TD_STAT_REG, &tmp, 1);

  return ret;
}

/**
  * @brief  Get GT911 First X Event Flag
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P1_XH_REG register
  * @retval Component status
  */
int32_t  gt911_p1_xh_ef(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P1_XH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P1_XH_EF_BIT_MASK;
    *value = *value >> GT911_P1_XH_EF_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 First X Low Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P1_XL_REG register
  * @retval Component status
  */
int32_t  gt911_p1_xl_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_P1_XL_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Get GT911 First X High Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P1_XH_REG register
  * @retval Component status
  */
int32_t  gt911_p1_xh_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P1_XH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P1_XH_TP_BIT_MASK;
    *value = *value >> GT911_P1_XH_TP_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 First Y Low Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P1_YL_REG register
  * @retval Component status
  */
int32_t  gt911_p1_yl_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_P1_YL_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Get GT911 First Y High Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P1_YH_REG register
  * @retval Component status
  */
int32_t  gt911_p1_yh_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P1_YH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P1_YH_TP_BIT_MASK;
    *value = *value >> GT911_P1_YH_TP_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 First Touch pressure
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P1_WEIGHTL_REG register
  * @retval Component status
  */
int32_t  gt911_p1_weight(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_P1_WEIGHTL_REG, (uint8_t *)value, 2);
}

/**
  * @brief  Get GT911 First track ID
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P1_TID_REG register
  * @retval Component status
  */
int32_t  gt911_p1_tid(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P1_TID_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P1_TID_BIT_MASK;
    *value = *value >> GT911_P1_TID_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Second X Event Flag
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P2_XH_REG register
  * @retval Component status
  */
int32_t  gt911_p2_xh_ef(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P2_XH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P2_XH_EF_BIT_MASK;
    *value = *value >> GT911_P2_XH_EF_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Second X High Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P2_XH_REG register
  * @retval Component status
  */
int32_t  gt911_p2_xh_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P2_XH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P2_XH_TP_BIT_MASK;
    *value = *value >> GT911_P2_XH_TP_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Second X Low Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P2_XL_REG register
  * @retval Component status
  */
int32_t  gt911_p2_xl_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_P2_XL_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Get GT911 Second Touch ID
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P2_XH_REG register
  * @retval Component status
  */
int32_t  gt911_p2_yh_tid(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P2_YH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P2_YH_TID_BIT_MASK;
    *value = *value >> GT911_P2_YH_TID_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Second Y High Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P2_YH_REG register
  * @retval Component status
  */
int32_t  gt911_p2_yh_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P2_YH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P2_YH_TP_BIT_MASK;
    *value = *value >> GT911_P2_YH_TP_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Second Y Low Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P2_YL_REG register
  * @retval Component status
  */
int32_t  gt911_p2_yl_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_P2_YL_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Get GT911 Second Touch pressure
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P2_WEIGHT_REG register
  * @retval Component status
  */
int32_t  gt911_p2_weight(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_P2_WEIGHT_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Get GT911 Second Touch area
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P2_MISC_REG register
  * @retval Component status
  */
int32_t  gt911_p2_misc(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P2_MISC_REG, (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= GT911_P2_MISC_BIT_MASK;
    *value = *value >> GT911_P2_MISC_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Third X Event Flag
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P3_XH_REG register
  * @retval Component status
  */
int32_t  gt911_p3_xh_ef(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P3_XH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P3_XH_EF_BIT_MASK;
    *value = *value >> GT911_P3_XH_EF_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Third X High Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P3_XH_REG register
  * @retval Component status
  */
int32_t  gt911_p3_xh_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P3_XH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P3_XH_TP_BIT_MASK;
    *value = *value >> GT911_P3_XH_TP_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Third X Low Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P3_XL_REG register
  * @retval Component status
  */
int32_t  gt911_p3_xl_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_P3_XL_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Get GT911 Third Touch ID
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P3_XH_REG register
  * @retval Component status
  */
int32_t  gt911_p3_yh_tid(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P3_YH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P3_YH_TID_BIT_MASK;
    *value = *value >> GT911_P3_YH_TID_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Third Y High Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P3_YH_REG register
  * @retval Component status
  */
int32_t  gt911_p3_yh_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P3_YH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P3_YH_TP_BIT_MASK;
    *value = *value >> GT911_P3_YH_TP_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Third Y Low Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P3_YL_REG register
  * @retval Component status
  */
int32_t  gt911_p3_yl_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_P3_YL_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Get GT911 Third Touch pressure
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P3_WEIGHT_REG register
  * @retval Component status
  */
int32_t  gt911_p3_weight(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_P3_WEIGHT_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Get GT911 Third Touch area
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P3_MISC_REG register
  * @retval Component status
  */
int32_t  gt911_p3_misc(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P3_MISC_REG, (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= GT911_P3_MISC_BIT_MASK;
    *value = *value >> GT911_P3_MISC_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Fourth X Event Flag
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P4_XH_REG register
  * @retval Component status
  */
int32_t  gt911_p4_xh_ef(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P4_XH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P4_XH_EF_BIT_MASK;
    *value = *value >> GT911_P4_XH_EF_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Fourth X High Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P4_XH_REG register
  * @retval Component status
  */
int32_t  gt911_p4_xh_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P4_XH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P4_XH_TP_BIT_MASK;
    *value = *value >> GT911_P4_XH_TP_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Fourth X Low Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P4_XL_REG register
  * @retval Component status
  */
int32_t  gt911_p4_xl_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_P4_XL_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Get GT911 Fourth Touch ID
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P4_XH_REG register
  * @retval Component status
  */
int32_t  gt911_p4_yh_tid(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P4_YH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P4_YH_TID_BIT_MASK;
    *value = *value >> GT911_P4_YH_TID_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Fourth Y High Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P4_YH_REG register
  * @retval Component status
  */
int32_t  gt911_p4_yh_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P4_YH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P4_YH_TP_BIT_MASK;
    *value = *value >> GT911_P4_YH_TP_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Fourth Y Low Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P4_YL_REG register
  * @retval Component status
  */
int32_t  gt911_p4_yl_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_P4_YL_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Get GT911 Fourth Touch pressure
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P4_WEIGHT_REG register
  * @retval Component status
  */
int32_t  gt911_p4_weight(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_P4_WEIGHT_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Get GT911 Fourth Touch area
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P4_MISC_REG register
  * @retval Component status
  */
int32_t  gt911_p4_misc(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P4_MISC_REG, (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= GT911_P4_MISC_BIT_MASK;
    *value = *value >> GT911_P4_MISC_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Fifth X Event Flag
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P5_XH_REG register
  * @retval Component status
  */
int32_t  gt911_p5_xh_ef(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P5_XH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P5_XH_EF_BIT_MASK;
    *value = *value >> GT911_P5_XH_EF_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Fifth X High Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P5_XH_REG register
  * @retval Component status
  */
int32_t  gt911_p5_xh_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P5_XH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P5_XH_TP_BIT_MASK;
    *value = *value >> GT911_P5_XH_TP_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Fifth X Low Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P5_XL_REG register
  * @retval Component status
  */
int32_t  gt911_p5_xl_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_P5_XL_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Get GT911 Fifth Touch ID
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P5_XH_REG register
  * @retval Component status
  */
int32_t  gt911_p5_yh_tid(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P5_YH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P5_YH_TID_BIT_MASK;
    *value = *value >> GT911_P5_YH_TID_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Fifth Y High Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P5_YH_REG register
  * @retval Component status
  */
int32_t  gt911_p5_yh_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P5_YH_REG, (uint8_t *)value, 1);

  if(ret == 0)
  {
    *value &= GT911_P5_YH_TP_BIT_MASK;
    *value = *value >> GT911_P5_YH_TP_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Get GT911 Fifth Y Low Touch Position
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P5_YL_REG register
  * @retval Component status
  */
int32_t  gt911_p5_yl_tp(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_P5_YL_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Get GT911 Fifth Touch pressure
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P5_WEIGHT_REG register
  * @retval Component status
  */
int32_t  gt911_p5_weight(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_P5_WEIGHT_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Get GT911 Fifth Touch area
  * @param  ctx Component context
  * @param  value pointer to the value of GT911_P5_MISC_REG register
  * @retval Component status
  */
int32_t  gt911_p5_misc(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;

  ret = gt911_read_reg(ctx, GT911_P5_MISC_REG, (uint8_t *)value, 1);
  if(ret == 0)
  {
    *value &= GT911_P5_MISC_BIT_MASK;
    *value = *value >> GT911_P5_MISC_BIT_POSITION;
  }

  return ret;
}

/**
  * @brief  Set GT911 Threshold for touch detection
  * @param  ctx Component context
  * @param  value Value to write to GT911_TH_GROUP_REG register
  * @retval Component status
  */
int32_t  gt911_th_group(gt911_ctx_t *ctx, uint8_t value)
{
  return gt911_write_reg(ctx, GT911_TH_GROUP_REG, &value, 1);
}

/**
  * @brief  Set GT911 Filter function coefficient
  * @param  ctx Component context
  * @param  value Value to write to GT911_TH_DIFF_REG register
  * @retval Component status
  */
int32_t  gt911_th_diff(gt911_ctx_t *ctx, uint8_t value)
{
  return gt911_write_reg(ctx, GT911_TH_DIFF_REG, &value, 1);
}

/**
  * @brief  Control the Switch between Active and Monitoring Mode
  * @param  ctx Component context
  * @param  value Value to write to GT911_CTRL_REG register
  * @retval Component status
  */
int32_t  gt911_ctrl(gt911_ctx_t *ctx, uint8_t value)
{
  return gt911_write_reg(ctx, GT911_CTRL_REG, &value, 1);
}

/**
  * @brief  Set the time period of switching from Active mode to Monitor
  * @param  ctx Component context
  * @param  value Value to write to GT911_TIMEENTERMONITOR_REG register
  * @retval Component status
  */
int32_t  gt911_time_enter_monitor(gt911_ctx_t *ctx, uint8_t value)
{
  return gt911_write_reg(ctx, GT911_TIMEENTERMONITOR_REG, &value, 1);
}

/**
  * @brief  Set rate in Active mode
  * @param  ctx Component context
  * @param  value Value to write to GT911_PERIODACTIVE_REG register
  * @retval Component status
  */
int32_t  gt911_period_active(gt911_ctx_t *ctx, uint8_t value)
{
  return gt911_write_reg(ctx, GT911_PERIODACTIVE_REG, &value, 1);
}

/**
  * @brief  Set rate in Monitor mode
  * @param  ctx Component context
  * @param  value Value to write to GT911_PERIODMONITOR_REG register
  * @retval Component status
  */
int32_t  gt911_period_monitor(gt911_ctx_t *ctx, uint8_t value)
{
  return gt911_write_reg(ctx, GT911_PERIODMONITOR_REG, &value, 1);
}

/**
  * @brief  Set the value of the minimum allowed angle while Rotating
  * @param  ctx Component context
  * @param  value Value to write to GT911_RADIAN_VALUE_REG register
  * @retval Component status
  */
int32_t  gt911_radian_value(gt911_ctx_t *ctx, uint8_t value)
{
  return gt911_write_reg(ctx, GT911_RADIAN_VALUE_REG, &value, 1);
}

/**
  * @brief  Set Maximum offset while Moving Left and Moving Right gesture
  * @param  ctx Component context
  * @param  value Value to write to GT911_OFFSET_LR_REG register
  * @retval Component status
  */
int32_t  gt911_offset_left_right(gt911_ctx_t *ctx, uint8_t value)
{
  return gt911_write_reg(ctx, GT911_OFFSET_LR_REG, &value, 1);
}

/**
  * @brief  Set Maximum offset while Moving Up and Moving Down gesture
  * @param  ctx Component context
  * @param  value Value to write to GT911_OFFSET_UD_REG register
  * @retval Component status
  */
int32_t  gt911_offset_up_down(gt911_ctx_t *ctx, uint8_t value)
{
  return gt911_write_reg(ctx, GT911_OFFSET_UD_REG, &value, 1);
}

/**
  * @brief  Set Minimum distance while Moving Left and Moving Right gesture
  * @param  ctx Component context
  * @param  value Value to write to GT911_DISTANCE_LR_REG register
  * @retval Component status
  */
int32_t  gt911_disatnce_left_right(gt911_ctx_t *ctx, uint8_t value)
{
  return gt911_write_reg(ctx, GT911_DISTANCE_LR_REG, &value, 1);
}

/**
  * @brief  Set Minimum distance while Moving Up and Moving Down gesture
  * @param  ctx Component context
  * @param  value Value to write to GT911_DISTANCE_UD_REG register
  * @retval Component status
  */
int32_t  gt911_distance_up_down(gt911_ctx_t *ctx, uint8_t value)
{
  return gt911_write_reg(ctx, GT911_DISTANCE_UD_REG, &value, 1);
}

/**
  * @brief  Set Maximum distance while Zoom In and Zoom Out gesture
  * @param  ctx Component context
  * @param  value Value to write to GT911_DISTANCE_ZOOM_REG register
  * @retval Component status
  */
int32_t  gt911_distance_zoom(gt911_ctx_t *ctx, uint8_t value)
{
  return gt911_write_reg(ctx, GT911_DISTANCE_ZOOM_REG, &value, 1);
}

/**
  * @brief  Read High 8-bit of LIB Version info
  * @param  ctx Component context
  * @param  value Pointer to GT911_LIB_VER_H_REG register value
  * @retval Component status
  */
int32_t  gt911_lib_ver_high(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_LIB_VER_H_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Read Low 8-bit of LIB Version info
  * @param  ctx Component context
  * @param  value Pointer to GT911_LIB_VER_L_REG register value
  * @retval Component status
  */
int32_t  gt911_lib_ver_low(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_LIB_VER_L_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Read status of cipher register
  * @param  ctx Component context
  * @param  value Pointer to GT911_CIPHER_REG register value
  * @retval Component status
  */
int32_t  gt911_cipher(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_CIPHER_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Select Interrupt (polling or trigger) mode
  * @param  ctx Component context
  * @param  value Value to write to GT911_MSW1_REG register
  * @retval Component status
  */
int32_t  gt911_m_sw1(gt911_ctx_t *ctx, uint8_t value)
{
  return gt911_write_reg(ctx, GT911_MSW1_REG, &value, 1);
}

/**
  * @brief  Select Current power mode
  * @param  ctx Component context
  * @param  value Value to write to GT911_PWR_MODE_REG register
  * @retval Component status
  */
int32_t  gt911_pwr_mode(gt911_ctx_t *ctx, uint8_t value)
{
  return gt911_write_reg(ctx, GT911_PWR_MODE_REG, &value, 1);
}

/**
  * @brief  Read the Firmware Version
  * @param  ctx Component context
  * @param  value Pointer to GT911_FIRMID_REG register value
  * @retval Component status
  */
int32_t  gt911_firm_id(gt911_ctx_t *ctx, uint8_t *value)
{
  // GT911 Firmware version: low byte, high byte
  return gt911_read_reg(ctx, GT911_FIRMID_REG, (uint8_t *)value, 2);
}

/**
  * @brief  Read the Goodix's GT911 ID (4-byte)
  * @param  ctx Component context
  * @param  value Pointer to GT911_CHIP_ID_REG register value
  * @retval Component status
  */
int32_t  gt911_chip_id(gt911_ctx_t *ctx, uint8_t *value)
{
  int32_t ret;
  // 4-byte Product ID (ASCII, "911"0x00)
  ret = gt911_read_reg(ctx, GT911_CHIP_ID_REG, (uint8_t *)value, 4);

  if(ret == 0)
  {
    /* *value &= GT911_CHIP_ID_BIT_MASK */
    /* *value = *value >> GT911_CHIP_ID_BIT_POSITION */
  }
  return ret;
}

/**
  * @brief  Read the Release code version
  * @param  ctx Component context
  * @param  value Pointer to GT911_RELEASE_CODE_ID_REG register value
  * @retval Component status
  */
int32_t  gt911_release_code_id(gt911_ctx_t *ctx, uint8_t *value)
{
  return gt911_read_reg(ctx, GT911_RELEASE_CODE_ID_REG, (uint8_t *)value, 1);
}

/**
  * @brief  Select Current Operating mode
  * @param  ctx Component context
  * @param  value Value to write to GT911_COMMAND_REG register
  * @retval Component status
  */
int32_t  gt911_mode(gt911_ctx_t *ctx, uint8_t value)
{
  return gt911_write_reg(ctx, GT911_COMMAND_REG, &value, 1);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
