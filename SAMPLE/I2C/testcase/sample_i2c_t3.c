#include "sys.h"
#include "delay.h"
#include "sample_i2c.h"
#include "stm32f10x.h"

#define read_ahb32(mreg)	 *(volatile unsigned int *)(mreg)
#define write_ahb32(mreg,val)   *(volatile unsigned int *)(mreg) = (val)

/*******************************************************************************
 * @brief   sample_i2c_t3
* Function  Master Mode-i2c1 as a transmitter(addr:10 bit)
 * @author  licuihuang
 * @date    2022-3
 * @param   void
 * @return  void
*******************************************************************************/ 
void sample_i2c_t3(void)
{
    u8 ret;
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
	
	printf("0x%x 0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE),read_ahb32(I2C2_BASE+I2C_ENABLE));
	write_ahb32(I2C1_BASE+I2C_ENABLE, 0x0);   //必须写这句
	write_ahb32(I2C2_BASE+I2C_ENABLE, 0x0);
	printf("I2C_ENABLE:0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE));
	
	snps_i2c_config(&i2c1_info, &icp);    //start i2c
	snps_i2c_slave_config(&i2c2_info, &icp);  //start i2c

//	write_ahb32(I2C1_BASE+I2C_DATA_CMD, 0x12);
    snps_i2c_write_1(&i2c1_info);	
	if (ret == 0) 
		printf("transter finish\r\n");
	else
		printf("transter fail\r\n");
	
	printf("i2c1_TAR: 0x%x ,i2c2_SAR : 0x%x \r\n",read_ahb32(i2c1_info.base_address +  I2C_TAR),read_ahb32(i2c2_info.base_address +  I2C_SAR));	
	byte_receive_data = read_ahb32(i2c2_info.base_address +  I2C_DATA_CMD) & 0xff;
	if( byte_write_data == byte_receive_data)
		printf("receiver finish, write data is 0x%x ,receive_data is 0x%x \r\n",byte_write_data,byte_receive_data);
	else 
		printf("receiver error, write data is 0x%x ,receive_data is 0x%x \r\n",byte_write_data,byte_receive_data);
	
	snps_i2c_close(&i2c1_info);
	snps_i2c_close(&i2c2_info);
}

