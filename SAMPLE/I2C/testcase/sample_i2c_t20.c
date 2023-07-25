#include "sys.h"
#include "delay.h"
#include "sample_i2c.h"
#include "stm32f10x.h"

#define read_ahb32(mreg)	 *(volatile unsigned int *)(mreg)
#define write_ahb32(mreg,val)   *(volatile unsigned int *)(mreg) = (val)

/*******************************************************************************
 * @brief   sample_i2c_t20
* Function  Interrupt_I2C1:I2C_MST_ON_HOLD(fail)
 * @author  licuihuang
 * @date    2022-3
 * @param   void
 * @return  void
*******************************************************************************/ 
void sample_i2c_t20(void)
{
	u8 ret,status;
	u32 byte_receive_data ;
	u32 byte_write_data = 0x34;

	struct snps_i2c_info i2c1_info ;
	struct snps_i2c_info i2c2_info ;
	struct i2c_cfg_param icp;

	i2c1_info.base_address = I2C1_BASE;  //all
	i2c1_info.clk_freq = I2C_PCLK;  //set_speed
	i2c1_info.xfer_param.target_dev_addr = SLAVE_ADDRESS_10;  //set_addr
	i2c1_info.xfer_param.len = 1;  //控制read write个数
	i2c1_info.xfer_param.buffer = &byte_write_data;  //read write

	i2c2_info.base_address = I2C2_BASE;
	i2c2_info.clk_freq = I2C_PCLK;
	i2c2_info.xfer_param.target_dev_addr = SLAVE_ADDRESS_10;
	i2c2_info.xfer_param.len = 1;
	i2c2_info.xfer_param.buffer = &byte_receive_data;

	icp.xfer_mode = I2C_POLLING_MODE;
	icp.dev_addr_as_slave = SLAVE_ADDRESS_10;
	icp.speed = I2C_SPEED_MODE_STANDARD;  

	snps_i2c_close(&i2c1_info);
	snps_i2c_close(&i2c2_info);	

	write_ahb32(0xE000E280,0xFFFFFFFF) ;//清除PENDING位		
	i2c1_nvic_init();	
	
	write_ahb32(I2C1_BASE+I2C_ENABLE, 0x0);
	write_ahb32(I2C2_BASE+I2C_ENABLE, 0x0);
	snps_i2c_config(&i2c1_info, &icp);    //start i2c
	snps_i2c_slave_config(&i2c2_info, &icp);  //start i2c

	write_ahb32(i2c1_info.base_address +  I2C_INTR_MASK,0xFFFF);  //i2c使能前后写都有效	
	status = read_ahb32(i2c1_info.base_address +  I2C_INTR_MASK);
	write_ahb32(i2c1_info.base_address +  I2C_INTR_MASK,status & I2C_MST_ON_HOLD);    //[13]：I2C_MST_ON_HOLD
	
	snps_i2c_write_1(&i2c1_info);		 //I2C（主从）活动	
	
	delay_ms(10);
	write_ahb32(i2c1_info.base_address +  I2C_DATA_CMD,0x56|I2C_RESTART );	
	delay_ms(2);
		
	printf("I2C_RAW_INTR_STAT:0x%x  0x%x\r\n",read_ahb32(i2c1_info.base_address +  I2C_RAW_INTR_STAT),read_ahb32(i2c2_info.base_address +  I2C_RAW_INTR_STAT));	
	printf("I2C_CON:0x%x  I2C_TAR:0x%x\r\n",read_ahb32(i2c1_info.base_address +  I2C_CON),read_ahb32(i2c1_info.base_address +  I2C_TAR));	
	printf("I2C_DATA_CMD:0x%x  0x%x\r\n",read_ahb32(i2c1_info.base_address +  I2C_DATA_CMD),read_ahb32(i2c2_info.base_address +  I2C_DATA_CMD));	
	printf("\r\n");
	snps_i2c_close(&i2c1_info);
	snps_i2c_close(&i2c2_info);
}

