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
	uint8_t sys_last_status;					//上次工作状态
    uint8_t sys_status;            	//系统工作状态：1-准备态、2-运行态、3-故障态、4-急停态、5-调试态
    uint8_t pump_status;        	//泵工作状态：1-准备态、2-运行态
		//未完待续
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
