#include "sys.h"
#include "delay.h"
#include "sample_i2c.h"
#include "stm32f10x.h"

#define read_ahb32(mreg)	 *(volatile unsigned int *)(mreg)
#define write_ahb32(mreg,val)   *(volatile unsigned int *)(mreg) = (val)

/*******************************************************************************
 * @brief   sample_i2c_t21
* Function  I2C1_remap���Ų���
 * @author  licuihuang
 * @date    2022-9-24
 * @param   void
 * @return  void
*******************************************************************************/ 
void sample_i2c_t21(void)
{		
	u32 ret,i;
	u32 byte_receive_data ;
	u32 byte_write_data = 0x12;
	u32 status1,status2;
	
	struct snps_i2c_info i2c1_info ;
	struct snps_i2c_info i2c2_info ;
	struct i2c_cfg_param icp; 
	
	printf("pin mux:0x%x ",read_ahb32(0x4001b018));
 	printf("remap:0x%x ",read_ahb32(0x4001b070));  
    write_ahb32(0x4001b070,0x2);     //pin remap
	write_ahb32(0x4001b06c,0x0);     //tim pin
	printf("remap:0x%x\r\n",read_ahb32(0x4001b070));	
	printf("tim pin:0x%x\r\n",read_ahb32(0x4001b06c));
	printf("0x%x 0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE),read_ahb32(I2C2_BASE+I2C_ENABLE)); 
	
	i2c1_info.base_address = I2C1_BASE;  //all
	i2c1_info.clk_freq = I2C_PCLK;  //set_speed
	i2c1_info.xfer_param.target_dev_addr = SLAVE_ADDRESS;  //set_addr
	i2c1_info.xfer_param.len = 1;  //����read write����
	i2c1_info.xfer_param.buffer = &byte_write_data;  //&byte_write_data; 

	i2c2_info.base_address = I2C2_BASE;
	i2c2_info.clk_freq = I2C_PCLK;
	i2c2_info.xfer_param.target_dev_addr = SLAVE_ADDRESS;
	i2c2_info.xfer_param.len = 1;
	i2c2_info.xfer_param.buffer = &byte_receive_data; //&byte_receive_data;
	
	icp.xfer_mode = I2C_POLLING_MODE;
	icp.dev_addr_as_slave = SLAVE_ADDRESS;
	icp.speed = I2C_SPEED_MODE_STANDARD;  
	printf("0x%x 0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE),read_ahb32(I2C2_BASE+I2C_ENABLE));
	
	snps_i2c_close(&i2c1_info);
	snps_i2c_close(&i2c2_info);
	write_ahb32(I2C1_BASE+I2C_ENABLE, 0x0);
	write_ahb32(I2C2_BASE+I2C_ENABLE, 0x0);
	printf("I2C_ENABLE:0x%x\r\n",read_ahb32(I2C1_BASE+I2C_ENABLE));

/*********i2c1 to i2c2********/ 
	snps_i2c_config(&i2c1_info, &icp);    //start i2c  ����ģʽ/��������/�ٶ�/TAR
	snps_i2c_slave_config(&i2c2_info, &icp);  //start i2c
	printf("TAR: 0x%x ,SAR : 0x%x \r\n",read_ahb32(i2c1_info.base_address +  I2C_TAR),read_ahb32(i2c2_info.base_address +  I2C_SAR));

	write_ahb32(I2C1_BASE+I2C_DATA_CMD, 0x12);
	printf("transter finish\r\n");
	delay_ms(2);
	printf("0x%x  \r\n\r\n",read_ahb32(i2c2_info.base_address +  I2C_DATA_CMD));
/*********i2c1 to i2c2********/ 

/*********i2c2 to i2c1********/   
//	snps_i2c_config(&i2c2_info, &icp);    //start i2c  ����ģʽ/��������/�ٶ�/TAR
//	snps_i2c_slave_config(&i2c1_info, &icp);  //start i2c
//	printf("TAR: 0x%x ,SAR : 0x%x \r\n",read_ahb32(i2c2_info.base_address +  I2C_TAR),read_ahb32(i2c1_info.base_address +  I2C_SAR));

//	write_ahb32(I2C2_BASE+I2C_DATA_CMD, 0x13);
//	printf("transter finish\r\n");
//	delay_ms(2);
//	printf("0x%x  \r\n\r\n",read_ahb32(i2c1_info.base_address +  I2C_DATA_CMD));
/*********i2c2 to i2c1********/

	snps_i2c_close(&i2c1_info);
    snps_i2c_close(&i2c2_info);
}

