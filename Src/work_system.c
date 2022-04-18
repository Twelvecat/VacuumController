#include "work_system.h"

struct _system MCUsystem;
struct _system_status system_status;
extern struct _pump pump;
extern struct _pid pid;
extern struct _time time;
extern struct HP5806 hp5806_A, hp5806_B;	//创建外置与板载传感器
extern struct _relay relay_A,relay_B; //创建继电器1
extern struct _rgb rgb; //创建继电器1
uint8_t flag_pump_stop = 0;

void system_init(void){
	MCUsystem.system_status = &system_status;
	system_StatusInit(MCUsystem.system_status);
	user_worksys_info("系统状态尝试初始化");
	MCUsystem.pump = &pump;
	PUMP_init(MCUsystem.pump);
	MCUsystem.pid = &pid;
	PID_init(MCUsystem.pid);
	MCUsystem.pid_mode = 0;
	MCUsystem.hp5806_A = &hp5806_A;
	MCUsystem.hp5806_B = &hp5806_B;
	user_worksys_info("板载HP5806尝试初始化");
	HP5806_Init(&hi2c1, MCUsystem.hp5806_A, 1);
	user_worksys_info("外置HP5806尝试初始化");
	HP5806_Init(&hi2c2, MCUsystem.hp5806_B, 2);
	MCUsystem.relay_A = &relay_A;
	MCUsystem.relay_B = &relay_B;
 	user_worksys_info("两路继电器尝试初始化");
	RELAY_InitRelay(MCUsystem.relay_A, 1);
	RELAY_InitRelay(MCUsystem.relay_B, 2);
	user_worksys_info("时间模块尝试初始化");
	MCUsystem.time = &time;
	TIM_TimeInit(MCUsystem.time);
	user_worksys_info("RGB尝试初始化");
	MCUsystem.rgb = &rgb;
	RGB_Init(MCUsystem.rgb);


	MCUsystem.set_value = 0; 			//设定负压
	MCUsystem.output_value = 0; //输出值
	MCUsystem.output_manual = 0; //手动输出值
	MCUsystem.leak_status = 0;			//泄漏标志位：0为未泄漏，1为泄漏
	MCUsystem.wifi_status = 0;			//wifi标志位：0为未连接，1为已连接
	


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
	system_StatusSwitch(MCUsystem.system_status, 1);
	system_status->flag_change = 1;
	TOUCH_variable_write(UIaddr_pump_status, UIaddr_pump_off);
	uint8_t buffer_registor_data[7] = {USER_R3, USER_RA, 0x04, registor_write, 0x03, 0x00, 0x03};//急停后进入最初页面
	HAL_UART_Transmit(&huart1, buffer_registor_data, 7, 10);
	HAL_UART_Transmit(&huart1, buffer_registor_data, 7, 10);
	HAL_UART_Transmit(&huart1, buffer_registor_data, 7, 10);
}

void system_status_run(void)
{
	if(MCUsystem.system_status->flag_change==1)
	{
		MCUsystem.system_status->flag_change = 0;
		switch (MCUsystem.system_status->current)
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
	MCUsystem.output_value = 0;
	PUMP_openPump(MCUsystem.pump);
	RGB_Change(MCUsystem.rgb,1);
	TIM_TimeExit(MCUsystem.time);
	PID_init(MCUsystem.pid);
}

void system_auto_run(void){
	PUMP_openPump(MCUsystem.pump);
	RGB_Change(MCUsystem.rgb,2);
	MCUsystem.time->flag_time = 1;
}

void system_auto_pause(void){
	MCUsystem.output_value = 0;
	PUMP_openPump(MCUsystem.pump);
	RGB_Change(MCUsystem.rgb,1);
	MCUsystem.time->flag_time = 0;
}

void system_manual_run(void){
	
	PUMP_openPump(MCUsystem.pump);
	RGB_Change(MCUsystem.rgb,2);
	MCUsystem.time->flag_time = 1;
	if(MCUsystem.time->remainder_sec==0)MCUsystem.time->remainder_sec = MCUsystem.time->setting_sec;
}

void system_manual_pause(void){
	MCUsystem.output_value = 0;
	MCUsystem.time->flag_time = 0;
	PUMP_openPump(MCUsystem.pump);
	RGB_Change(MCUsystem.rgb,1);
}

void system_into_stop(void){
	system_StatusSwitch(MCUsystem.system_status, 6);
	MCUsystem.system_status->flag_change = 0;
	MCUsystem.time->flag_time = 0;
	MCUsystem.output_value = 0;
	PUMP_closePump(MCUsystem.pump);
	RGB_Change(MCUsystem.rgb,4);
	TIM_TimeExit(MCUsystem.time);
	flag_pump_stop = 1;
	uint8_t buffer_registor_data[7] = {USER_R3, USER_RA, 0x04, registor_write, 0x03, 0x00, 0x20};//记得修改
	HAL_UART_Transmit(&huart1, buffer_registor_data, 7, 10);
	HAL_UART_Transmit(&huart1, buffer_registor_data, 7, 10);
	HAL_UART_Transmit(&huart1, buffer_registor_data, 7, 10);
}


void system_into_setting(void){
	MCUsystem.output_value = 0;
	PUMP_openPump(MCUsystem.pump);
	RGB_Change(MCUsystem.rgb,0);
}

void system_manual_finish(void)
{
	uint8_t buffer_registor_data[7] = {USER_R3, USER_RA, 0x04, registor_write, 0x03, 0x00, 0x14};
	HAL_UART_Transmit(&huart1, buffer_registor_data, 7, 10);
	system_StatusSwitch(MCUsystem.system_status, 1);
}

void system_MCUreset(void)
{
	__set_FAULTMASK(1); //关中断
	NVIC_SystemReset(); //复位   
}
