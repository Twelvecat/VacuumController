#ifndef __RELAY_H__
#define __RELAY_H__

#include "main.h"
#include "usart.h"

void InintRelay(void);
void OpenRelay(uint8_t ID);
void CloseRelay(uint8_t ID);

#define USER_RELAY_DEBUG
#ifdef USER_RELAY_DEBUG
	#define user_relay_printf(format, ...) printf( format "\r\n", ##__VA_ARGS__)
	#define user_relay_info(format, ...) printf("[\trelay]info:" format "\r\n", ##__VA_ARGS__)
	#define user_relay_debug(format, ...) printf("[\trelay]debug:" format "\r\n", ##__VA_ARGS__)
	#define user_relay_error(format, ...) printf("[\trelay]error:" format "\r\n",##__VA_ARGS__)
#else
	#define user_relay_printf(format, ...)
	#define user_relay_info(format, ...)
	#define user_relay_debug(format, ...)
	#define user_relay_error(format, ...)
#endif

#endif /* __RELAY_H__ */
