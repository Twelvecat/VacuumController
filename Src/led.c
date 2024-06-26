#include "led.h"

//void LED_init(void){
//	LED_Close();
//	user_led_info("LED初始化完成");
//}

//void LED_Open(void){
//	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
//	user_led_info("LED已点亮");
//}

//void LED_Toggle(void){
//	HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
//	user_led_info("LED已点翻转");
//}

//void LED_Close(void){
//	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
//	user_led_info("LED已熄灭");
//}

void RGB_Init(struct _rgb *rgb){
	RGB_Change(rgb, 0);
	user_led_info("RGB初始化完成");
}

/*****************************************************************************
 * status: 000=0 001=1 010=2 100=4 011=3 110=6 111=7 101=5
 * RGB对应：R是4，G是2，B是1
 * 典型值：白色-1，红色-4，绿色-2，黄色-6
 *****************************************************************************/
void RGB_Change(struct _rgb *rgb, uint8_t status){	
	rgb->R_status = (status & 0b100) >> 2;
	rgb->G_status = (status & 0b010) >> 1;
	rgb->B_status = (status & 0b001);

	if(rgb->R_status == 0)CloseRGB_R();
	else if (rgb->R_status == 1)OpenRGB_R();

	if(rgb->G_status == 0)CloseRGB_G();
	else if (rgb->G_status == 1)OpenRGB_G();

	if(rgb->B_status == 0)CloseRGB_B();
	else if (rgb->B_status == 1)OpenRGB_B();
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

