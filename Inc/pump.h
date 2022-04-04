#ifndef __PUMP_H__
#define __PUMP_H__

#define MAX_SPEED 400

#include "main.h"
#include "tim.h"
#include "usart.h"

void PUMP_changeSpeed(struct _pump *pump, int16_t speed);
void PUMP_openPump(struct _pump *pump);
void PUMP_closePump(struct _pump *pump);
void PUMP_init(struct _pump *pump);

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

struct _pump{
  float pwm;            		//泵的占空比,范围0%-100%之间的浮点数
	uint8_t status;            	//泵的状态: 0为停止，1为运行或准备
	float frequency;          //泵的运行频率，单位为Hz
};

extern struct _pump pump;

#endif /* __PUMP_H__ */
