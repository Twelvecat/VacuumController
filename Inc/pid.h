#ifndef __PID_H__
#define __PID_H__

#include "main.h"
#include "math.h"

//#define USER_PID_DEBUG
#ifdef USER_PID_DEBUG
	#define user_pid_printf(format, ...) printf( format "\r\n", ##__VA_ARGS__)
	#define user_pid_info(format, ...) printf("[\tpid]info:" format "\r\n", ##__VA_ARGS__)
	#define user_pid_debug(format, ...) printf("[\tpid]debug:" format "\r\n", ##__VA_ARGS__)
	#define user_pid_error(format, ...) printf("[\tpid]error:" format "\r\n",##__VA_ARGS__)
#else
	#define user_pid_printf(format, ...)
	#define user_pid_info(format, ...)
	#define user_pid_debug(format, ...)
	#define user_pid_error(format, ...)
#endif

struct _pid{
    float SetSpeed;            	//定义设定值
    float ActualSpeed;        	//定义实际值
    float err;                	//定义偏差值
    float err_last;            	//定义上一个偏差值
    float Kp,Ki,Kd;            	//定义比例、积分、微分系数
    float voltage;            	//定义电压值（控制执行器的变量）
    float integral;            	//定义积分值
    float umax;									//正饱和值
    float umin;									//负饱和值
		float dead_band;						//死区，单位为百帕
};

extern struct _pid pid;

void PID_init(struct _pid* pid);
void PID_posRealize(struct _pid* pid, float speed, float ActualSpeed);

#endif /* __PID_H__ */
