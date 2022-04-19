#include "leak.h"

/*
进气主要考虑泄漏问题，出气考虑堵塞问题；目前在出气管道没有安装传感器，堵塞问题暂不解决；
泄漏分为两种：
1.完全泄漏，泵全力工作，管道内压强也只停留在大气压水平，考虑到箱体情况，可以给10s-30s的观察时间；
2.小孔泄漏，由于进样原本就存在孔，但是孔洞相对较小，所以局部泄漏的检测后期还需要调整，
场景12都有一个重要指标，就是PWM开满，当PWM开满100时开始计时，当PWM小于100则不计时关闭计时，开满会记录10次，10次波动都不大则触发泄漏，如果波动很大，则清空重来。
*/

uint8_t leak_timer_start_flag = 0;
float leak_value[10] = {0};
uint8_t leak_current = 0;
extern uint8_t event_flag;

void LEAK_cheack(void){
	if (leak_timer_start_flag  && MCUsystem.system_status->current == 2){
		leak_value[leak_current++] = (MCUsystem.hp5806_B->Pcomp/100.0);
		if(leak_current==1) return;
		else if(leak_current==10) 
		{
			leak_current=0;
			float temp1 = MCUsystem.hp5806_A->Pcomp/100.0 - leak_value[9]-MCUsystem.set_value;
			if(temp1> -10 && temp1 <10) return;
			else{//添加泄漏事件
				if((event_flag&0x10)  == 0){
					SafeEvent Event;
					Event.priority = 2;
					Event.data = 0x05;
					HEAP_push(Event, safe_event_pq);
					event_flag = event_flag|0x10;
					user_heap_info("event_flag:%x", event_flag);
				}
			}
		}
		else{
			float temp2 = leak_value[leak_current-2] - leak_value[leak_current-1];
			if(temp2 < -10 || temp2 >10) 
			{
				leak_current=0;
				return;
			}
		}
	}
	else{
		leak_current=0;
	}
}

/*
警告应当分为：
1.提示型：只改变工作灯，弹窗，并不影响其他执行，警告期间应该亮黄灯，关闭后恢复正常
	温度高低，管道泄漏，设备初始化或测量失败
2.暂停型: 改变工作灯，弹窗，暂停设备，关闭弹窗后，依旧保持暂停，需手动解除
	气压高低
3.终止型：改变工作灯，弹窗，暂时设备，恢复后初始化
4.无法操作型：改变工作灯，弹窗，（不）暂停设备，恢复后初始化-其实就是emm上位机设置，建议单独搞吧

目前来看，没有办法弹窗，决定都作为页面跳转，然后记录上一页，返回后返回到上一页即可；
缺点就是多警告同时触发如何处理问题；
警告采用队列触发，检测忙，按照优先级队列








*/
