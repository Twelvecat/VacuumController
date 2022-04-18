#ifndef __WORK_SYSTEM_H__
#define __WORK_SYSTEM_H__

#include "main.h"


//#define USER_WORK_SYSTEM_DEBUG
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

struct _system_status{
	uint8_t last_2;		//上上次工作状态
	uint8_t last;		  //上次工作状态
  uint8_t current;  //系统工作状态：0-初始化态、1-准备态、2-自动运行态、3-自动暂停态、4-手动运行态、5-手动暂停态、6-急停态、7-设置态
	uint8_t flag_change; //是否更新过状态
};

extern struct _system_status system_status;

struct _system{
	struct _pump *pump;		//创建一个泵
	struct _pid *pid;		//创建一个pid
	struct HP5806 *hp5806_A, *hp5806_B;	//创建外置与板载传感器
	struct _relay *relay_A,*relay_B; //创建继电器1
	struct _rgb *rgb;//创建RGB灯
	struct _time *time;  //时间模块
	struct _system_status *system_status;
	float set_value; //设定负压
	float output_value; //输出值
	float output_manual; //手动输出值
	uint8_t pid_mode;			//pid模式：0为积分分离pid，1为模糊pid
	uint8_t leak_status;			//泄漏标志位：0为未泄漏，1为泄漏
	uint8_t wifi_status;			//wifi标志位：0为未连接，1为已连接
};

extern struct _system MCUsystem;
extern uint8_t flag_pump_stop;

void system_init(void);
void system_StatusSwitch(struct _system_status *system_status, uint8_t status);
void system_StatusInit(struct _system_status *system_status);
void system_StatusBack(struct _system_status *system_status);
void system_status_run(void);

void system_into_init(void);
void system_into_prep(void);
void system_auto_run(void);
void system_auto_pause(void);
void system_manual_run(void);
void system_manual_pause(void);
void system_into_stop(void);
void system_into_setting(void);
void system_manual_finish(void);
void system_MCUreset(void);

#endif /* __WORK_SYSTEM_H__ */
