/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "key.h"
#include "work_system.h"
/* USER CODE END Includes */

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

/* USER CODE BEGIN Private defines */
struct _time{
	uint8_t flag_time;
	uint16_t hour; 
	uint8_t min;	
	uint8_t sec; 
	int16_t setting_sec; 
	int16_t remainder_sec; 
};
extern struct _time time;
void TIM_TimeInit(struct _time *time);
void TIM_TimeExit(struct _time *time);
void TIM_run(struct _time *time);
/* USER CODE END Private defines */

void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_TIM4_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE BEGIN Prototypes */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
