#include "hp5806.h"

struct HP5806 hp5806_A, hp5806_B;	//创建外置与板载传感器


/*****************************************************************************
 * 功  能：传感器测量温度及气压
 * 参  数：id: 传感器序列号，1为板载，2为外置
 * 			hp5806 需要操作的传感器
 * 返回值：NULL
 *****************************************************************************/
void HP5806_run(struct HP5806* hp5806)
{
	if(hp5806->id==1)
	{
		user_hp5806_info("开始测量板载HP5806温度");
		HP5806_GetTemperature(&hi2c1, hp5806);
		user_hp5806_info("开始测量板载HP5806气压");
		HP5806_GetPressure(&hi2c1, hp5806);
	}
	else if(hp5806->id==2)
	{
		user_hp5806_info("开始测量外置HP5806温度");
		HP5806_GetTemperature(&hi2c2, hp5806);
		user_hp5806_info("开始测量外置HP5806气压");
		HP5806_GetPressure(&hi2c2, hp5806);
	}
}


/*****************************************************************************
 * 功  能：初始化传感器
 * 参  数：hi2c:	需要操作的IIC通道
 *      		hp5806:	需要操作的传感器
 * 返回值：Void
 *****************************************************************************/
void HP5806_Init(I2C_HandleTypeDef *hi2c, struct HP5806 *hp5806, uint8_t id)
{
	if (HP5806_Check(hi2c))
	{
		user_hp5806_error("HP5806初始化失败");
		return;
	}
	HAL_Delay(5);
	uint8_t source = 0;
	// HP5806_Reset(hi2c);
	// HAL_Delay(100);
	HAL_Delay(5);
	HP5806_GetCoefficient(hi2c, hp5806);
	HAL_Delay(5);
	HP5806_Pconf(hi2c, hp5806, HP5806_PM_RATE_1 | HP5806_PM_PRC_16);
	HAL_Delay(5);
	HP5806_ReadByte(hi2c, HP5806_COED_SRCE, &source);
	source = (source & 0x80);
	if (source == HP5806_TMP_COEF_SRCE_TBD)
		HP5806_Tconf(hi2c, hp5806, HP5806_TMP_EXT_INTERNAL | HP5806_TMP_RATE_1 | HP5806_TMP_PRC_1);
	else if (source == HP5806_TMP_COEF_SRCE_MEMS)
		HP5806_Tconf(hi2c, hp5806, HP5806_TMP_EXT_EXTERNAL | HP5806_TMP_RATE_1 | HP5806_TMP_PRC_1);
	HAL_Delay(5);
	HP5806_Shift(hi2c, hp5806, HP5806_T_SHIFT_NO | HP5806_P_SHIFT_YES | HP5806_FIFO_EN_NO);
	HAL_Delay(5);
	hp5806->id = id;
	user_hp5806_info("HP5806初始化完成");
}

/*****************************************************************************
 * 功  能：获取气压值
 * 参  数：hi2c:	需要操作的IIC通道
 *      		hp5806:	需要操作的传感器
 * 返回值：0成功 1失败
 *****************************************************************************/
uint8_t HP5806_GetPressure(I2C_HandleTypeDef *hi2c, struct HP5806 *hp5806)
{
	uint8_t data = HP5806_MEAS_CTRL_COMMAND_P_MEASUREMENT;
	uint8_t value[3];
	if (HP5806_WriteByte(hi2c, HP5806_MEAS_CFG, &data) == 0)
	{
		HAL_Delay(30); //温度转换时间，需要配合过采样率设置
		if (HP5806_ReadMultBytes(hi2c, HP5806_PSR_B2, 3, value) == 0)
		{
			hp5806->Praw = ((((uint32_t)value[0]) << 16) | (((uint32_t)value[1]) << 8) | ((uint32_t)value[2]));
			if (hp5806->Praw & 0x800000)
			{
				hp5806->Praw |= 0xff000000;
			}
			hp5806->Prawsc = (float)hp5806->Praw / (float)HP5806_kP;
			hp5806->Pcomp = hp5806->c00 + hp5806->Prawsc * (hp5806->c10 + hp5806->Prawsc * (hp5806->c20 + hp5806->Prawsc * hp5806->c30)) + hp5806->Trawsc * hp5806->c01 + hp5806->Trawsc * hp5806->Prawsc * (hp5806->c11 + hp5806->Prawsc * hp5806->c21);
			user_hp5806_info("压力测量结果为：%.2fhPa", hp5806->Pcomp / 100);
			return 0;
		}
	}
	user_hp5806_error("压力测量失败，当前数值不可靠");
	return 1;
}

/*****************************************************************************
 * 功  能：获取温度值
 * 参  数：hi2c:	需要操作的IIC通道
 *      		hp5806:	需要操作的传感器
 * 返回值：0成功 1失败
 *****************************************************************************/
uint8_t HP5806_GetTemperature(I2C_HandleTypeDef *hi2c, struct HP5806 *hp5806)
{
	uint8_t data = HP5806_MEAS_CTRL_COMMAND_T_MEASUREMENT;
	uint8_t value[3];
	if (HP5806_WriteByte(hi2c, HP5806_MEAS_CFG, &data) == 0)
	{
		HAL_Delay(5); //温度转换时间，需要配合过采样率设置
		if (HP5806_ReadMultBytes(hi2c, HP5806_TMP_B2, 3, value) == 0)
		{
			hp5806->Traw = ((((uint32_t)value[0]) << 16) | (((uint32_t)value[1]) << 8) | ((uint32_t)value[2]));
			if (hp5806->Traw & 0x800000)
			{
				hp5806->Traw |= 0xff000000;
			}
			hp5806->Trawsc = (float)hp5806->Traw / (float)HP5806_kT;
			hp5806->Tcomp = (double)(hp5806->c0 * 0.5) + (hp5806->c1 * hp5806->Trawsc);
			user_hp5806_info("温度测量结果为：%.2f℃", hp5806->Tcomp);
			return 0;
		}
	}
	user_hp5806_error("温度测量失败，当前数值(含压力)不可靠");
	return 1;
}

/*****************************************************************************
 * 功  能：软件复位
 * 参  数：hi2c:	需要操作的IIC通道
 * 返回值：0成功 1失败
 *****************************************************************************/
uint8_t HP5806_Reset(I2C_HandleTypeDef *hi2c)
{
	uint8_t data = (HP5806_FIFO_FLUSH_CLEAR | HP5806_SORT_RST);
	if (HP5806_WriteByte(hi2c, HP5806_RESET, &data) == 0)
	{
		user_hp5806_info("软件复位成功");
		return 0;
	}
	else
	{
		user_hp5806_error("软件复位失败");
		return 1;
	}
}

/*****************************************************************************
 * 功  能：SHIFT配置
 * 参  数：hi2c:	需要操作的IIC通道
 *      		hp5806:	需要操作的传感器
 *         buf:  需要配置的值
 * 返回值：0成功 1失败
 *****************************************************************************/
uint8_t HP5806_Shift(I2C_HandleTypeDef *hi2c, struct HP5806 *hp5806, uint8_t buf)
{
	uint8_t data = buf;
	user_hp5806_info("尝试数值移位（SHIFT）配置，值为0x%X", data);
	if (HP5806_WriteByte(hi2c, HP5806_CFG_REG, &data) == 0)
	{
		hp5806->CFG_REG = buf;
		user_hp5806_info("数值移位（SHIFT）配置已成功");
		return 0;
	}
	else
	{
		user_hp5806_error("数值移位（SHIFT）配置失败");
		return 1;
	}
}

/*****************************************************************************
 * 功  能：温度测量配置
 * 参  数：hi2c:	需要操作的IIC通道
 *      		hp5806:	需要操作的传感器
 *         buf:  需要配置的值
 * 返回值：0成功 1失败
 *****************************************************************************/
uint8_t HP5806_Tconf(I2C_HandleTypeDef *hi2c, struct HP5806 *hp5806, uint8_t buf)
{
	uint8_t data = buf;
	user_hp5806_info("尝试温度测量配置，值为0x%X", data);
	if (HP5806_WriteByte(hi2c, HP5806_TMP_CFG, &data) == 0)
	{
		hp5806->TMP_CFG = buf;
		user_hp5806_info("温度测量配置已成功");
		return 0;
	}
	else
	{
		user_hp5806_error("温度测量配置失败");
		return 1;
	}
}

/*****************************************************************************
 * 功  能：压力测量配置
 * 参  数：hi2c:	需要操作的IIC通道
 *      		hp5806:	需要操作的传感器
 *         buf:  需要配置的值
 * 返回值：0成功 1失败
 *****************************************************************************/
uint8_t HP5806_Pconf(I2C_HandleTypeDef *hi2c, struct HP5806 *hp5806, uint8_t buf)
{
	uint8_t data = buf;
	user_hp5806_info("尝试压力测量配置，值为0x%X", data);
	if (HP5806_WriteByte(hi2c, HP5806_PRS_CFG, &data) == 0)
	{
		hp5806->PRS_CFG = buf;
		user_hp5806_info("压力测量配置已成功");
		return 0;
	}
	else
	{
		user_hp5806_error("压力测量配置失败");
		return 1;
	}
}

/*****************************************************************************
 * 功  能：获取传感器校准系数
 * 参  数：hi2c:	需要操作的IIC通道
 *      		hp5806:	需要操作的传感器
 *         buf:  读取数据存放的地址
 * 返回值：0成功 1失败
 *****************************************************************************/
uint8_t HP5806_GetCoefficient(I2C_HandleTypeDef *hi2c, struct HP5806 *hp5806)
{
	uint8_t buf[18];
	if (HP5806_ReadMultBytes(hi2c, HP5806_COEF_c0, 18, buf) == 0)
	{
		HP5806_CalcCoefficient(hp5806, buf);
		user_hp5806_info("HP5806校准参数已经获取");
		return 0;
	}
	else
	{
		user_hp5806_error("HP5806获取失败，系数计算无意义");
		return 1;
	}
}

/*****************************************************************************
 * 功  能：计算传感器校准系数
 * 参  数：hp5806:	需要操作的传感器
 *         buf:  存储返回系数的地址
 * 返回值：Void
 *****************************************************************************/
void HP5806_CalcCoefficient(struct HP5806 *hp5806, uint8_t *buf)
{
	hp5806->c0 = ((((uint16_t)buf[0]) << 4) | (((uint16_t)buf[1]) >> 4));
	if (hp5806->c0 & 0x800)
	{
		hp5806->c0 |= 0xf000;
	}
	hp5806->c1 = ((((uint16_t)(buf[1] & 0x0F)) << 8) | (uint16_t)buf[2]);
	if (hp5806->c1 & 0x800)
	{
		hp5806->c1 |= 0xf000;
	}
	hp5806->c00 = ((((uint32_t)buf[3]) << 12) | (((uint32_t)buf[4]) << 4) | (((uint32_t)buf[5]) >> 4));
	if (hp5806->c00 & 0x80000)
	{
		hp5806->c00 |= 0xfff00000;
	}
	hp5806->c10 = ((((uint32_t)(buf[5] & 0x0F)) << 16) | (((uint32_t)buf[6]) << 8) | ((uint32_t)buf[7]));
	if (hp5806->c10 & 0x80000)
	{
		hp5806->c10 |= 0xfff00000;
	}
	hp5806->c01 = (int16_t)((((uint16_t)buf[8]) << 8) | ((uint16_t)buf[9]));
	hp5806->c11 = (int16_t)((((uint16_t)buf[10]) << 8) | ((uint16_t)buf[11]));
	hp5806->c20 = (int16_t)((((uint16_t)buf[12]) << 8) | ((uint16_t)buf[13]));
	hp5806->c21 = (int16_t)((((uint16_t)buf[14]) << 8) | ((uint16_t)buf[15]));
	hp5806->c30 = (int16_t)((((uint16_t)buf[16]) << 8) | ((uint16_t)buf[17]));
}

/*****************************************************************************
 * 功  能：检查设备ID是否正确
 * 参  数：hi2c:	需要操作的IIC通道
 * 返回值：0成功 1失败
 *****************************************************************************/
uint8_t HP5806_Check(I2C_HandleTypeDef *hi2c)
{
	uint8_t productID = 0;
	if (HP5806_ReadByte(hi2c, HP5806_Product_ID, &productID) == 0)
	{
		if (productID == (HP5806_REV_ID | HP5806_PROD_ID))
		{
			user_hp5806_info("HP5806设备检查ID通过");
			return 0;
		}
	}
	user_hp5806_error("HP5806设备检查ID不通过");
	return 1;
}

/*****************************************************************************
 * 功  能：写一个字节数据到 HP5806 寄存器
 * 参  数：hi2c:	需要操作的IIC通道
 *					reg： 寄存器地址
 *         data: 要写入的数据
 * 返回值：0成功 1失败
 *****************************************************************************/
uint8_t HP5806_WriteByte(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *data)
{
	if (HAL_I2C_Mem_Write(hi2c, HP5806WAddr, reg, I2C_MEMADD_SIZE_8BIT, data, 1, 1000) != HAL_OK)
	{
		user_hp5806_error("IIC写单个字符失败");
		return 1;
	}
	else
	{
		return 0;
	}
}

/*****************************************************************************
 * 功  能：从指定HP5806寄存器读取一个字节数据
 * 参  数：hi2c:	需要操作的IIC通道
 * 				reg： 寄存器地址
 *         buf:  读取数据存放的地址
 * 返回值：0成功 1失败
 *****************************************************************************/
uint8_t HP5806_ReadByte(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *buf)
{
	if (HAL_I2C_Mem_Read(hi2c, HP5806RAddr, reg, I2C_MEMADD_SIZE_8BIT, buf, 1, 1000) != HAL_OK)
	{
		user_hp5806_error("IIC读单个字符失败");
		return 1;
	}
	else
		return 0;
}

/*****************************************************************************
 * 功  能：从指定寄存器写入指定长度数据
 * 参  数：hi2c:	需要操作的IIC通道
 *					reg：寄存器地址
 *         len：写入数据长度
 *         buf: 写入数据存放的地址
 * 返回值：0成功 1失败
 *****************************************************************************/
uint8_t HP5806_WriteMultBytes(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t len, uint8_t *buf)
{
	if (HAL_I2C_Mem_Write(hi2c, HP5806WAddr, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 1000) != HAL_OK)
	{
		user_hp5806_error("IIC写多个字符失败");
		return 1;
	}
	else
		return 0;
}

/*****************************************************************************
 * 功  能：从指定寄存器读取指定长度数据
 * 参  数：hi2c:	需要操作的IIC通道
 *					reg：寄存器地址
 *         len：读取数据长度
 *         buf: 读取数据存放的地址
 * 返回值：0成功 1失败
 *****************************************************************************/
uint8_t HP5806_ReadMultBytes(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t len, uint8_t *buf)
{
	if (HAL_I2C_Mem_Read(hi2c, HP5806RAddr, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 1000) != HAL_OK)
	{
		user_hp5806_error("IIC读多个字符失败");
		return 1;
	}
	else
		return 0;
}
