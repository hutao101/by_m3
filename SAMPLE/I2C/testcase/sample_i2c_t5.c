#include "sys.h"
#include "delay.h"
#include "sample_i2c.h"
#include "stm32f10x.h"

#define read_ahb32(mreg)	 *(volatile unsigned int *)(mreg)
#define write_ahb32(mreg,val)   *(volatile unsigned int *)(mreg) = (val)

/*******************************************************************************
 * @brief   sample_i2c_t5
* Function  Slave Mode-i2c1 as a transmitter(addr:7 bit)
 * @author  licuihuang
 * @date    2022-3
 * @param   void
 * @return  void
*******************************************************************************/ 
void sample_i2c_t5(void)
{
    u8 ret;
	u32 byte_receive_data = 0x13;
	u32 byte_write_data = 0x12;

	struct snps_i2c_info i2c1_info ;
	struct snps_i2c_info i2c2_info ;
	struct i2c_cfg_param icp;   

	printf("0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE));
	i2c1_info.base_address = I2C1_BASE;  //all
	i2c1_info.clk_freq = I2C_PCLK;  //set_speed
	i2c1_info.xfer_param.target_dev_addr = SLAVE_ADDRESS;  //set_addr
	i2c1_info.xfer_param.len = 1;  //控制read write个数
	i2c1_info.xfer_param.buffer = &byte_write_data;  //read write

	i2c2_info.base_address = I2C2_BASE;
	i2c2_info.clk_freq = I2C_PCLK;
	i2c2_info.xfer_param.target_dev_addr = SLAVE_ADDRESS;
	i2c2_info.xfer_param.len = 1;
	i2c2_info.xfer_param.buffer = &byte_receive_data;

	icp.xfer_mode = I2C_POLLING_MODE;
	icp.dev_addr_as_slave = SLAVE_ADDRESS;
	icp.speed = I2C_SPEED_MODE_STANDARD;  		

	snps_i2c_close(&i2c1_info);
	snps_i2c_close(&i2c2_info);
	write_ahb32(I2C1_BASE+I2C_ENABLE, 0x0);
	write_ahb32(I2C2_BASE+I2C_ENABLE, 0x0);

	snps_i2c_config(&i2c2_info, &icp);        //传输模式/启主禁从/速度/TAR
	snps_i2c_slave_config(&i2c1_info, &icp);  //i2c2做主机 i2c1做从机

	printf("TAR: 0x%x ,SAR : 0x%x \r\n",read_ahb32(i2c2_info.base_address +  I2C_TAR),read_ahb32(i2c1_info.base_address +  I2C_SAR));
	write_ahb32(i2c2_info.base_address + I2C_DATA_CMD, I2C_CMD);	//i2c1请求读数据

	delay_ms(3);
	printf("I2C1_RAW_INTR_STAT:0x%x\r\n",read_ahb32(i2c1_info.base_address + I2C_RAW_INTR_STAT)); //判断i2c1读请求标志是否起来
	while((read_ahb32(i2c1_info.base_address + I2C_RAW_INTR_STAT) & I2C_RD_REQ) || (read_ahb32(i2c1_info.base_address + I2C_RAW_INTR_STAT) & I2C_TX_ABRT))
	{			
		read_ahb32(i2c1_info.base_address + I2C_CLR_RD_REQ);
		read_ahb32(i2c1_info.base_address + I2C_CLR_TX_ARBT);
		printf("clear-I2C_RAW_INTR_STAT:%x\r\n",read_ahb32(i2c1_info.base_address + I2C_RAW_INTR_STAT)); 
	}
	
	snps_i2c_write_1(&i2c1_info); //i2c1发送数据
	delay_ms(2);
	printf("I2C2_CMD:0x%x\r\n",read_ahb32(i2c2_info.base_address + I2C_DATA_CMD));
		
	printf("\r\n");
	snps_i2c_close(&i2c1_info);
	snps_i2c_close(&i2c2_info);
}

