#include "key.h"

uint32_t Time_Scan = 0;
int8_t KEY_Dir1 = 0;
uint8_t last_key1 = 1;
uint8_t last_last_key1 = 1;
uint32_t number1 = 0;

void Scan_The_Key(void)//add this function into a 10ms TIM
{
	++Time_Scan;//add the time,10ms every time
////////////////////////////////////////////////////////////////////
	if(!(last_last_key1 || last_key1))//dir1 +1
	{
		if(GPIO_Read_Key_1())
		{
			KEY_Dir1 = -1;
			last_key1 = 1;
		}
		else if(Time_Scan - number1 > 60)
		{
			KEY_Dir1 = 1;	
			last_key1 = 1;
		}
	}
	else
	{
		if(!GPIO_Read_Key_1())
		{
			last_last_key1 = last_key1;
			last_key1 = 0;
			number1 = Time_Scan;
		}
		else
		{
			last_key1 = 1;
		}
	}
////////////////////////////////////////////////////////////////////////
}

unsigned char Function_For_Key()
{
	if(KEY_Dir1 == -1 )
	{
		user_key_info("KEY_STOP is pressed(short)");
		return 1;
	}
	else if(KEY_Dir1 == 1)
	{
		user_key_info("KEY_STOP is pressed(long)");
		return 2;
	}
	else return 0;
}
