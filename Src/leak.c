#include "leak.h"

/*
������Ҫ����й©���⣬�������Ƕ������⣻Ŀǰ�ڳ����ܵ�û�а�װ�����������������ݲ������
й©��Ϊ���֣�
1.��ȫй©����ȫ���������ܵ���ѹǿҲֻͣ���ڴ���ѹˮƽ�����ǵ�������������Ը�10s-30s�Ĺ۲�ʱ�䣻
2.С��й©�����ڽ���ԭ���ʹ��ڿף����ǿ׶���Խ�С�����Ծֲ�й©�ļ����ڻ���Ҫ������
����12����һ����Ҫָ�꣬����PWM��������PWM����100ʱ��ʼ��ʱ����PWMС��100�򲻼�ʱ�رռ�ʱ���������¼10�Σ�10�β����������򴥷�й©����������ܴ������������
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
			else system_into_warning();
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