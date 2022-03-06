#include "relay.h"

void InintRelay(void)
{
	CloseRelay(1);
	CloseRelay(2);
	user_relay_info("继电器初始化完毕");
}

void OpenRelay(uint8_t ID)
{
	if(ID == 1)
	{
		user_relay_info("继电器1已开启");
		HAL_GPIO_WritePin(OUT1_GPIO_Port, OUT1_Pin, GPIO_PIN_RESET);
	}
	else if(ID == 2)
	{
		user_relay_info("继电器2已开启");
		HAL_GPIO_WritePin(OUT2_GPIO_Port, OUT2_Pin, GPIO_PIN_RESET);	
	}
}

void CloseRelay(uint8_t ID)
{
	if(ID == 1)
	{
		user_relay_info("继电器1已关闭");
		HAL_GPIO_WritePin(OUT1_GPIO_Port, OUT1_Pin, GPIO_PIN_SET);
	}
	else if(ID == 2)
	{
		user_relay_info("继电器2已关闭");
		HAL_GPIO_WritePin(OUT2_GPIO_Port, OUT2_Pin, GPIO_PIN_SET);	
	}
}
