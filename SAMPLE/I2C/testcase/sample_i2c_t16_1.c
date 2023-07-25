#include "sys.h"
#include "delay.h"
#include "sample_i2c.h"
#include "stm32f10x.h"

#define read_ahb32(mreg)	 *(volatile unsigned int *)(mreg)
#define write_ahb32(mreg,val)   *(volatile unsigned int *)(mreg) = (val)

/*******************************************************************************
 * @brief   sample_i2c_t16
* Function  Interrupt_I2C2:I2C_TX_OVER[3]
 * @author  licuihuang
 * @date    2022-4
 * @param   void
 * @return  void
*******************************************************************************/ 
void sample_i2c_t16_1(void)
{
	u32 byte_receive_data[20] = {0x0};
	u32 byte_write_data[20] = {0x0}; //0x1234;
	u32 reg = 0,status = 0,i;

	struct snps_i2c_info i2c1_info ;
	struct snps_i2c_info i2c2_info ;
	struct i2c_cfg_param icp;  

	i2c1_info.base_address = I2C1_BASE;  //all
	i2c1_info.clk_freq = I2C_PCLK;  //set_speed
	i2c1_info.xfer_param.target_dev_addr = SLAVE_ADDRESS;  //set_addr
	i2c1_info.xfer_param.len = 17;  //控制read write个数
	i2c1_info.xfer_param.buffer =  byte_write_data; //byte_receive_data; 

	i2c2_info.base_address = I2C2_BASE;
	i2c2_info.clk_freq = I2C_PCLK;
	i2c2_info.xfer_param.target_dev_addr = SLAVE_ADDRESS;
	i2c2_info.xfer_param.len = 17;
	i2c2_info.xfer_param.buffer =  byte_receive_data; //byte_write_data;

	icp.xfer_mode = I2C_POLLING_MODE;
	icp.dev_addr_as_slave = SLAVE_ADDRESS;
	icp.speed = I2C_SPEED_MODE_STANDARD;  //I2C_FAST_SPEED;		

	snps_i2c_close(&i2c1_info);
	snps_i2c_close(&i2c2_info);	

	write_ahb32(0xE000E280,0xFFFFFFFF) ;//清除PENDING位		
	i2c2_nvic_init();	

	write_ahb32(I2C1_BASE+I2C_ENABLE, 0x0);
	write_ahb32(I2C2_BASE+I2C_ENABLE, 0x0);
	snps_i2c_config(&i2c2_info, &icp);    //i2c1做主机  传输模式/启主禁从/速度/TAR  
	snps_i2c_slave_config(&i2c1_info, &icp);  //i2c2做从机

	write_ahb32(i2c2_info.base_address +  I2C_INTR_MASK,0xFFFF);  //i2c使能前后写都有效	
	status = read_ahb32(i2c2_info.base_address +  I2C_INTR_MASK);
	write_ahb32(i2c2_info.base_address +  I2C_INTR_MASK,status & I2C_TX_OVER);             //[3]：I2C_TX_OVER,1 

	for(i=0x1;i<=0x11;i++)
	{ 		
		byte_write_data[i-1] = i;
	}

	snps_i2c_write_1(&i2c2_info);  
	write_ahb32(i2c2_info.base_address +  I2C_DATA_CMD,0x99 );  //[3]:TX_OVER 1 需在I2C_DATA_CMD写入别的命令
	delay_ms(2);
	printf("i2c2 write: I2C_INTR_STAT:0x%x\r\n",read_ahb32(i2c2_info.base_address +  I2C_INTR_STAT));					

	printf("finish\r\n\r\n");			
	snps_i2c_close(&i2c1_info);
	snps_i2c_close(&i2c2_info);	
}	  

	