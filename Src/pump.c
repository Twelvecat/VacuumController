#include "pump.h"

void ChangeSpeed(int16_t speed)
{
	if(speed<0) speed=0;
	else if(speed > MAX_SPEED) speed = MAX_SPEED;
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, speed);//设置占空比
	user_pump_info("已修改泵的占空比为%.2f%%",100.0*speed/MAX_SPEED);
}

void OpenPump(void)
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	user_pump_info("泵已进入准备状态");
}

void ClosePump(void)
{
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);//关闭PWM
	user_pump_info("泵已退出工作状态");
}
