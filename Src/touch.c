#include "touch.h"

extern struct _system system;
uint16_t StartNum=0,TalNum=0;
uint8_t CommBuff[BUFFER_SIZE];//定义指令缓冲区
uint8_t buffer_variable_data[8] = {USER_R3, USER_RA, 0x05, variable_write, 0x00, 0x00, 0x00, 0x00};
uint8_t buffer_registor_data[6] = {USER_R3, USER_RA, 0x03, variable_read, 0x00, 0x00};
uint8_t buffer_curve_data[7] = {USER_R3, USER_RA, 0x04, curve_write, 0x03,0x00, 0x00};

void TOUCH_UpdataUI(void)
{
		TOUCH_UpdataHP5806();
		TOUCH_UpdataPump();
		TOUCH_UpdataPID();
		TOUCH_UpdataPres();
		TOUCH_UpdataStatus();
		user_touch_info("UI更新完毕");
}

void TOUCH_UpdataHP5806(void)
{
		TOUCH_variable_write(UIaddr_hp5806_B_pres, (uint16_t)(system.hp5806_B->Pcomp/10));
		TOUCH_variable_write(UIaddr_hp5806_A_pres, (uint16_t)(system.hp5806_A->Pcomp/10));
		TOUCH_variable_write(UIaddr_hp5806_B_temp, (uint16_t)(system.hp5806_B->Tcomp*10));
		TOUCH_variable_write(UIaddr_hp5806_A_temp, (uint16_t)(system.hp5806_A->Tcomp*10));
		user_touch_info("HP5806更新完毕");
}

void TOUCH_UpdataPump(void)
{
		TOUCH_variable_write(UIaddr_pumo_status, (uint16_t)system.pump->status);
		TOUCH_variable_write(UIaddr_pumo_freq, (uint16_t)(system.pump->frequency/100));
		TOUCH_variable_write(UIaddr_pumo_pwm, (uint16_t)(system.pump->pwm*100));
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

void TOUCH_UpdataPres(void)
{
		uint16_t temp = (uint16_t)(system.hp5806_A->Pcomp/10-system.hp5806_B->Pcomp/10);
		TOUCH_variable_write(UIaddr_set_pres, (uint16_t)system.set_value*10);
		TOUCH_variable_write(UIaddr_current_pres, temp);
		TOUCH_curve_write(0x01,temp);
		TOUCH_curve_write(0x02,(uint16_t)system.set_value);
		user_touch_info("负压情况更新完毕");
}

void TOUCH_UpdataStatus(void)
{
		TOUCH_variable_write(UIaddr_relay_status, (uint16_t)system.relay_A->status);
		TOUCH_variable_write(UIaddr_leak_status, (uint16_t)system.leak_status);
		TOUCH_variable_write(UIaddr_wifi_status, (uint16_t)system.wifi_status);
		TOUCH_variable_write(UIaddr_system_status, (uint16_t)system.sys_status);
		user_touch_info("其他状态更新完毕");
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

void TOUCH_curve_write(uint8_t id,uint16_t data)
{
	buffer_curve_data[4] = id;
	buffer_curve_data[5] = (uint8_t)((data & 0xFF00)>>8);
	buffer_curve_data[6] = (uint8_t)(data & 0x00FF);
	HAL_UART_Transmit(&huart1, buffer_curve_data, 7, 10);
}

void TOUCH_deal_command(void)
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
                
                break;
            case 0x83:
                
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
