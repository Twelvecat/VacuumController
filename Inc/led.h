#ifndef __LED_H__
#define __LED_H__

#include "main.h"
#include "usart.h"

//void LED_Open(void);
//void LED_Toggle(void);
//void LED_Close(void);
//void LED_init(void);




void RGB_Init(struct _rgb *rgb);
void RGB_Change(struct _rgb *rgb, uint8_t status);

void OpenRGB_R(void);
void OpenRGB_G(void);
void OpenRGB_B(void);

void CloseRGB_R(void);
void CloseRGB_G(void);
void CloseRGB_B(void);

struct _rgb{
    uint8_t R_status;           //RGB的R状态：0为熄灭，1为开启
    uint8_t G_status;           //RGB的G状态：0为熄灭，1为开启
    uint8_t B_status;           //RGB的B状态：0为熄灭，1为开启
};

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
