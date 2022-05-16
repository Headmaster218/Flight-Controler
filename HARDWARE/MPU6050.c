#include <MPU6050.h>

u8 MPU_Init(void)
{ 
	u16 data ,lpf;
	I2C1_Soft_Single_Write(MPU_ADDR,MPU_PWR_MGMT1_REG,0X80);	//复位MPU6050
	delay_ms(200);
	I2C1_Soft_Single_Write(MPU_ADDR, MPU_PWR_MGMT1_REG,0X00);	//唤醒MPU6050 
	I2C1_Soft_Single_Write(MPU_ADDR, MPU_GYRO_CFG_REG,3<<3);;					//陀螺仪传感器,±2000dps
	I2C1_Soft_Single_Write(MPU_ADDR, MPU_GYRO_CFG_REG,0<<3);;					//加速度传感器,±2g
	lpf = I2C1_Soft_Single_Write(MPU_ADDR, MPU_SAMPLE_RATE_REG, 1000/50-1);//设置采样率为4 < 50Hz <1000
	I2C1_Soft_Single_Write(MPU_ADDR, MPU_INT_EN_REG,0X00);	//关闭所有中断
	I2C1_Soft_Single_Write(MPU_ADDR, MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
	I2C1_Soft_Single_Write(MPU_ADDR, MPU_FIFO_EN_REG,0X00);	//关闭FIFO
	I2C1_Soft_Single_Write(MPU_ADDR, MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	if(I2C1_Soft_Single_Read(MPU_ADDR,MPU_DEVICE_ID_REG) == MPU_ADDR)//器件ID正确
	{
		I2C1_Soft_Single_Write(MPU_ADDR, MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
		I2C1_Soft_Single_Write(MPU_ADDR, MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
		if(lpf>=188)data=1;
		else if(lpf>=98)data=2;
		else if(lpf>=42)data=3;
		else if(lpf>=20)data=4;
		else if(lpf>=10)data=5;
		else data=6; 
		return I2C1_Soft_Single_Write(MPU_ADDR, MPU_CFG_REG,data);
 	}
	return 1;
	
}

//得到温度值
//返回值:温度值(扩大了100倍)
short MPU_Get_Temperature(void)
{
    u8 buf[2]; 
    short raw;
		float temp;
		I2C1_Soft_Mult_Read(MPU_ADDR, MPU_TEMP_OUTH_REG,buf,2); 
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;;
}
//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6],res;  
	res=!I2C1_Soft_Mult_Read(MPU_ADDR,MPU_GYRO_XOUTH_REG,buf,6);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=!I2C1_Soft_Mult_Read(MPU_ADDR,MPU_ACCEL_XOUTH_REG,buf,6);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}