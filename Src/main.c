/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t usart1buff[10];
uint8_t usart2buff[10];
uint8_t count_updata;
uint8_t count_leak;
uint8_t count_time;
uint8_t count_hp5806;
uint8_t count_pid;
uint8_t current_proccess_event;
uint8_t flag_proccess_event;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Monitor_modifications(void)
{
	//继电器模块
	RELAY_run(MCUsystem.relay_A);
	RELAY_run(MCUsystem.relay_B);
	if(count_time>=5)
	{//时间模块
		count_time=0;
		TIM_run(MCUsystem.time);
	}
	if(count_leak >= 5)
	{//泄漏监测
		count_leak = 0;
		LEAK_cheack();
	}
	if(count_updata>3)
	{//屏幕更新模块
		TOUCH_UpdataUI();
		count_updata=0;
	}
	if(count_hp5806>1)
	{//气压传感器模块
		count_hp5806=0;
		HP5806_run(MCUsystem.hp5806_A);
    HP5806_run(MCUsystem.hp5806_B);
		TOUCH_curve_write();
	}
	if(count_pid>1)
	{//pid模块
		count_pid=0;
		if(MCUsystem.system_status->current == 2)
		{
			PID_posRealize(MCUsystem.pid, (*MCUsystem.hp5806_A).Pcomp/100-MCUsystem.set_value, (*MCUsystem.hp5806_B).Pcomp/100);
			MCUsystem.output_value = (*MCUsystem.pid).voltage;
		}
	}
	if(MCUsystem.system_status->current == 4)
	{//手动输出介入
		MCUsystem.output_value = MCUsystem.output_manual;
	}
	system_status_run();
}

int check_safe_event(PriorityQueue *pq, SafeEvent *currentSafeEvent)
{
	if(currentSafeEvent->data != current_proccess_event)
		{//来了一个新事件，且优先级高
			current_proccess_event = currentSafeEvent->data;
			flag_proccess_event = 1;
			return 1;
		}
		else if(flag_proccess_event)
		{//可能来了一个新事件，也可能是同个事件，内容一致，属于重复触发，且未处理完毕
			return 0;//等待处理
		}
		else
		{//理论上属于，来了一个新事件，但内容和上次一样，属于重复触发，上次事件处理完毕
			HEAP_pop(pq,currentSafeEvent);
			return 0;//就不应该执行，因为在事件结束后需要清空当前事件，所以处理完毕后c和f都为0;只有事件内容为0才可能触发
		}
}


/*
0x01: 温度上限 P：1
0x02: 温度下限 P：1
0x03: 气压上限 P：4
0x04: 气压下限 P：5
0x05: 管道泄漏 P：2
0x06: 设备初始化未通过 P：3
*/
uint8_t event_flag = 0;
void process_safe_event(PriorityQueue *pq)
{
	SafeEvent currentSafeEvent;
	if(HEAP_is_empty(pq))
	{
		current_proccess_event = 0x00;//无待处理的事件
		flag_proccess_event = 0;
		return;
	}
	HEAP_find_max(pq, &currentSafeEvent);
	//此处解析事件
	if(currentSafeEvent.data == 0x01)
	{
		if(check_safe_event(pq, &currentSafeEvent))
		{
			if(MCUsystem.system_status->current!=6)RGB_Change(MCUsystem.rgb,6);
			TOUCH_change_page(0x002D);
			TOUCH_safeEvent_Qt(currentSafeEvent.data);
		}
	}
	else if(currentSafeEvent.data == 0x02)
	{
		if(check_safe_event(pq, &currentSafeEvent))
		{
			if(MCUsystem.system_status->current!=6)RGB_Change(MCUsystem.rgb,6);
			TOUCH_change_page(0x002C);
			TOUCH_safeEvent_Qt(currentSafeEvent.data);
		}
	}
	else if(currentSafeEvent.data == 0x03)
	{
		if(check_safe_event(pq, &currentSafeEvent))
		{
			TOUCH_change_page(0x002F);
			TOUCH_safeEvent_Qt(currentSafeEvent.data);
			user_main_info("终止前压力测量结果为：%.2fhPa", MCUsystem.hp5806_B->Pcomp / 100);
			if(MCUsystem.system_status->current!=6){
				if(MCUsystem.system_status->current == 2 ||  MCUsystem.system_status->current == 3)
				{
					system_StatusSwitch(MCUsystem.system_status, 3);
					system_auto_pause();
					MCUsystem.system_status->flag_change = 0;
				}
				else if(MCUsystem.system_status->current == 4 ||  MCUsystem.system_status->current == 5)
				{
					system_StatusSwitch(MCUsystem.system_status, 5);
					system_manual_pause();
					MCUsystem.system_status->flag_change = 0;
				}
				else
				{
					system_StatusSwitch(MCUsystem.system_status, 1);
					system_into_prep();
					MCUsystem.system_status->flag_change = 0;
				}
			}
		if(MCUsystem.system_status->current!=6)RGB_Change(MCUsystem.rgb,4);
		}
	}
	else if(currentSafeEvent.data == 0x04)
	{
		if(check_safe_event(pq, &currentSafeEvent))
		{
			TOUCH_change_page(0x0030);
			TOUCH_safeEvent_Qt(currentSafeEvent.data);
			user_main_info("终止前压力测量结果为：%.2fhPa", MCUsystem.hp5806_B->Pcomp / 100);
			if(MCUsystem.system_status->current!=6){
				if(MCUsystem.system_status->current == 2 ||  MCUsystem.system_status->current == 3)
				{
					system_StatusSwitch(MCUsystem.system_status, 3);
					system_auto_pause();
					MCUsystem.system_status->flag_change = 0;
				}
				else if(MCUsystem.system_status->current == 4 ||  MCUsystem.system_status->current == 5)
				{
					system_StatusSwitch(MCUsystem.system_status, 5);
					system_manual_pause();
					MCUsystem.system_status->flag_change = 0;
				}
				else
				{
					system_StatusSwitch(MCUsystem.system_status, 1);
					system_into_prep();
					MCUsystem.system_status->flag_change = 0;
				}
			}
		if(MCUsystem.system_status->current!=6)RGB_Change(MCUsystem.rgb,4);
		}
	}
	else if(currentSafeEvent.data == 0x05)
	{
		if(check_safe_event(pq, &currentSafeEvent))
		{
			if(MCUsystem.system_status->current!=6)RGB_Change(MCUsystem.rgb,6);
			TOUCH_change_page(0x002B);
			TOUCH_safeEvent_Qt(currentSafeEvent.data);
		}
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(1000); //需要给传感器上电进入状态一些时间，不然板载传感器会测不准
	safe_event_pq = HEAP_init(10); //容量为10的安全事件优先队列
	system_init();
  user_main_info("GPIO,IIC1/2,USART1/2,TIM2/3/4初始化完成");
  user_main_info("定时器3中断开启，周期%.2fHz", 72000000.0 / (htim3.Instance->ARR + 1) / (htim3.Instance->PSC + 1));
  HAL_TIM_Base_Start_IT(&htim3);
  user_main_info("定时器4中断开启，周期%.2fHz", 72000000.0 / (htim4.Instance->ARR + 1) / (htim4.Instance->PSC + 1));
  HAL_TIM_Base_Start_IT(&htim4);
//	UART_Start_Receive_IT(&huart1, uint8_t *pData, uint16_t Size);
	HAL_UART_Receive_IT(&huart1, usart1buff, 1);
	HAL_UART_Receive_IT(&huart2, usart2buff, 1);
	TOUCH_Reste();
  HAL_Delay(2000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


		TOUCH_extract_command();
		Monitor_modifications();
		process_safe_event(safe_event_pq);
		//状态更新(本机发给上位机，轮询，上位机修改直接发送)，当前速度（直接发没啥问题），手自动速度（需要同步，同时发）
		PUMP_changeSpeed(MCUsystem.pump, MCUsystem.output_value);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
