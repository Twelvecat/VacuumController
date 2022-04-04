#ifndef __RGB_H__
#define __RGB_H__

#include "main.h"
#include "usart.h"

void OpenRGB_R(void);
void OpenRGB_G(void);
void OpenRGB_B(void);

void CloseRGB_R(void);
void CloseRGB_G(void);
void CloseRGB_B(void);

struct _rgb{
    uint8_t R_status;           //RGB��R״̬��0ΪϨ��1Ϊ����
    uint8_t G_status;           //RGB��G״̬��0ΪϨ��1Ϊ����
    uint8_t B_status;           //RGB��B״̬��0ΪϨ��1Ϊ����
};

extern struct _rgb rgb;//����RGB��

void RGB_Init(struct _rgb *rgb);
void RGB_Change(struct _rgb *rgb, uint8_t status);

#define USER_RGB_DEBUG
#ifdef USER_RGB_DEBUG
	#define user_rgb_printf(format, ...) printf( format "\r\n", ##__VA_ARGS__)
	#define user_rgb_info(format, ...) printf("[\trgb]info:" format "\r\n", ##__VA_ARGS__)
	#define user_rgb_debug(format, ...) printf("[\trgb]debug:" format "\r\n", ##__VA_ARGS__)
	#define user_rgb_error(format, ...) printf("[\trgb]error:" format "\r\n",##__VA_ARGS__)
#else
	#define user_rgb_printf(format, ...)
	#define user_rgb_info(format, ...)
	#define user_rgb_debug(format, ...)
	#define user_rgb_error(format, ...)
#endif

#endif /* __RGB_H__ */