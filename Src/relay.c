#include "relay.h"

struct _relay relay_A; //创建继电器1

void RELAY_InitRelay(struct _relay *relay, uint8_t id)
{
	relay->id = id;
	RELAY_CloseRelay(relay);
	user_relay_info("继电器初始化完毕");
}

void RELAY_OpenRelay(struct _relay *relay)
{
	if(relay->id == 1)
	{
		user_relay_info("继电器1已开启");
		HAL_GPIO_WritePin(OUT1_GPIO_Port, OUT1_Pin, GPIO_PIN_RESET);
	}
	else if(relay->id == 2)
	{
		user_relay_info("继电器2已开启");
		HAL_GPIO_WritePin(OUT2_GPIO_Port, OUT2_Pin, GPIO_PIN_RESET);	
	}
	relay->status=1;
}

void RELAY_CloseRelay(struct _relay *relay)
{
	if(relay->id == 1)
	{
		user_relay_info("继电器1已关闭");
		HAL_GPIO_WritePin(OUT1_GPIO_Port, OUT1_Pin, GPIO_PIN_SET);
	}
	else if(relay->id == 2)
	{
		user_relay_info("继电器2已关闭");
		HAL_GPIO_WritePin(OUT2_GPIO_Port, OUT2_Pin, GPIO_PIN_SET);	
	}
		relay->status=0;
}
