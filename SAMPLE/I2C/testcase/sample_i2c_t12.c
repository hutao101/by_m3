#include "sys.h"
#include "delay.h"
#include "sample_i2c.h"
#include "stm32f10x.h"

#define read_ahb32(mreg)	 *(volatile unsigned int *)(mreg)
#define write_ahb32(mreg,val)   *(volatile unsigned int *)(mreg) = (val)

/*******************************************************************************
 * @brief   sample_i2c_t12
 * Function bus reset-SCL Stuck Timeout
 * @author  licuihuang
 * @date    2022-3
 * @param   void
 * @return  void
*******************************************************************************/    
void sample_i2c_t12(void)
{
	u32 byte_receive_data [2] = {0x01,0x01};//0x28;
	u32 byte_write_data [2] = {0x01};//{0x78,0x12}; //0x1234;
	u32 reg = 0,i = 0;

	struct snps_i2c_info i2c1_info ;
	struct snps_i2c_info i2c2_info ;
	struct i2c_cfg_param icp;  

	i2c1_info.base_address = I2C1_BASE;  //all
	i2c1_info.clk_freq = I2C_PCLK;  //set_speed
	i2c1_info.xfer_param.target_dev_addr = SLAVE_ADDRESS_10;  //set_addr
	i2c1_info.xfer_param.len = 2;  //控制read write个数
	i2c1_info.xfer_param.buffer =  byte_write_data; //byte_receive_data; 

	i2c2_info.base_address = I2C2_BASE;
	i2c2_info.clk_freq = I2C_PCLK;
	i2c2_info.xfer_param.target_dev_addr = SLAVE_ADDRESS_10;
	i2c2_info.xfer_param.len = 2;
	i2c2_info.xfer_param.buffer =  byte_receive_data; //byte_write_data;

	icp.xfer_mode = I2C_POLLING_MODE;
	icp.dev_addr_as_slave = SLAVE_ADDRESS_10;
	icp.speed = I2C_SPEED_MODE_STANDARD;  //I2C_FAST_SPEED;

	snps_i2c_disable(&i2c1_info);
	snps_i2c_disable(&i2c2_info);	
	write_ahb32(i2c1_info.base_address + I2C_SCL_STUCK_AT_LOW_TIMEOUT, 0x35);

	write_ahb32(I2C1_BASE+I2C_ENABLE, 0x0);
	write_ahb32(I2C2_BASE+I2C_ENABLE, 0x0);
	snps_i2c_config(&i2c1_info, &icp);        //i2c1做主机  传输模式/启主禁从/速度/TAR/总线清除功能
	snps_i2c_slave_config(&i2c2_info, &icp);  //i2c2做从机

	printf("I2C1_CON:0x%x\r\n",read_ahb32(i2c1_info.base_address + I2C_CON)); 
	printf("I2C_SCL_STUCK_AT_LOW_TIMEOUT:0x%x  ",read_ahb32(i2c1_info.base_address + I2C_SCL_STUCK_AT_LOW_TIMEOUT)); 
	printf("I2C_SDA_STUCK_AT_LOW_TIMEOUT:0x%x  \r\n",read_ahb32(i2c1_info.base_address + I2C_SDA_STUCK_AT_LOW_TIMEOUT)); 

	write_ahb32(i2c1_info.base_address + I2C_DATA_CMD, I2C_CMD );              //I2C1读
	write_ahb32(i2c1_info.base_address + I2C_DATA_CMD, I2C_CMD | I2C_STOP);	   //读行为都是连一起的，但最后一个写读的时间决定了读所有数据的时间	
	delay_ms(2);  //时间需够		
	snps_i2c_write(&i2c2_info);		//I2C2写
	
	delay_ms(1);
	if(read_ahb32(i2c1_info.base_address + I2C_RAW_INTR_STAT) & 0x4000)
	{
		printf("i2c1 is enter SCL_STUCK_AT_LOW\r\n");
	}
	else
	{
		printf("i2c1 is not enter SCL_STUCK_AT_LOW,I2C_RAW_INTR_STAT:0x%x\r\n",read_ahb32(i2c1_info.base_address + I2C_RAW_INTR_STAT)); 
	}
	printf("I2C1_DATA_CMD:0x%x  ",read_ahb32(i2c1_info.base_address +  I2C_DATA_CMD));	
	printf("0x%x\r\n  ",read_ahb32(i2c1_info.base_address +  I2C_DATA_CMD));	

	read_ahb32(i2c1_info.base_address + I2C_CLR_INTR);
	read_ahb32(i2c1_info.base_address + I2C_CLR_SCL_STUCK_DET);

	printf("\r\n");
	snps_i2c_close(&i2c1_info);
	snps_i2c_close(&i2c2_info);				                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
}
		
		
		