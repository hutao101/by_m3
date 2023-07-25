#include "sys.h"
#include "delay.h"
#include "sample_i2c.h"
#include "stm32f10x.h"

#define read_ahb32(mreg)	 *(volatile unsigned int *)(mreg)
#define write_ahb32(mreg,val)   *(volatile unsigned int *)(mreg) = (val)

/*******************************************************************************
 * @brief   sample_i2c_t15
* Function  Interrupt_I2C1:I2C_RX_UNDER[0]/I2C_RX_FULL[2]/I2C_TX_EMPTY[4]/I2C_RD_REQ[5]/I2C_TX_ABRT[6]/
            I2C_RX_DONE[7]/I2C_ACTIVITY[8]/I2C_STOP_DET[9]/I2C_START_DET[10]/I2C_RESTART_DET[12]
            ([*]指I2C_INTR_STAT寄存器中的位数)
 * @author  licuihuang
 * @date    2022-4
 * @param   void
 * @return  void
*******************************************************************************/ 
void sample_i2c_t15(void)
{
	u32 byte_receive_data = 0x20;
	u32 byte_write_data[2] = {0x19}; //0x1234;
	u32 reg = 0,status = 0,i,irq;

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

	for(i=0;i<10;i++)
	{
		switch(i)
		{
			case 0:
				printf("test I2C_RX_UNDER irq, I2C_INTR_STAT[0]:\r\n");
				irq=I2C_RX_UNDER;
				break;
			case 1:
				printf("test I2C_RX_FULL irq, I2C_INTR_STAT[2]:\r\n");
				irq=I2C_RX_FULL;
				break;
			case 2:
				printf("test I2C_TX_EMPTY irq, I2C_INTR_STAT[4]:\r\n");
				irq=I2C_TX_EMPTY;
				break;
			case 3:
				printf("test I2C_RD_REQ irq, I2C_INTR_STAT[5]:\r\n");
				irq=I2C_RD_REQ;
				break;
			case 4:
				printf("test I2C_RX_DONE irq, I2C_INTR_STAT[7]:\r\n");
				irq=I2C_RX_DONE;
				break;
			case 5:
				printf("test I2C_ACTIVITY irq, I2C_INTR_STAT[8]:\r\n");
				irq=I2C_ACTIVITY;
				break;
			case 6:
				printf("test I2C_STOP_DET irq, I2C_INTR_STAT[9]:\r\n");
				irq=I2C_STOP_DET;
				break;
			case 7:
				printf("test I2C_START_DET irq, I2C_INTR_STAT[10]:\r\n");
				irq=I2C_START_DET;
				break;
			case 8:
				printf("test I2C_RESTART_DET irq, I2C_INTR_STAT[12]:\r\n");
				irq=I2C_RESTART_DET;
				break;
			case 9:
				printf("test I2C_TX_ABRT irq, I2C_INTR_STAT[6]:\r\n");
				irq=I2C_TX_ABRT;
				break;
			default:
				break;
			
		}
		snps_i2c_close(&i2c1_info);
		snps_i2c_close(&i2c2_info);	

		write_ahb32(0xE000E280,0xFFFFFFFF) ;//清除PENDING位		
		i2c1_nvic_init();	

		write_ahb32(I2C1_BASE+I2C_ENABLE, 0x0);
		write_ahb32(I2C2_BASE+I2C_ENABLE, 0x0);
		snps_i2c_config(&i2c2_info, &icp);    //i2c1做主机  传输模式/启主禁从/速度/TAR  //[4]:TX_EMPTY,1
		snps_i2c_slave_config(&i2c1_info, &icp);  //i2c2做从机

		write_ahb32(i2c1_info.base_address +  I2C_INTR_MASK,0xFFFF);  //i2c使能前后写都有效	
		status = read_ahb32(i2c1_info.base_address +  I2C_INTR_MASK);
		write_ahb32(i2c1_info.base_address +  I2C_INTR_MASK,status & irq);

		snps_i2c_write_1(&i2c2_info);  //[2]:RX_FULL [8]:ACTIVITY ([9]:STOP_DET) [10]:START_DET  (只能用_1函数写，不然i2c会关闭)
		read_ahb32(i2c1_info.base_address +  I2C_DATA_CMD);
		read_ahb32(i2c1_info.base_address +  I2C_DATA_CMD);//[0]:RX_UNDER 1
		write_ahb32(i2c2_info.base_address +  I2C_DATA_CMD,I2C_CMD | I2C_STOP);  //[5]:RD_REQ [9]:STOP_DET [12]:RESTART_DET
		
		if(i!=9)
		{
			while((read_ahb32(i2c1_info.base_address +  I2C_RAW_INTR_STAT) & I2C_RD_REQ) == 0);//无此条语句会产生中断[6]:TX_ABRT,		
		}
		
		if(read_ahb32(i2c1_info.base_address +  I2C_RAW_INTR_STAT) & I2C_TX_ABRT);         //接上：原因：I2C1读命令还没生效（I2C2读请求还没起来)I2C2就开始写数据了
		{
				read_ahb32(i2c1_info.base_address +  I2C_CLR_TX_ARBT);
		}

		snps_i2c_write_1(&i2c1_info);
		delay_ms(2); 			//需有延时，要不读不出数据，不影响中断
		read_ahb32(i2c2_info.base_address +  I2C_DATA_CMD);
//		printf("I2C_DATA_CMD:0x%x\r\n",read_ahb32(i2c2_info.base_address +  I2C_DATA_CMD));				

		printf("finish\r\n");			
		snps_i2c_close(&i2c1_info);
		snps_i2c_close(&i2c2_info);	
		printf("\r\n");	
	}
}		
		
		