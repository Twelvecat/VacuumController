#include "work_system.h"

struct _system system;
extern struct _pump pump;
extern struct _pid pid;
extern struct HP5806 hp5806_A, hp5806_B;	//创建外置与板载传感器
extern struct _relay relay_A; //创建继电器1
extern struct _rgb rgb; //创建继电器1

void system_init(void){
	system.pump = &pump;
	PUMP_init(system.pump);
	system.pid = &pid;
	PID_init(system.pid);
	system.pid_mode = 0;
	system.hp5806_A = &hp5806_A;
	system.hp5806_B = &hp5806_B;
	user_worksys_info("板载HP5806尝试初始化");
	HP5806_Init(&hi2c1, system.hp5806_A, 1);
	user_worksys_info("外置HP5806尝试初始化");
	HP5806_Init(&hi2c2, system.hp5806_B, 2);
	system.relay_A = &relay_A;
 	user_worksys_info("两路继电器尝试初始化");
	RELAY_InitRelay(system.relay_A, 1);
	user_worksys_info("RGB尝试初始化");
	system.rgb = &rgb;
	RGB_Init(system.rgb);

	system.set_value = 0; 			//设定负压
	system.leak_status = 0;			//泄漏标志位：0为未泄漏，1为泄漏
	system.wifi_status = 0;			//wifi标志位：0为未连接，1为已连接
	system.sys_status = 1;

	system_into_preparedness();
}

void system_into_preparedness(void){
	system.sys_last_status=system.sys_status;
	system.sys_status=1;
	PUMP_changeSpeed(system.pump, 0);
	PUMP_openPump(system.pump);
	RGB_Change(system.rgb,1);
}

void system_into_operating(void){
	system.sys_last_status=system.sys_status;
	system.sys_status=2;
	PUMP_openPump(system.pump);
	RGB_Change(system.rgb,2);
}

void system_into_warning(void){
	system.sys_last_status=system.sys_status;
	system.sys_status=3;
	PUMP_openPump(system.pump);
	RGB_Change(system.rgb,6);
}

void system_into_stop(void){
	system.sys_last_status=system.sys_status;
	system.sys_status=4;
	PUMP_closePump(system.pump);
	RGB_Change(system.rgb,4);
}

void system_into_debug(void){
	system.sys_last_status=system.sys_status;
	system.sys_status=5;
	PUMP_changeSpeed(system.pump, 0);
	PUMP_openPump(system.pump);
	RGB_Change(system.rgb,0);
}

void system_into_manual(void){
	system.sys_last_status=system.sys_status;
	system.sys_status=6;
	PUMP_changeSpeed(system.pump, 0);
	PUMP_openPump(system.pump);
	RGB_Change(system.rgb,0);
}

void system_back(void){
	switch (system.sys_last_status)
	{
		case 1:
		{
			system_into_preparedness();
			break;
		}
		case 2:
		{
			system_into_operating();
			break;
		}
		case 3:
		{
			system_into_warning();
			break;
		}
		case 4:
		{
			system_into_stop();
			break;
		}
		case 5:
		{
			system_into_debug();
			break;
		}
		case 6:
		{
			system_into_manual();
			break;
		}
		default: break;
	}
}
