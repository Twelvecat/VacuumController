#include "pid.h"

struct _pid pid;


void PID_init(struct _pid *pid)
{
	pid->SetSpeed = 0.0;	//设置的压力
	pid->ActualSpeed = 0.0; //实际的压力
	pid->err = 0.0;			//实际误差
	pid->err_last = 0.0;	//上一次的误差
	pid->voltage = 0.0;		//输出值
	pid->integral = 0.0;	//积分值

	pid->Kp = 7.419;//4.63;//1.27;
	pid->Ki = 0.269;//0.67;//0.45;
	pid->Kd = 7.516;//0.67;//0.2;
	pid->umax = 100; //用于积分分离
	pid->umin = -100;
	pid->dead_band = 1.5;
}

void PID_posRealize(struct _pid *pid, float speed, float ActualSpeed)
{
	int index; //用于积分分离,即通过该值来确定是否使用积分常数
	pid->SetSpeed = (float)speed;
	pid->ActualSpeed = (float)ActualSpeed; //采集出实际值
	pid->err = pid->ActualSpeed - pid->SetSpeed;
	user_pid_info("PID差值为:%.2f", pid->err);
	if( (pid->err < pid->dead_band) && (pid->err > -pid->dead_band) ) pid->err = 0; //添加了死区，避开两个传感器的误差
//	if (pid->ActualSpeed > pid->SetSpeed + pid->umax) //若实际值大于最大值
//	{
//		index = 0;
//	}
//	else if (pid->ActualSpeed < pid->SetSpeed + pid->umin)
//	{
//		index = 0;
//	}
//	else
//	{
//		index = 1;
//		pid->integral += pid->err;//这个地方可以再研究研究，取消积分分离后是否需要清空积分状态
//	}
	index = 1;
	pid->integral += pid->err;
	pid->voltage = pid->Kp * pid->err + index * pid->Ki * pid->integral + pid->Kd * (pid->err - pid->err_last);
	if(pid->voltage<0) pid->voltage=0;
	else if(pid->voltage > MAX_SPEED) pid->voltage = MAX_SPEED;
	user_pid_info("PID理论输出:%.2f", pid->voltage);
	pid->err_last = pid->err;
}
