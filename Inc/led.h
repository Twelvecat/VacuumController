#ifndef __LED_H__
#define __LED_H__

#include "main.h"
#include "usart.h"

void InitLED(void);
void InitRGB(void);
void OpenLED(void);
void ToggleLED(void);
void OpenRGB_R(void);
void OpenRGB_G(void);
void OpenRGB_B(void);
void CloseLED(void);
void CloseRGB_R(void);
void CloseRGB_G(void);
void CloseRGB_B(void);

#define USER_LED_DEBUG
#ifdef USER_LED_DEBUG
	#define user_led_printf(format, ...) printf( format "\r\n", ##__VA_ARGS__)
	#define user_led_info(format, ...) printf("[\tled]info:" format "\r\n", ##__VA_ARGS__)
	#define user_led_debug(format, ...) printf("[\tled]debug:" format "\r\n", ##__VA_ARGS__)
	#define user_led_error(format, ...) printf("[\tled]error:" format "\r\n",##__VA_ARGS__)
#else
	#define user_led_printf(format, ...)
	#define user_led_info(format, ...)
	#define user_led_debug(format, ...)
	#define user_led_error(format, ...)
#endif

#endif /* __LED_H__ */
