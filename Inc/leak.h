#ifndef __LEAK_H__
#define __LEAK_H__

#include "main.h"

void LEAK_cheack(void);
extern uint8_t leak_timer_start_flag;

//#define USER_LEAK_DEBUG
#ifdef USER_LEAK_DEBUG
	#define user_leak_printf(format, ...) printf( format "\r\n", ##__VA_ARGS__)
	#define user_leak_info(format, ...) printf("[\tleak]info:" format "\r\n", ##__VA_ARGS__)
	#define user_leak_debug(format, ...) printf("[\tleak]debug:" format "\r\n", ##__VA_ARGS__)
	#define user_leak_error(format, ...) printf("[\tleak]error:" format "\r\n",##__VA_ARGS__)
#else
	#define user_leak_printf(format, ...)
	#define user_leak_info(format, ...)
	#define user_leak_debug(format, ...)
	#define user_leak_error(format, ...)
#endif

#endif /* __LEAK_H__ */
