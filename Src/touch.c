#include "touch.h"
#include "fuzzyPID.h"

extern struct _system MCUsystem;
extern uint8_t uart2_isbusy;
extern struct PID** pid_vector;
extern int control_id;
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
		TOUCH_UpdataStatus();
//		TOUCH_UpdataPID();
//		TOUCH_UpdataPres();

//		user_touch_info("UI更新完毕");
}


void TOUCH_safeEvent_Qt(uint16_t event){
	TOUCH_variable_write2(UIaddr_warring, (uint16_t)(event));
}

void TOUCH_UpdataHP5806(void)
{//仪表盘
		TOUCH_variable_write(UIaddr_hp5806_B_pres, (uint16_t)(MCUsystem.hp5806_B->Pcomp/10));
			TOUCH_variable_write2(UIaddr_hp5806_B_pres, (uint16_t)(MCUsystem.hp5806_B->Pcomp/10));
		TOUCH_variable_write(UIaddr_hp5806_A_pres, (uint16_t)(MCUsystem.hp5806_A->Pcomp/10));
			TOUCH_variable_write2(UIaddr_hp5806_A_pres, (uint16_t)(MCUsystem.hp5806_A->Pcomp/10));
		TOUCH_variable_write(UIaddr_hp5806_B_temp, (int16_t)(MCUsystem.hp5806_B->Tcomp*10));
			TOUCH_variable_write2(UIaddr_hp5806_B_temp, (int16_t)(MCUsystem.hp5806_B->Tcomp*10));
		TOUCH_variable_write(UIaddr_hp5806_A_temp, (int16_t)(MCUsystem.hp5806_A->Tcomp*10));
			TOUCH_variable_write2(UIaddr_hp5806_A_temp, (int16_t)(MCUsystem.hp5806_A->Tcomp*10));
		TOUCH_variable_write(UIaddr_hp5806_B_pres_ptr, (uint16_t)(1200-MCUsystem.hp5806_B->Pcomp/100));
		TOUCH_variable_write(UIaddr_hp5806_B_temp_ptr, (uint16_t)(15+MCUsystem.hp5806_B->Tcomp));
	
		user_touch_info("HP5806更新完毕");
}

void TOUCH_UpdataPres(void)
{//负压情况
		uint16_t temp = (uint16_t)(MCUsystem.hp5806_A->Pcomp/10-MCUsystem.hp5806_B->Pcomp/10);
		TOUCH_variable_write(UIaddr_set_pres, (uint16_t)(MCUsystem.set_value*10));
			TOUCH_variable_write2(UIaddr_set_pres, (uint16_t)(MCUsystem.set_value*10));
		TOUCH_variable_write(UIaddr_current_pres, temp);
			TOUCH_variable_write2(UIaddr_current_pres, temp);
		user_touch_info("负压情况更新完毕");
}

void TOUCH_UpdataRelay(void)
{//继电器情况
		uint16_t temp = (uint16_t)(((MCUsystem.relay_A->status & 0xFF) << 1) | (MCUsystem.relay_B->status & 0xFF) );
		TOUCH_variable_write(UIaddr_relay_status, temp);
			TOUCH_variable_write2(UIaddr_relay_status, temp);
		user_touch_info("负压情况更新完毕");
}

void TOUCH_UpdataTime(void)
{//时间情况
	TOUCH_variable_write(UIaddr_hour, MCUsystem.time->hour);
		TOUCH_variable_write2(UIaddr_hour, MCUsystem.time->hour);
	TOUCH_variable_write(UIaddr_min, MCUsystem.time->min);
		TOUCH_variable_write2(UIaddr_min, MCUsystem.time->min);
	TOUCH_variable_write(UIaddr_sec, MCUsystem.time->sec);
		TOUCH_variable_write2(UIaddr_sec, MCUsystem.time->sec);
	TOUCH_variable_write(UIaddr_setting_sec, MCUsystem.time->setting_sec);
		TOUCH_variable_write2(UIaddr_setting_sec, MCUsystem.time->setting_sec);
	TOUCH_variable_write(UIaddr_remainder_sec, MCUsystem.time->remainder_sec);
		TOUCH_variable_write2(UIaddr_remainder_sec, MCUsystem.time->remainder_sec);
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
		data = (uint16_t)(MCUsystem.hp5806_B->Pcomp/100);
		TOUCH_change_data(buffer_curve_data, 5, data);
		data = (uint16_t)(MCUsystem.hp5806_A->Pcomp/100);
		TOUCH_change_data(buffer_curve_data, 7, data);
		data = (uint16_t)(MCUsystem.hp5806_A->Pcomp/100-MCUsystem.set_value);
		TOUCH_change_data(buffer_curve_data, 9, data);
	//温度曲线
		data = (uint16_t)(15+MCUsystem.hp5806_B->Tcomp);
		TOUCH_change_data(buffer_curve_data, 11, data);
		data = (uint16_t)(15+MCUsystem.hp5806_A->Tcomp);
		TOUCH_change_data(buffer_curve_data, 13, data);
	//误差曲线
		data = (uint16_t)(MCUsystem.set_value);
		TOUCH_change_data(buffer_curve_data, 15, data);
		data = (uint16_t)(MCUsystem.hp5806_A->Pcomp/100-MCUsystem.hp5806_B->Pcomp/100);
		TOUCH_change_data(buffer_curve_data, 17, data);
		
	HAL_UART_Transmit(&huart1, buffer_curve_data, 19, 10);
}



void TOUCH_UpdataStatus(void)
{//状态更新（服务于远程），配合上位机开发
		uint16_t temp = (uint16_t)(MCUsystem.system_status->current);
		TOUCH_variable_write2(0x003D, temp);
	//更新输出速度
		temp = (uint16_t)(MCUsystem.output_value*10);
		TOUCH_variable_write2(0x013D, temp);
	//更新手动速度
		temp = (uint16_t)(MCUsystem.output_manual/4);
		TOUCH_variable_write2(UIaddr_manual_pwm, temp);
		TOUCH_variable_write(UIaddr_manual_pwm, temp);
}

void TOUCH_UpdataPump(void)
{
		TOUCH_variable_write(UIaddr_pump_status, UIaddr_pump_off);
//		TOUCH_variable_write(UIaddr_pump_freq, (uint16_t)(MCUsystem.pump->frequency/100));
//		TOUCH_variable_write(UIaddr_pump_pwm, (uint16_t)(MCUsystem.pump->pwm*100));
		user_touch_info("PUMP更新完毕");
}

void TOUCH_UpdataPID(void)
{
		TOUCH_variable_write(UIaddr_pid_p, (uint16_t)(MCUsystem.pid->Kp*100));
		TOUCH_variable_write(UIaddr_pid_i, (uint16_t)(MCUsystem.pid->Ki*100));
		TOUCH_variable_write(UIaddr_pid_d, (uint16_t)(MCUsystem.pid->Kd*100));
		TOUCH_variable_write(UIaddr_pid_mode, (uint16_t)MCUsystem.pid_mode);
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

void TOUCH_variable_write2(uint16_t adds, uint16_t data)
{
		buffer_variable_data[3] = variable_write;
		buffer_variable_data[4] = (uint8_t)((adds & 0xFF00)>>8);
		buffer_variable_data[5] = (uint8_t)(adds & 0x00FF);
		buffer_variable_data[6] = (uint8_t)((data & 0xFF00)>>8);
		buffer_variable_data[7] = (uint8_t)(data & 0x00FF);
		while(uart2_isbusy == 1);
		uart2_isbusy=1;
		HAL_UART_Transmit_DMA(&huart2,buffer_variable_data,8);
		HAL_Delay(2);
}


void TOUCH_change_page(uint16_t page)
{
	uint8_t buffer_registor_data[7] = {USER_R3, USER_RA, 0x04, registor_write, 0x03, 0x00, 0x00};
	buffer_registor_data[5] = (uint8_t)((page & 0xFF00)>>8);
	buffer_registor_data[6] = (uint8_t)(page & 0x00FF);
	HAL_UART_Transmit(&huart1, buffer_registor_data, 7, 10);
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
	if(MCUsystem.system_status->current == 6) return;
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
		MCUsystem.output_manual = (float)(temp_data)*(MAX_SPEED/100.0);
	}
	else if(UIaddr_set_pres == command_adds)
	{//设定负压		
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		MCUsystem.set_value = (float)(temp_data/10.0);
	}	
	else if(UIaddr_setting_sec == command_adds)
	{//设定时间	
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		MCUsystem.time->setting_sec = (int16_t)(temp_data);
		MCUsystem.time->remainder_sec = MCUsystem.time->setting_sec;
	}	
	else if(UIaddr_relay_status == command_adds)
	{//继电器情况		
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		MCUsystem.relay_A->status = (temp_data & 0x0002) > 1;
		MCUsystem.relay_B->status = (temp_data & 0x0001);
	}	
	else if(UIaddr_auto == command_adds)
	{//自动情况		
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		if(temp_data == UIaddr_auto_run) 	system_StatusSwitch(MCUsystem.system_status, 2);
		else if(temp_data == UIaddr_auto_pause) system_StatusSwitch(MCUsystem.system_status, 3);
		else if(temp_data == UIaddr_auto_continue) system_StatusSwitch(MCUsystem.system_status, 2);
		else if(UIaddr_qt_continue == temp_data){
			system_StatusSwitch(MCUsystem.system_status, 2);
			change_page(2);
		}
		else if(UIaddr_qt_pause == temp_data){
			system_StatusSwitch(MCUsystem.system_status, 3);
			change_page(3);
		}
		else if(UIaddr_qt_run == temp_data){
			system_StatusSwitch(MCUsystem.system_status, 2);
			change_page(2);
		}
	}	
	else if(UIaddr_manual == command_adds)
	{//手动情况		
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		if(temp_data == UIaddr_manual_run) 	system_StatusSwitch(MCUsystem.system_status, 4);
		else if(temp_data == UIaddr_manual_pause) system_StatusSwitch(MCUsystem.system_status, 5);
		else if(temp_data == UIaddr_manual_continue) system_StatusSwitch(MCUsystem.system_status, 4);
		else if(UIaddr_qt_continue == temp_data){
			system_StatusSwitch(MCUsystem.system_status, 4);
			change_page(4);
		}
		else if(UIaddr_qt_pause == temp_data){
			system_StatusSwitch(MCUsystem.system_status, 5);
			change_page(5);
		}
		else if(UIaddr_qt_run == temp_data){
			system_StatusSwitch(MCUsystem.system_status, 4);
			change_page(4);
		}
	}
	else if(UIaddr_stop == command_adds)
	{//终止情况
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		system_StatusSwitch(MCUsystem.system_status, 1);
		if(temp_data == 0x0101)change_page(1);
	}
	else if(UIaddr_reset == command_adds)
	{//复位情况
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		if(temp_data == 0x0001) system_MCUreset();
	}
	else if(UIaddr_warring == command_adds)
	{//警告知晓消除警告
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		if(temp_data == 0x0001) system_know_warring();
		else if(temp_data == 0x0101) {system_know_warring();}
	}
	else if(UIaddr_pid_mode == command_adds)
	{//pid_mode
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		if(temp_data == 0x0001) MCUsystem.pid_mode = 1;
		else if(temp_data == 0x0000) MCUsystem.pid_mode = 0;
	}

	else if(UIaddr_pid_p == command_adds)
	{//pid_kp
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		MCUsystem.pid->Kp = ((int16_t)temp_data)/1000.0f;
		pid_vector[control_id]->kp = MCUsystem.pid->Kp;
		pid_vector[control_id]->delta_kp_max = pid_vector[control_id]->kp/MCUsystem.delta_k;
	}	

	else if(UIaddr_pid_i == command_adds)
	{//pid_ki
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		MCUsystem.pid->Ki = ((int16_t)temp_data)/1000.0f;
		pid_vector[control_id]->ki = MCUsystem.pid->Ki;
		pid_vector[control_id]->delta_ki_max = pid_vector[control_id]->ki/MCUsystem.delta_k;
	}	

	else if(UIaddr_pid_d == command_adds)
	{//pid_kd
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		MCUsystem.pid->Kd = ((int16_t)temp_data)/1000.0f;
		pid_vector[control_id]->kd = MCUsystem.pid->Kd;
		pid_vector[control_id]->delta_kd_max = pid_vector[control_id]->kd/MCUsystem.delta_k;
	}
	else if(UIaddr_pid_delta_k == command_adds)
	{//pid_delta_k
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		MCUsystem.delta_k = ((int16_t)temp_data)/1000.0f;
		pid_vector[control_id]->delta_kp_max = pid_vector[control_id]->kp/MCUsystem.delta_k;
		pid_vector[control_id]->delta_ki_max = pid_vector[control_id]->ki/MCUsystem.delta_k;
		pid_vector[control_id]->delta_kd_max = pid_vector[control_id]->kd/MCUsystem.delta_k;
	}	
	else if(UIaddr_pid_updata == command_adds)
	{//pid_delta_k
		temp_data = ((uint16_t)p_Cmdbuf[7] << 8) | ((uint16_t)p_Cmdbuf[8]);
		if(temp_data == 0x0001){
			TOUCH_variable_write2(UIaddr_pid_mode, (uint16_t)(MCUsystem.pid_mode));
			TOUCH_variable_write2(UIaddr_pid_p, (uint16_t)(pid_vector[control_id]->kp*1000));
			TOUCH_variable_write2(UIaddr_pid_i, (uint16_t)(pid_vector[control_id]->ki*1000));
			TOUCH_variable_write2(UIaddr_pid_d, (uint16_t)(pid_vector[control_id]->kd*1000));
			TOUCH_variable_write2(UIaddr_pid_delta_k, (uint16_t)(MCUsystem.delta_k*1000));
		}
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
//					if(MCUsystem.sys_status!=4)system_into_stop();
//					break;
//			}
//			case 0x0002:
//					if(MCUsystem.sys_status!=4 && MCUsystem.sys_status!=6)system_into_manual();
//					else if(MCUsystem.sys_status==6)	system_back(1);
//					break;
//			case 0x0003:
//				if(MCUsystem.sys_status!=4 && MCUsystem.sys_status!=3)system_into_warning();
//				else if(MCUsystem.sys_status==3)	system_back(1);
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

void TOUCH_pidData(void){
		if(MCUsystem.pid_mode==0)printf("PID Mode:PID\n");
		else if(MCUsystem.pid_mode==1)printf("PID Mode:fuzzyPID\n");
		printf("PID Kp:%.3f\n",pid_vector[control_id]->kp);
		printf("PID Ki:%.3f\n",pid_vector[control_id]->ki);
		printf("PID Kd:%.3f\n",pid_vector[control_id]->kd);
		printf("PID Delta:%.3f\n",MCUsystem.delta_k);
		printf("PID Delta Kp Max:%.3f\n",pid_vector[control_id]->delta_kp_max);
		printf("PID Delta Ki Max:%.3f\n",pid_vector[control_id]->delta_ki_max);
		printf("PID Delta Kd Max:%.3f\n",pid_vector[control_id]->delta_kd_max);
}
