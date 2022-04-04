#ifndef __RELAY_H__
#define __RELAY_H__

#include "main.h"
#include "usart.h"

struct _relay{
	uint8_t status;            	//继电器的状态: 0为关闭，1为导通
	uint8_t id;					//继电器的序号: 1为继电器1，2为继电器2
};

extern struct _relay relay_A; //创建继电器1

void RELAY_InitRelay(struct _relay *relay, uint8_t id);
void RELAY_OpenRelay(struct _relay *relay);
void RELAY_CloseRelay(struct _relay *relay);

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
