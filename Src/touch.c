#include "touch.h"

extern struct _system system;
uint16_t StartNum=0,TalNum=0;
uint8_t CommBuff[BUFFER_SIZE];//定义指令缓冲区
uint8_t buffer_variable_data[8] = {USER_R3, USER_RA, 0x05, variable_write, 0x00, 0x00, 0x00, 0x00};
uint8_t buffer_registor_data[6] = {USER_R3, USER_RA, 0x03, variable_read, 0x00, 0x00};


void TOUCH_Reste(void)
{
	uint8_t buffer_registor_data[7] = {USER_R3, USER_RA, 0x04, registor_write, 0xEE, 0x5A, 0xA5};//记得修改
	HAL_UART_Transmit(&huart1, buffer_registor_data, 7, 10);
}

void TOUCH_UpdataUI(void)
{
		TOUCH_UpdataHP5806();
		TOUCH_UpdataRelay();
		TOUCH_UpdataPres();
		TOUCH_UpdataTime();
//		TOUCH_UpdataPID();
//		TOUCH_UpdataPres();
//		TOUCH_UpdataStatus();
//		user_touch_info("UI更新完毕");
}

void TOUCH_UpdataHP5806(void)
{//仪表盘
		TOUCH_variable_write(UIaddr_hp5806_B_pres, (uint16_t)(system.hp5806_B->Pcomp/10));
		TOUCH_variable_write(UIaddr_hp5806_A_pres, (uint16_t)(system.hp5806_A->Pcomp/10));
		TOUCH_variable_write(UIaddr_hp5806_B_temp, (int16_t)(system.hp5806_B->Tcomp*10));
		TOUCH_variable_write(UIaddr_hp5806_A_temp, (int16_t)(system.hp5806_A->Tcomp*10));
		TOUCH_variable_write(UIaddr_hp5806_B_pres_ptr, (uint16_t)(1200-system.hp5806_B->Pcomp/100));
		TOUCH_variable_write(UIaddr_hp5806_B_temp_ptr, (uint16_t)(15+system.hp5806_B->Tcomp));
	
		user_touch_info("HP5806更新完毕");
}

void TOUCH_UpdataPres(void)
{//负压情况
		uint16_t temp = (uint16_t)(system.hp5806_A->Pcomp/10-system.hp5806_B->Pcomp/10);
		TOUCH_variable_write(UIaddr_set_pres, (uint16_t)(system.set_value*10));
		TOUCH_variable_write(UIaddr_current_pres, temp);
		user_touch_info("负压情况更新完毕");
}

void TOUCH_UpdataRelay(void)
{//继电器情况
		uint16_t temp = (uint16_t)(((system.relay_A->status & 0xFF) << 1) | (system.relay_B->status & 0xFF) );
		TOUCH_variable_write(UIaddr_relay_status, temp);
		user_touch_info("负压情况更新完毕");
}

void TOUCH_UpdataTime(void)
{//时间情况
	TOUCH_variable_write(UIaddr_hour, system.time->hour);
	TOUCH_variable_write(UIaddr_min, system.time->min);
	TOUCH_variable_write(UIaddr_sec, system.time->sec);
	TOUCH_variable_write(UIaddr_setting_sec, system.time->setting_sec);
	TOUCH_variable_write(UIaddr_remainder_sec, system.time->remainder_sec);
	user_touch_info("时间情况更新完毕");
}

void TOUCH_change_data(uint8_t *list, uint8_t id, uint16_t data)
{
	list[id] = (uint8_t)((data & 0xFF00)>>8);
	list[id+1] = (uint8_t)(data & 0x00FF);
}

/*
0:外置压力-黑-300~1200hPa-无小数
1:板载压力-黄-300~1200hPa-无小数
2:设定负压-红-300~1200hPa-无小数
3:管道温度-黑-0~100℃-无小数，当前温度+15后发送
4:工作温度-黄-0~100℃-无小数，当前温度+15后发送
5:设定值-红-0~900hPa-无小数
6:当前值-绿-0~900hPa-无小数
*/
void TOUCH_curve_write(void)
{//实时曲线
	//初始化数据
	uint8_t buffer_curve_data[19] = {USER_R3, USER_RA, 0x10, curve_write, 0x7F, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00};
	uint16_t data = 0x0000;
	//压力曲线
		data = (uint16_t)(system.hp5806_B->Pcomp/100);
		TOUCH_change_data(buffer_curve_data, 5, data);
		data = (uint16_t)(system.hp5806_A->Pcomp/100);
		TOUCH_change_data(buffer_curve_data, 7, data);
		data = (uint16_t)(system.hp5806_A->Pcomp/100-system.set_value);
		TOUCH_change_data(buffer_curve_data, 9, data);
	//温度曲线
		data = (uint16_t)(15+system.hp5806_B->Tcomp);
		TOUCH_change_data(buffer_curve_data, 11, data);
		data = (uint16_t)(15+system.hp5806_A->Tcomp);
		TOUCH_change_data(buffer_curve_data, 13, data);
	//误差曲线
		data = (uint16_t)(system.set_value);
		TOUCH_change_data(buffer_curve_data, 15, data);
		data = (uint16_t)(system.hp5806_A->Pcomp/100-system.hp5806_B->Pcomp/100);
		TOUCH_change_data(buffer_curve_data, 17, data);
		
	HAL_UART_Transmit(&huart1, buffer_curve_data, 19, 10);
}



void TOUCH_UpdataStatus(void)
{//状态更新（服务于远程），配合上位机开发
	uint8_t buffer_registor_data[7] = {USER_R3, USER_RA, 0x04, registor_write, 0x03, 0x00, 0x00};
	if(system.system_status->current == 1)buffer_registor_data[6]=0x03;
	else if(system.system_status->current == 2)buffer_registor_data[6]=0x04;
	else if(system.system_status->current == 3)buffer_registor_data[6]=0x05;
	else if(system.system_status->current == 4)buffer_registor_data[6]=0x15;
	else if(system.system_status->current == 5)buffer_registor_data[6]=0x16;
	HAL_UART_Transmit(&huart1, buffer_registor_data, 7, 10);
}

void TOUCH_UpdataPump(void)
{
		TOUCH_variable_write(UIaddr_pump_status, UIaddr_pump_off);
//		TOUCH_variable_write(UIaddr_pump_freq, (uint16_t)(system.pump->frequency/100));
//		TOUCH_variable_write(UIaddr_pump_pwm, (uint16_t)(system.pump->pwm*100));
		user_touch_info("PUMP更新完毕");
}

void TOUCH_UpdataPID(void)
{
		TOUCH_variable_write(UIaddr_pid_p, (uint16_t)(system.pid->Kp*100));
		TOUCH_variable_write(UIaddr_pid_i, (uint16_t)(system.pid->Ki*100));
		TOUCH_variable_write(UIaddr_pid_d, (uint16_t)(system.pid->Kd*100));
		TOUCH_variable_write(UIaddr_pid_mode, (uint16_t)system.pid_mode);
		user_touch_info("PID更新完毕");
}


void TOUCH_variable_write(uint16_t adds, uint16_t data)
{
		buffer_variable_data[3] = variable_write;
		buffer_variable_data[4] = (uint8_t)((adds & 0xFF00)>>8);
		buffer_variable_data[5] = (uint8_t)(adds & 0x00FF);
		buffer_variable_data[6] = (uint8_t)((data & 0xFF00)>>8);
		buffer_variable_data[7] = (uint8_t)(data & 0x00FF);
	  HAL_UART_Transmit(&huart1, buffer_variable_data, 8, 10);
}



 //TalNum为当前接收缓存的角标，会循环；CurNum表示当前正在解析的指令角标，通常从StartNum开始计算；StartNum表示该组或下组指令的开始位置
void TOUCH_extract_command(void)
{
    uint16_t i,CurNum,tem_TalNum;
    uint8_t CmdBuf[256];
    uint16_t nowbuffer,len;
    len=StartNum;
    tem_TalNum=TalNum;
    if(tem_TalNum==len)//
       return;
    if(CommBuff[StartNum]!=USER_R3)
    {
        StartNum++;
        if(StartNum==BUFFER_SIZE)
          StartNum=0;
        return;
    }
    if(tem_TalNum>len)
      nowbuffer=tem_TalNum-len;
    else
      nowbuffer=tem_TalNum+BUFFER_SIZE-len;
    if(nowbuffer<5)
      return;
    CurNum=StartNum+2;
    if(CurNum>BUFFER_SIZE-1)
      CurNum-=BUFFER_SIZE;
    len=CommBuff[CurNum]+3;
    if(nowbuffer<len)
      return;
    i=0;
    CurNum=StartNum;
    while(1)
    {
        CmdBuf[i++]=CommBuff[CurNum++];
        if(CurNum==BUFFER_SIZE)
          CurNum=0;
        if(i==4)
        {
            if(CmdBuf[0]!=USER_R3||CmdBuf[1]!=USER_RA)//
            {
                StartNum=CurNum;
                return;
            }
            len=CmdBuf[2];
        }
        else if(i>4)
        {
            if(i==len+3)//
            {
                StartNum=CurNum;
                break;
            }
            else if(i>255)//
            {
                StartNum=CurNum;
                return;
            }
            else if(CurNum==tem_TalNum)
              return;
        }        
    }
    ///现在解析指令CmdBuf保存一整条指令
    switch(CmdBuf[3])
    {
			case 0x81:
					TOUCH_deal_command(CmdBuf);
					break;
			case 0x83:
					TOUCH_deal_command(CmdBuf);
					break;
			default:////命令无效,删除
					break;
    }
    return;
}

//应当在串口终端中执行如下函数
//void USART3_IRQHandler(void)
//{
//	CommBuff[TalNum++]=USART_ReceiveData(USART3);//保存串口数据
//	if(TalNum==BUFFER_SIZE)
//		TalNum=0;                 
//}

void TOUCH_deal_command(uint8_t *p_Cmdbuf)
{
	if(system.system_status->current == 6) return;
	uint8_t i=0 ;
	//uint8_t all_len = p_Cmdbuf[2]+3; 总长度
	uint8_t data_len = p_Cmdbuf[6];
	if(data_len == 0) return;
	uint16_t command_adds = ((uint16_t)p_Cmdbuf[4] << 8) | ((uint16_t)p_Cmdbuf[5]); //指令地址
	uint16_t temp_data = 0x0000;
	user_touch_info("请求处理指令");
	if(UIaddr_manual_pwm == command_adds)
	{
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		system.output_manual = (float)(temp_data)*(MAX_SPEED/100.0);
	}
	else if(UIaddr_set_pres == command_adds)
	{//设定负压		
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		system.set_value = (float)(temp_data/10.0);
	}	
	else if(UIaddr_setting_sec == command_adds)
	{//设定时间	
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		system.time->setting_sec = (int16_t)(temp_data);
		system.time->remainder_sec = system.time->setting_sec;
	}	
	else if(UIaddr_relay_status == command_adds)
	{//继电器情况		
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		system.relay_A->status = (temp_data & 0x0002) > 1;
		system.relay_B->status = (temp_data & 0x0001);
	}	
	else if(UIaddr_auto == command_adds)
	{//自动情况		
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		if(temp_data == UIaddr_auto_run) 	system_StatusSwitch(system.system_status, 2);
		else if(temp_data == UIaddr_auto_pause) system_StatusSwitch(system.system_status, 3);
		else if(temp_data == UIaddr_auto_continue) system_StatusSwitch(system.system_status, 2);
	}	
	else if(UIaddr_manual == command_adds)
	{//手动情况		
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		if(temp_data == UIaddr_manual_run) 	system_StatusSwitch(system.system_status, 4);
		else if(temp_data == UIaddr_manual_pause) system_StatusSwitch(system.system_status, 5);
		else if(temp_data == UIaddr_manual_continue) system_StatusSwitch(system.system_status, 4);
	}
	else if(UIaddr_stop == command_adds)
	{//终止情况
		system_StatusSwitch(system.system_status, 1);
	}
	else if(UIaddr_reset == command_adds)
	{//终止情况
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		if(temp_data == 0x0001) system_MCUreset();
	}		
	else{
		for(i=0;i<data_len;i++)
		{
			temp_data = ((uint16_t)p_Cmdbuf[7+2*i] << 8) | ((uint16_t)p_Cmdbuf[8+2*i]);
			TOUCH_run_command(temp_data);
		}
	}
}

void TOUCH_run_command(uint16_t cmd)
{
//	user_main_info("接收到指令：%x",cmd);
//	switch(cmd)
//    {
//			case 0x0001:
//			{
//					if(system.sys_status!=4)system_into_stop();
//					break;
//			}
//			case 0x0002:
//					if(system.sys_status!=4 && system.sys_status!=6)system_into_manual();
//					else if(system.sys_status==6)	system_back(1);
//					break;
//			case 0x0003:
//				if(system.sys_status!=4 && system.sys_status!=3)system_into_warning();
//				else if(system.sys_status==3)	system_back(1);
//				break;
//			default:////命令无效,删除
//					break;
//    }
    return;
}
/*
0001：急停
0002：手动模式
*/
