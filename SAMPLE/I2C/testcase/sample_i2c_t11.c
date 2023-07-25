#include "sys.h"
#include "delay.h"
#include "sample_i2c.h"
#include "stm32f10x.h"

#define read_ahb32(mreg)	 *(volatile unsigned int *)(mreg)
#define write_ahb32(mreg,val)   *(volatile unsigned int *)(mreg) = (val)
	
/*******************************************************************************
 * @brief   sample_i2c_t11
* Function  bus reset:SDA Stuck Timeout
 * @author  licuihuang
 * @date    2022-3
 * @param   void
 * @return  void
*******************************************************************************/
void sample_i2c_t11(void)
{
	u32 byte_receive_data[2] = {0x71,0x11};//0x28;
	u32 byte_write_data[2] = {0x01};//{0x78,0x12}; 
	u32 i,reg = 0,rcg = 0;
	
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

	write_ahb32(i2c1_info.base_address + I2C_SDA_STUCK_AT_LOW_TIMEOUT,0x177);  //配置SDA Stuck Timeout 0x4a1

	write_ahb32(I2C1_BASE+I2C_ENABLE, 0x0);
	write_ahb32(I2C2_BASE+I2C_ENABLE, 0x0);	
	snps_i2c_config(&i2c1_info, &icp);        //i2c1做主机  传输模式/启主禁从/速度/TAR/总线清除功能
	snps_i2c_slave_config(&i2c2_info, &icp);  //i2c2做从机
 
    delay_ms(3);
	printf("I2C1_CON:0x%x\r\n",read_ahb32(i2c1_info.base_address + I2C_CON));  
    printf("I2C_SCL_STUCK_AT_LOW_TIMEOUT:0x%x  ",read_ahb32(i2c1_info.base_address + I2C_SCL_STUCK_AT_LOW_TIMEOUT)); 
	printf("I2C_SDA_STUCK_AT_LOW_TIMEOUT:0x%x  \r\n",read_ahb32(i2c1_info.base_address + I2C_SDA_STUCK_AT_LOW_TIMEOUT));  
	
    write_ahb32(i2c1_info.base_address + I2C_DATA_CMD, I2C_CMD );    //I2C1读
    write_ahb32(i2c1_info.base_address + I2C_DATA_CMD, I2C_CMD | I2C_STOP); 

	delay_ms(2);  //时间需够		
	snps_i2c_write_1(&i2c2_info);   //I2C2写

    delay_ms(1);
	if(read_ahb32(i2c1_info.base_address + I2C_TX_ABRT_SOURCE) & 0x20000)    //[17]:SDA_STUCK_AT_LOW
	{
		printf("i2c1 is enter SDA_STUCK_AT_LOW\r\n");
	}
	else
	{
		printf("i2c1 is not enter SDA_STUCK_AT_LOW,I2C_TX_ABRT_SOURCE:0x%x\r\n",read_ahb32(i2c1_info.base_address + I2C_TX_ABRT_SOURCE)); 
	}		
	
	 reg = read_ahb32(i2c1_info.base_address + I2C_ENABLE);
	 write_ahb32(i2c1_info.base_address + I2C_ENABLE, reg | 0x08);   //[3]:SDA_STUCK_RECOVERY_ENABLE（1）,auto clear
	 while(read_ahb32(i2c1_info.base_address + I2C_ENABLE)&0x08);    //等待I2C_ENABLE[3]为0
	 printf("I2C1_ENABLE:0x%x  ",read_ahb32(i2c1_info.base_address + I2C_ENABLE)); 	
		
	read_ahb32(i2c1_info.base_address + I2C_CLR_TX_ARBT);  
	read_ahb32(i2c1_info.base_address + I2C_CLR_INTR);      //清除中断
	read_ahb32(i2c1_info.base_address + I2C_CLR_SCL_STUCK_DET);
	 
    delay_ms(5000);
//	while(!(read_ahb32(i2c1_info.base_address + I2C_STATUS)&0x800));                    //等待I2C_STATUS[11]为0
	printf("I2C1_STATUS:0x%x\r\n",read_ahb32(i2c1_info.base_address + I2C_STATUS)); 	//[11]:	SDA_STUCK IS RECOVER(0)
    printf("I2C1_DATA_CMD:0x%x  ",read_ahb32(i2c1_info.base_address +  I2C_DATA_CMD));	
	printf("0x%x\r\n  ",read_ahb32(i2c1_info.base_address +  I2C_DATA_CMD));	
		
	printf("\r\n");
	snps_i2c_close(&i2c1_info);
    snps_i2c_close(&i2c2_info);	  
}		
		
		