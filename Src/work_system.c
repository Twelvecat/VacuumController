#include "work_system.h"

struct _system system;

void system_init(void){
	system_into_preparedness();
}

void system_into_preparedness(void){
	system.sys_last_status=system.sys_status;
	system.sys_status=1;
	system.pump_status=1;
	ChangeSpeed(0);
	OpenPump();
	InitRGB();
	OpenRGB_B();
}

void system_into_operating(void){
	system.sys_last_status=system.sys_status;
	system.sys_status=2;
	system.pump_status=2;
	OpenPump();
	InitRGB();
	OpenRGB_G();
}

void system_into_warning(void){
	system.sys_last_status=system.sys_status;
	system.sys_status=3;
	system.pump_status=2;
	OpenPump();
	InitRGB();
	OpenRGB_G();
	OpenRGB_R();
}

void system_into_stop(void){
	system.sys_last_status=system.sys_status;
	system.sys_status=4;
	system.pump_status=1;
	ClosePump();
	InitRGB();
	OpenRGB_R();
}

void system_into_debug(void){
	system.sys_last_status=system.sys_status;
	system.sys_status=5;
	system.pump_status=1;
	ChangeSpeed(0);
	OpenPump();
	InitRGB();
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
	default: break;
	}
}
