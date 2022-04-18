#include "work_system.h"

struct _system system;
struct _system_status system_status;
extern struct _pump pump;
extern struct _pid pid;
extern struct _time time;
extern struct HP5806 hp5806_A, hp5806_B;	//创建外置与板载传感器
extern struct _relay relay_A,relay_B; //创建继电器1
extern struct _rgb rgb; //创建继电器1
uint8_t flag_pump_stop = 0;

void system_init(void){
	system.system_status = &system_status;
	system_StatusInit(system.system_status);
	user_worksys_info("系统状态尝试初始化");
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
	system.relay_B = &relay_B;
 	user_worksys_info("两路继电器尝试初始化");
	RELAY_InitRelay(system.relay_A, 1);
	RELAY_InitRelay(system.relay_B, 2);
	user_worksys_info("时间模块尝试初始化");
	system.time = &time;
	TIM_TimeInit(system.time);
	user_worksys_info("RGB尝试初始化");
	system.rgb = &rgb;
	RGB_Init(system.rgb);


	system.set_value = 0; 			//设定负压
	system.output_value = 0; //输出值
	system.output_manual = 0; //手动输出值
	system.leak_status = 0;			//泄漏标志位：0为未泄漏，1为泄漏
	system.wifi_status = 0;			//wifi标志位：0为未连接，1为已连接
	


	system_into_prep();
}

void system_StatusInit(struct _system_status *system_status)
{
	system_status->current = 0;
	system_status->last  = 0;
	system_status->last_2 = 0;
	system_status->flag_change = 0;
}

void system_StatusSwitch(struct _system_status *system_status, uint8_t status)
{
	if(status < 8){
		if(system_status->current != status){
			system_status->last_2 = system_status->last;
			system_status->last  = system_status->current;
			system_status->current = status;
			system_status->flag_change = 1;
		}
	}
}

void system_StatusBack(struct _system_status *system_status)
{//用于急停退出，进入准备态
	system_status->current = system_status->last;
	system_status->last = system_status->last_2;
	system_StatusSwitch(system.system_status, 1);
	system_status->flag_change = 1;
	TOUCH_variable_write(UIaddr_pump_status, UIaddr_pump_off);
	uint8_t buffer_registor_data[7] = {USER_R3, USER_RA, 0x04, registor_write, 0x03, 0x00, 0x03};//急停后进入最初页面
	HAL_UART_Transmit(&huart1, buffer_registor_data, 7, 10);
	HAL_UART_Transmit(&huart1, buffer_registor_data, 7, 10);
	HAL_UART_Transmit(&huart1, buffer_registor_data, 7, 10);
}

void system_status_run(void)
{
	if(system.system_status->flag_change==1)
	{
		system.system_status->flag_change = 0;
		switch (system.system_status->current)
		{
			case 0:
			{
				system_into_init();
				break;
			}
			case 1:
			{
				system_into_prep();
				break;
			}
			case 2:
			{
				system_auto_run();
				break;
			}
			case 3:
			{
				system_auto_pause();
				break;
			}
			case 4:
			{
				system_manual_run();
				break;
			}
			case 5:
			{
				system_manual_pause();
				break;
			}
			case 7:
			{
				system_into_setting();
				break;
			}
			default: 
			{
				break;
			}
		}
	}
}

void system_into_init(void){
	;
}

void system_into_prep(void){
	system.output_value = 0;
	PUMP_openPump(system.pump);
	RGB_Change(system.rgb,1);
	TIM_TimeExit(system.time);
	PID_init(system.pid);
}

void system_auto_run(void){
	PUMP_openPump(system.pump);
	RGB_Change(system.rgb,2);
	system.time->flag_time = 1;
}

void system_auto_pause(void){
	system.output_value = 0;
	PUMP_openPump(system.pump);
	RGB_Change(system.rgb,1);
	system.time->flag_time = 0;
}

void system_manual_run(void){
	
	PUMP_openPump(system.pump);
	RGB_Change(system.rgb,2);
	system.time->flag_time = 1;
	if(system.time->remainder_sec==0)system.time->remainder_sec = system.time->setting_sec;
}

void system_manual_pause(void){
	system.output_value = 0;
	system.time->flag_time = 0;
	PUMP_openPump(system.pump);
	RGB_Change(system.rgb,1);
}

void system_into_stop(void){
	system_StatusSwitch(system.system_status, 6);
	system.system_status->flag_change = 0;
	system.time->flag_time = 0;
	system.output_value = 0;
	PUMP_closePump(system.pump);
	RGB_Change(system.rgb,4);
	TIM_TimeExit(system.time);
	flag_pump_stop = 1;
	uint8_t buffer_registor_data[7] = {USER_R3, USER_RA, 0x04, registor_write, 0x03, 0x00, 0x20};//记得修改
	HAL_UART_Transmit(&huart1, buffer_registor_data, 7, 10);
	HAL_UART_Transmit(&huart1, buffer_registor_data, 7, 10);
	HAL_UART_Transmit(&huart1, buffer_registor_data, 7, 10);
}


void system_into_setting(void){
	system.output_value = 0;
	PUMP_openPump(system.pump);
	RGB_Change(system.rgb,0);
}

void system_manual_finish(void)
{
	uint8_t buffer_registor_data[7] = {USER_R3, USER_RA, 0x04, registor_write, 0x03, 0x00, 0x14};
	HAL_UART_Transmit(&huart1, buffer_registor_data, 7, 10);
	system_StatusSwitch(system.system_status, 1);
}

void system_MCUreset(void)
{
	__set_FAULTMASK(1); //关中断
	NVIC_SystemReset(); //复位   
}
