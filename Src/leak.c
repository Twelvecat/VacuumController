#include "leak.h"

/*
进气主要考虑泄漏问题，出气考虑堵塞问题；目前在出气管道没有安装传感器，堵塞问题暂不解决；
泄漏分为两种：
1.完全泄漏，泵全力工作，管道内压强也只停留在大气压水平，考虑到箱体情况，可以给10s-30s的观察时间；
2.小孔泄漏，由于进样原本就存在孔，但是孔洞相对较小，所以局部泄漏的检测后期还需要调整，
场景12都有一个重要指标，就是PWM开满，当PWM开满100时开始计时，当PWM小于100则不计时关闭计时，开满会记录10次，10次波动都不打则触发泄漏，如果波动很大，则清空重来。
*/

uint8_t leak_timer_start_flag = 0;
float leak_value[10] = {0};
uint8_t leak_current = 0;

void LEAK_cheack(void){
	if (leak_timer_start_flag){
		leak_value[leak_current++] = (system.hp5806_B->Pcomp/100.0);
		if(leak_current==1) return;
		else if(leak_current==10) 
		{
			leak_current=0;
			float temp1 = leak_value[9]-system.set_value;
			if(temp1> -10 && temp1 <10) return;
			else ;//漏气了
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
