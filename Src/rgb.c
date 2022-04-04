#include "rgb.h"

struct _rgb rgb;//����RGB��

void RGB_Init(struct _rgb *rgb){
	RGB_Change(rgb, 0);
	user_rgb_info("RGB��ʼ�����");
}

/*****************************************************************************
 * status: 000=0 001=1 010=2 100=4 011=3 110=6 111=7 101=5
 * RGB��Ӧ��R��4��G��2��B��1
 * ����ֵ����ɫ-1����ɫ-4����ɫ-2����ɫ-6
 *****************************************************************************/
void RGB_Change(struct _rgb *rgb, uint8_t status){	
	rgb->R_status = (status & 4) >> 2;
	rgb->G_status = (status & 2) >> 1;
	rgb->B_status = (status & 1);

	if(rgb->R_status == 0)CloseRGB_R();
	else if (rgb->R_status == 1)OpenRGB_R();

	if(rgb->G_status == 0)CloseRGB_G();
	else if (rgb->G_status == 1)OpenRGB_G();

	if(rgb->B_status == 0)CloseRGB_B();
	else if (rgb->B_status == 1)OpenRGB_B();
}

void OpenRGB_R(void){
	HAL_GPIO_WritePin(RGB_R_GPIO_Port, RGB_R_Pin, GPIO_PIN_RESET);
	user_rgb_info("RGB�ѵ������");
}

void OpenRGB_G(void){
	HAL_GPIO_WritePin(RGB_G_GPIO_Port, RGB_G_Pin, GPIO_PIN_RESET);
	user_rgb_info("RGB�ѵ����̵�");
}

void OpenRGB_B(void){
	HAL_GPIO_WritePin(RGB_B_GPIO_Port, RGB_B_Pin, GPIO_PIN_RESET);
	user_rgb_info("RGB�ѵ����׵�");
}

void CloseRGB_R(void){
	HAL_GPIO_WritePin(RGB_R_GPIO_Port, RGB_R_Pin, GPIO_PIN_SET);
	user_rgb_info("RGB��Ϩ����");
}

void CloseRGB_G(void){
	HAL_GPIO_WritePin(RGB_G_GPIO_Port, RGB_G_Pin, GPIO_PIN_SET);
	user_rgb_info("RGB��Ϩ���̵�");
}

void CloseRGB_B(void){
	HAL_GPIO_WritePin(RGB_B_GPIO_Port, RGB_B_Pin, GPIO_PIN_SET);
	user_rgb_info("RGB��Ϩ������");
}
