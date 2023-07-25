#include "sys.h"
#include "delay.h"
#include "sample_i2c.h"
#include "stm32f10x.h"

#define read_ahb32(mreg)	 *(volatile unsigned int *)(mreg)
#define write_ahb32(mreg,val)   *(volatile unsigned int *)(mreg) = (val)

/*******************************************************************************
 * @brief   sample_i2c_t6
* Function  Slave Mode-i2c1 as a receiver(addr:7 bit)
 * @author  licuihuang
 * @date    2022-3
 * @param   void
 * @return  void
*******************************************************************************/ 
void sample_i2c_t6(void)
{
	u8 ret;
	u32 byte_receive_data ;
	u32 byte_write_data = 0x58;
	u32 status1,status2;

	struct snps_i2c_info i2c1_info ;
	struct snps_i2c_info i2c2_info ;
	struct i2c_cfg_param icp;

	i2c1_info.base_address = I2C1_BASE;  //all
	i2c1_info.clk_freq = I2C_PCLK;  //set_speed
	i2c1_info.xfer_param.target_dev_addr = SLAVE_ADDRESS;  //set_addr
	i2c1_info.xfer_param.len = 1;  //控制read write个数
	i2c1_info.xfer_param.buffer = &byte_receive_data;  //&byte_write_data; 

	i2c2_info.base_address = I2C2_BASE;
	i2c2_info.clk_freq = I2C_PCLK;
	i2c2_info.xfer_param.target_dev_addr = SLAVE_ADDRESS;
	i2c2_info.xfer_param.len = 1;
	i2c2_info.xfer_param.buffer = &byte_write_data; //&byte_receive_data;

	icp.xfer_mode = I2C_POLLING_MODE;
	icp.dev_addr_as_slave = SLAVE_ADDRESS;
	icp.speed = I2C_SPEED_MODE_STANDARD;  

	snps_i2c_close(&i2c1_info);
	snps_i2c_close(&i2c2_info);
	write_ahb32(I2C1_BASE+I2C_ENABLE, 0x0);
	write_ahb32(I2C2_BASE+I2C_ENABLE, 0x0);

	snps_i2c_config(&i2c2_info, &icp);        //i2c2做主机
	snps_i2c_slave_config(&i2c1_info, &icp);  //i2c1做从机

	printf("i2c2_TAR: 0x%x ,i2c1_SAR : 0x%x \r\n",read_ahb32(i2c2_info.base_address +  I2C_TAR),read_ahb32(i2c1_info.base_address +  I2C_SAR));
	delay_ms(1);
	write_ahb32(i2c2_info.base_address + I2C_DATA_CMD, 0x55) ; //i2c2发送数据
	delay_ms(1);
	printf("I2C1_CMD:0x%x\r\n",read_ahb32(i2c1_info.base_address + I2C_DATA_CMD));

	snps_i2c_close(&i2c1_info);
	snps_i2c_close(&i2c2_info);
}


