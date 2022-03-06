#ifndef __KEY_H__
#define __KEY_H__

#include "main.h"


#define USER_KEY_DEBUG
#ifdef USER_KEY_DEBUG
	#define user_key_printf(format, ...) printf( format "\r\n", ##__VA_ARGS__)
	#define user_key_info(format, ...) printf("[\tkey]info:" format "\r\n", ##__VA_ARGS__)
	#define user_key_debug(format, ...) printf("[\tkey]debug:" format "\r\n", ##__VA_ARGS__)
	#define user_key_error(format, ...) printf("[\tkey]error:" format "\r\n",##__VA_ARGS__)
#else
	#define user_key_printf(format, ...)
	#define user_key_info(format, ...)
	#define user_key_debug(format, ...)
	#define user_key_error(format, ...)
#endif



#define GPIO_Read_Key_1()  HAL_GPIO_ReadPin(KEY_STOP_GPIO_Port, KEY_STOP_Pin)



extern uint32_t Time_Scan;//the time of the TIM for 10ms
extern int8_t KEY_Dir1;//dir1 1 or-1,put 0 as default

void Scan_The_Key(void);//add this function into a 10ms TIM
unsigned char Function_For_Key(void);

#endif /* __KEY_H__ */
