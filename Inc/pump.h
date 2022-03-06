#ifndef __PUMP_H__
#define __PUMP_H__

#define MAX_SPEED 400

#include "main.h"
#include "tim.h"
#include "usart.h"

void ChangeSpeed(int16_t speed);
void OpenPump(void);
void ClosePump(void);

#define USER_PUMP_DEBUG
#ifdef USER_PUMP_DEBUG
	#define user_pump_printf(format, ...) printf( format "\r\n", ##__VA_ARGS__)
	#define user_pump_info(format, ...) printf("[\tpump]info:" format "\r\n", ##__VA_ARGS__)
	#define user_pump_debug(format, ...) printf("[\tpump]debug:" format "\r\n", ##__VA_ARGS__)
	#define user_pump_error(format, ...) printf("[\tpump]error:" format "\r\n",##__VA_ARGS__)
#else
	#define user_pump_printf(format, ...)
	#define user_pump_info(format, ...)
	#define user_pump_debug(format, ...)
	#define user_pump_error(format, ...)
#endif

#endif /* __PUMP_H__ */
