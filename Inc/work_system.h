#ifndef __WORK_SYSTEM_H__
#define __WORK_SYSTEM_H__

#include "main.h"


#define USER_WORK_SYSTEM_DEBUG
#ifdef USER_WORK_SYSTEM_DEBUG
	#define user_worksys_printf(format, ...) printf( format "\r\n", ##__VA_ARGS__)
	#define user_worksys_info(format, ...) printf("[\tworksys]info:" format "\r\n", ##__VA_ARGS__)
	#define user_worksys_debug(format, ...) printf("[\tworksys]debug:" format "\r\n", ##__VA_ARGS__)
	#define user_worksys_error(format, ...) printf("[\tworksys]error:" format "\r\n",##__VA_ARGS__)
#else
	#define user_worksys_printf(format, ...)
	#define user_worksys_info(format, ...)
	#define user_worksys_debug(format, ...)
	#define user_worksys_error(format, ...)
#endif

struct _system{
	struct _pump *pump;		//创建一个泵
	struct _pid *pid;		//创建一个pid
	uint8_t pid_mode;			//pid模式：0为积分分离pid，1为模糊pid
	struct HP5806 *hp5806_A, *hp5806_B;	//创建外置与板载传感器
	float set_value; //设定负压
	struct _relay *relay_A; //创建继电器1
	struct _rgb *rgb;//创建RGB灯
	uint8_t leak_status;			//泄漏标志位：0为未泄漏，1为泄漏
	uint8_t wifi_status;			//wifi标志位：0为未连接，1为已连接
	uint8_t sys_last_status;		//上次工作状态
  uint8_t sys_status;            	//系统工作状态：1-准备态、2-运行态、3-故障态、4-急停态、5-调试态、6-手动态
};

extern struct _system system;


void system_init(void);
void system_into_preparedness(void);
void system_into_operating(void);
void system_into_warning(void);
void system_into_stop(void);
void system_into_debug(void);
void system_back(void);

#endif /* __WORK_SYSTEM_H__ */
