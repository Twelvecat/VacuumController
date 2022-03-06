#ifndef __HP5806_H__
#define __HP5806_H__

#include "main.h"
#include "i2c.h"
#include "usart.h"

#define HP5806Addr 	0x77
#define HP5806WAddr 	HP5806Addr << 1
#define HP5806RAddr 	(HP5806Addr << 1) + 1

//变换系数，修改过采样率需要配套修改此处
#define HP5806_kP	253952
#define HP5806_kT	524288

//压力数据存储r
#define HP5806_PSR_B2	0x00
#define HP5806_PSR_B1	0x01
#define HP5806_PSR_B0	0x02

//温度数据存储r
#define HP5806_TMP_B2	0x03
#define HP5806_TMP_B1	0x04
#define HP5806_TMP_B0	0x05

//测压率与分辨率配置
#define HP5806_PRS_CFG	0x06
	//PM_RATE,PM_PRC需要or使用
		//PM_RATE[6:4]rw
		#define HP5806_PM_RATE_1	0x00
		#define HP5806_PM_RATE_2	0x10
		#define HP5806_PM_RATE_4	0x20
		#define HP5806_PM_RATE_8	0x30
		#define HP5806_PM_RATE_16	0x40
		#define HP5806_PM_RATE_32 0x50
		#define HP5806_PM_RATE_64	0x60
		#define HP5806_PM_RATE_128	0x70
		//PM_PRC[3:0]rw
		#define HP5806_PM_PRC_1	0x00
		#define HP5806_PM_PRC_2	0x01
		#define HP5806_PM_PRC_4	0x02
		#define HP5806_PM_PRC_8	0x03
		#define HP5806_PM_PRC_16	0x04
		#define HP5806_PM_PRC_32	0x05
		#define HP5806_PM_PRC_64	0x06
		#define HP5806_PM_PRC_128	0x07
		//Reserved[7]
	
//测温率与分辨率配置
#define HP5806_TMP_CFG	0x07
	//TMP_EXT,TMP_RATE,TMP_PRC需要or使用
		//TMP_EXT[7]rw
		#define HP5806_TMP_EXT_INTERNAL	0x00
		#define HP5806_TMP_EXT_EXTERNAL	0x80
		//PM_RATE[6:4]rw
		#define HP5806_TMP_RATE_1	0x00
		#define HP5806_TMP_RATE_2	0x10
		#define HP5806_TMP_RATE_4	0x20
		#define HP5806_TMP_RATE_8	0x30
		#define HP5806_TMP_RATE_16	0x40
		#define HP5806_TMP_RATE_32 0x50
		#define HP5806_TMP_RATE_64	0x60
		#define HP5806_TMP_RATE_128	0x70
		//TMP_PRC[2:0]rw
		#define HP5806_TMP_PRC_1	0x00
		#define HP5806_TMP_PRC_2	0x01
		#define HP5806_TMP_PRC_4	0x02
		#define HP5806_TMP_PRC_8	0x03
		#define HP5806_TMP_PRC_16	0x04
		#define HP5806_TMP_PRC_32	0x05
		#define HP5806_TMP_PRC_64	0x06
		#define HP5806_TMP_PRC_128	0x07
		//Reserved[3]
	
//传感器工作模式与状态配置
#define HP5806_MEAS_CFG	0x08
	//COED_RDY,SENSOR_RDY,TMP_RDY,PRS_RDY,MEAS_CTRL需要or使用
		//COED_RDY[7]r
		#define HP5806_COED_RDY_NO	0x00
		#define HP5806_COED_RDY_YES	0x80
		//SENSOR_RDY[6]r
		#define HP5806_SENSOR_RDY_NO	0x00
		#define HP5806_SENSOR_RDY_YES	0x40
		//TMP_RDY[5]r
		#define HP5806_TMP_RDY_NO	0x00
		#define HP5806_TMP_RDY_YES	0x20
		//PRS_RDY[4]r
		#define HP5806_PRS_RDY_NO	0x00
		#define HP5806_PRS_RDY_YES	0x10
		//MEAS_CTRL[2:0]rw
		#define HP5806_MEAS_CTRL_STANDBY	0x00
		#define HP5806_MEAS_CTRL_COMMAND_P_MEASUREMENT	0x01
		#define HP5806_MEAS_CTRL_COMMAND_T_MEASUREMENT	0x02
		#define HP5806_MEAS_CTRL_BACKGROUND_P_CMEASUREMENT	0x05
		#define HP5806_MEAS_CTRL_BACKGROUND_T_CMEASUREMENT	0x06
		#define HP5806_MEAS_CTRL_BACKGROUND_PT_CMEASUREMENT	0x07
		//Reserved[3]

//FIFO配置
#define HP5806_CFG_REG	0x09
	//T_SHIFT,P_SHIFT,FIFO_EN需要or使用
		//T_SHIFT[3]rw
		#define HP5806_T_SHIFT_NO	0x00
		#define HP5806_T_SHIFT_YES	0x08
		//P_SHIFT[2]rw
		#define HP5806_P_SHIFT_NO	0x00
		#define HP5806_P_SHIFT_YES	0x04
		//FIFO_EN[1]rw
		#define HP5806_FIFO_EN_NO	0x00
		#define HP5806_FIFO_EN_YES	0x02
		//Reserved[7:4,0]

//FIFO状态
#define HP5806_FIFO_STS	0x0B
	//FIFO_FULL,FIFL_EMPTY需要or使用
		//FIFO_FULL[1]r
		#define HP5806_FIFO_FULL_NO	0x00
		#define HP5806_FIFO_FULL_YES	0x02
		//FIFL_EMPTY[0]r
		#define HP5806_FIFL_EMPTY_NO	0x00
		#define HP5806_FIFL_EMPTY_YES	0x01
		//Reserved[7:2]

//FIFO刷新和软件复位
#define HP5806_RESET	0x0C
	//FIFO_FLUSH,SORT_RST需要or使用
		//FIFO_FLUSH[7]w
		#define HP5806_FIFO_FLUSH_EMPTY	0x00
		#define HP5806_FIFO_FLUSH_CLEAR	0x80
		//SORT_RST[3:0]w
		#define HP5806_SORT_RST	0x09
		//Reserved[6:4]


//产品ID
#define HP5806_Product_ID	0x0D
	//REV_ID,PROD_ID需要or使用
		//REV_ID[7:4]r
		#define HP5806_REV_ID	0x10
		//PROD_ID[3:0]r
		#define HP5806_PROD_ID	0x00

//传感器校准参数r
#define HP5806_COEF_c0	0x10			//c0[11:4]
#define HP5806_COEF_c0c1	0x11		//c0[3:0]	c1[11:8]
#define HP5806_COEF_c1	0x12			//c1[7:0]
#define HP5806_COEF_c00H	0x13		//c00[19:12]
#define HP5806_COEF_c00L	0x14		//c00[11:4]
#define HP5806_COEF_c00c10	0x15	//c00[3:0]	c10[19:16]	
#define HP5806_COEF_c10H	0x16		//c10[15:8]
#define HP5806_COEF_c10L	0x17		//c10[7:0]
#define HP5806_COEF_c01H	0x18		//c01[15:8]
#define HP5806_COEF_c01L	0x19		//c01[7:0]
#define HP5806_COEF_c11H	0x1A		//c11[15:8]
#define HP5806_COEF_c11L	0x1B		//c11[7:0]
#define HP5806_COEF_c20H	0x1C		//c20[15:8]
#define HP5806_COEF_c20L	0x1D		//c20[7:0]
#define HP5806_COEF_c21H	0x1E		//c21[15:8]
#define HP5806_COEF_c21L	0x1F		//c21[7:0]
#define HP5806_COEF_c30H	0x20		//c30[15:8]
#define HP5806_COEF_c30L	0x21		//c30[7:0]

//校准系数源
#define HP5806_COED_SRCE	0x28
	//TMP_COEF_SRCE[7]r
	#define HP5806_TMP_COEF_SRCE_TBD	0x00
	#define HP5806_TMP_COEF_SRCE_MEMS	0x80
	//Reserved[6:0]


struct HP5806{
		//传感器校正系数
    int16_t c0;
		int16_t c1;
		int32_t c00;
		int32_t c10;
		int16_t c01;
		int16_t c11;
		int16_t c20;
		int16_t c21;
		int16_t c30;
		//获取值
		int32_t Traw;
		int32_t Praw;
		//计算校准输出值
		float Tcomp;
		float Pcomp;
		float	Prawsc;
		float	Trawsc;
		//配置值
		uint8_t	PRS_CFG;
		uint8_t	TMP_CFG;
		uint8_t	CFG_REG;
};             

extern struct HP5806 hp5806_A,hp5806_B;

uint8_t HP5806_WriteByte(I2C_HandleTypeDef *hi2c,uint8_t reg,uint8_t *data);
uint8_t HP5806_ReadByte(I2C_HandleTypeDef *hi2c,uint8_t reg,uint8_t *buf);
uint8_t HP5806_WriteMultBytes(I2C_HandleTypeDef *hi2c,uint8_t reg,uint8_t len,uint8_t *buf);
uint8_t HP5806_ReadMultBytes(I2C_HandleTypeDef *hi2c,uint8_t reg,uint8_t len,uint8_t *buf);

uint8_t HP5806_Check(I2C_HandleTypeDef *hi2c);
void HP5806_CalcCoefficient(struct HP5806* hp5806,uint8_t *buf);
uint8_t HP5806_GetCoefficient(I2C_HandleTypeDef *hi2c,struct HP5806* hp5806);
uint8_t HP5806_Pconf(I2C_HandleTypeDef *hi2c,struct HP5806* hp5806,uint8_t buf);
uint8_t HP5806_Tconf(I2C_HandleTypeDef *hi2c,struct HP5806* hp5806,uint8_t buf);
uint8_t HP5806_Shift(I2C_HandleTypeDef *hi2c,struct HP5806* hp5806,uint8_t buf);
uint8_t HP5806_Reset(I2C_HandleTypeDef *hi2c);
uint8_t HP5806_GetTemperature(I2C_HandleTypeDef *hi2c,struct HP5806* hp5806);
uint8_t HP5806_GetPressure(I2C_HandleTypeDef *hi2c,struct HP5806* hp5806);
void HP5806_Init(I2C_HandleTypeDef *hi2c,struct HP5806* hp5806);
void HP5806_B_Run(void);
void HP5806_A_Run(void);


#define USER_HP5806_DEBUG
#ifdef USER_HP5806_DEBUG
	#define user_hp5806_printf(format, ...) printf( format "\r\n", ##__VA_ARGS__)
	#define user_hp5806_info(format, ...) printf("[\thp5806]info:" format "\r\n", ##__VA_ARGS__)
	#define user_hp5806_debug(format, ...) printf("[\thp5806]debug:" format "\r\n", ##__VA_ARGS__)
	#define user_hp5806_error(format, ...) printf("[\thp5806]error:" format "\r\n",##__VA_ARGS__)
#else
	#define user_hp5806_printf(format, ...)
	#define user_hp5806_info(format, ...)
	#define user_hp5806_debug(format, ...)
	#define user_hp5806_error(format, ...)
#endif

#endif /* __HP5806_H__ */
