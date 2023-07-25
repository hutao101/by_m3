#include "sys.h"
#include "delay.h"
#include "sample_i2c.h"
#include "stm32f10x.h"

#define read_ahb32(mreg)	 *(volatile unsigned int *)(mreg)
#define write_ahb32(mreg,val)   *(volatile unsigned int *)(mreg) = (val)

/*******************************************************************************
 * @brief   sample_i2c_t14
 * Function SDA hold time when i2c1 as a receiver
 * @author  licuihuang
 * @date    2022-4
 * @param   void
 * @return  void
*******************************************************************************/ 
void sample_i2c_t14(void)
{
	u32 byte_receive_data = 0x28;
	u32 byte_write_data[2] = {0x28,0X12}; //0x1234;
	u32 reg = 0,status = 0,i;

	struct snps_i2c_info i2c1_info ;
	struct snps_i2c_info i2c2_info ;
	struct i2c_cfg_param icp;  

	i2c1_info.base_address = I2C1_BASE;  //all
	i2c1_info.clk_freq = I2C_PCLK;  //set_speed
	i2c1_info.xfer_param.target_dev_addr = SLAVE_ADDRESS;  //set_addr
	i2c1_info.xfer_param.len = 1;  //控制read write个数
	i2c1_info.xfer_param.buffer =  byte_write_data; //byte_receive_data; 

	i2c2_info.base_address = I2C2_BASE;
	i2c2_info.clk_freq = I2C_PCLK;
	i2c2_info.xfer_param.target_dev_addr = SLAVE_ADDRESS;
	i2c2_info.xfer_param.len = 1;
	i2c2_info.xfer_param.buffer =  &byte_receive_data; //byte_write_data;

	icp.xfer_mode = I2C_POLLING_MODE;
	icp.dev_addr_as_slave = SLAVE_ADDRESS;
	icp.speed = I2C_SPEED_MODE_STANDARD;  //I2C_FAST_SPEED;

	snps_i2c_disable(&i2c1_info);
	snps_i2c_disable(&i2c2_info);

	//   for(i=0x37;i<0x3F;i++)       //测试hold time极限值
	{
		//	write_ahb32(i2c2_info.base_address +  I2C_SDA_HOLD,0x700000);        //I2C_SDA_HOLD[23:8]
		write_ahb32(i2c2_info.base_address +  I2C_SDA_HOLD,0x70);                //I2C_SDA_HOLD[7:0]
	write_ahb32(I2C1_BASE+I2C_ENABLE, 0x0);
	write_ahb32(I2C2_BASE+I2C_ENABLE, 0x0);
	snps_i2c_config(&i2c1_info, &icp);        //i2c1做主机  传输模式/启主禁从/速度/TAR
	snps_i2c_slave_config(&i2c2_info, &icp);  //i2c2做从机

	delay_ms(2);
	printf("I2C_SDA_HOLD:0x%x  0x%x\r\n",read_ahb32(i2c1_info.base_address +  I2C_SDA_HOLD),read_ahb32(i2c2_info.base_address +  I2C_SDA_HOLD));

	write_ahb32(i2c1_info.base_address + I2C_DATA_CMD, I2C_CMD | I2C_STOP );//I2C（主从）活动	
	delay_ms(2); 
	printf("I2C_RD_REQ:0x%x  0x%x\r\n",read_ahb32(i2c1_info.base_address +  I2C_RAW_INTR_STAT),read_ahb32(i2c2_info.base_address +  I2C_RAW_INTR_STAT));

	snps_i2c_write(&i2c2_info);	
	delay_ms(2); 
	printf("I2C1_DATA_CMD:0x%x\r\n  ",read_ahb32(i2c1_info.base_address +  I2C_DATA_CMD));	

	printf("\r\n");
	}
	snps_i2c_close(&i2c1_info);
	snps_i2c_close(&i2c2_info);		
}
		
		
		