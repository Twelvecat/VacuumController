#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "usart.h"
#include "main.h"

#define USER_TOUCH_DEBUG
#ifdef USER_TOUCH_DEBUG
	#define user_touch_printf(format, ...) printf( format "\r\n", ##__VA_ARGS__)
	#define user_touch_info(format, ...) printf("[\ttouch]info:" format "\r\n", ##__VA_ARGS__)
	#define user_touch_debug(format, ...) printf("[\ttouch]debug:" format "\r\n", ##__VA_ARGS__)
	#define user_touch_error(format, ...) printf("[\ttouch]error:" format "\r\n",##__VA_ARGS__)
#else
	#define user_touch_printf(format, ...)
	#define user_touch_info(format, ...)
	#define user_touch_debug(format, ...)
	#define user_touch_error(format, ...)
#endif


#define BUFFER_SIZE 2048//指令缓冲区大小，用户可根据自己单片机的容量修改
#define USER_R3 0xA5//帧头
#define USER_RA 0x5A//帧头

#define registor_read 0x81//寄存器读
#define registor_write 0x80//寄存器写
#define variable_read 0x83//变量读
#define variable_write 0x82//变量写
#define curve_write 0x84//实时曲线

#define UIaddr_hp5806_B_pres 0x0080
#define UIaddr_hp5806_A_pres 0x0084
#define UIaddr_hp5806_B_temp 0x0088
#define UIaddr_hp5806_A_temp 0x008C

#define UIaddr_pump_status 0x0090
#define UIaddr_pump_freq 0x0094
#define UIaddr_pump_pwm 0x0098

#define UIaddr_pid_p 0x009C
#define UIaddr_pid_i 0x00A0
#define UIaddr_pid_d 0x00A4
#define UIaddr_pid_mode 0x00A8

#define UIaddr_set_pres 0x00AC
#define UIaddr_current_pres 0x00B0

#define UIaddr_relay_status 0x00B4
#define UIaddr_leak_status 0x00B8
#define UIaddr_wifi_status 0x00BC
#define UIaddr_system_status 0x00C0

#define UIaddr_manual_pwm 0x00C4
#define UIaddr_set_value 0x0826

void TOUCH_variable_write(uint16_t adds, uint16_t data);
void TOUCH_UpdataUI(void);
void TOUCH_UpdataHP5806(void);
void TOUCH_UpdataPump(void);
void TOUCH_UpdataPID(void);
void TOUCH_UpdataPres(void);
void TOUCH_UpdataStatus(void);
void TOUCH_curve_write(uint8_t id,uint16_t data);
void TOUCH_extract_command(void);
void TOUCH_deal_command(uint8_t *p_Cmdbuf);
void TOUCH_run_command(uint16_t cmd);

#endif /* __TOUCH_H__ */
