#include "pump.h"

struct _pump pump;


void PUMP_init(struct _pump *pump)
{
	PUMP_openPump(pump);
	PUMP_changeSpeed(pump, 0);
	pump->frequency = 72000000.0 / (htim2.Instance->ARR + 1) / (htim2.Instance->PSC + 1);
}

void PUMP_changeSpeed(struct _pump *pump, int16_t speed)
{
	if(speed<0) speed=0;
	else if(speed > MAX_SPEED) speed = MAX_SPEED;
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, speed);//设置占空比
	pump->pwm = speed*100/MAX_SPEED;
	user_pump_info("已修改泵的占空比为%.2f%%",100.0*speed/MAX_SPEED);
}

void PUMP_openPump(struct _pump *pump)
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	pump->status = 1;
	user_pump_info("泵已进入准备状态");
}

void PUMP_closePump(struct _pump *pump)
{
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);//关闭PWM
	pump->status = 0;
	user_pump_info("泵已退出工作状态");
}
