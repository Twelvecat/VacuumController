#include "led.h"

void InitLED(void){
	CloseLED();
	user_led_info("LED初始化完成");
}

void InitRGB(void){
	CloseRGB_R();
	CloseRGB_G();
	CloseRGB_B();
	user_led_info("RGB初始化完成");
}

void OpenLED(void){
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	user_led_info("LED已点亮");
}

void ToggleLED(void){
	HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
	user_led_info("LED已点翻转");
	HAL_GPIO_TogglePin(RGB_G_GPIO_Port,RGB_G_Pin);
	HAL_GPIO_TogglePin(RGB_R_GPIO_Port,RGB_R_Pin);
	
}

void OpenRGB_R(void){
	HAL_GPIO_WritePin(RGB_R_GPIO_Port, RGB_R_Pin, GPIO_PIN_RESET);
	user_led_info("RGB已点亮红灯");
}

void OpenRGB_G(void){
	HAL_GPIO_WritePin(RGB_G_GPIO_Port, RGB_G_Pin, GPIO_PIN_RESET);
	user_led_info("RGB已点亮绿灯");
}

void OpenRGB_B(void){
	HAL_GPIO_WritePin(RGB_B_GPIO_Port, RGB_B_Pin, GPIO_PIN_RESET);
	user_led_info("RGB已点亮白灯");
}

void CloseLED(void){
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	user_led_info("LED已熄灭");
}

void CloseRGB_R(void){
	HAL_GPIO_WritePin(RGB_R_GPIO_Port, RGB_R_Pin, GPIO_PIN_SET);
	user_led_info("RGB已熄灭红灯");
}

void CloseRGB_G(void){
	HAL_GPIO_WritePin(RGB_G_GPIO_Port, RGB_G_Pin, GPIO_PIN_SET);
	user_led_info("RGB已熄灭绿灯");
}

void CloseRGB_B(void){
	HAL_GPIO_WritePin(RGB_B_GPIO_Port, RGB_B_Pin, GPIO_PIN_SET);
	user_led_info("RGB已熄灭蓝灯");
}

