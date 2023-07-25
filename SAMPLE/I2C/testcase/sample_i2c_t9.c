#include "sys.h"
#include "delay.h"
#include "sample_i2c.h"
#include "stm32f10x.h"

#define read_ahb32(mreg)	 *(volatile unsigned int *)(mreg)
#define write_ahb32(mreg,val)   *(volatile unsigned int *)(mreg) = (val)

/*******************************************************************************
 * @brief   sample_i2c_t9
* Function  bulk transfer:i2c1 as a receiver(addr:7 bit)
 * @author  licuihuang
 * @date    2022-3
 * @param   void
 * @return  void
*******************************************************************************/ 
void sample_i2c_t9(void)
{
	u32 byte_receive_data[5] ;
	u32 byte_write_data[5] = {0x11,0x12,0x13,0x14,0x15}; //0x1234;

	struct snps_i2c_info i2c1_info ;
	struct snps_i2c_info i2c2_info ;
	struct i2c_cfg_param icp;  

	i2c1_info.base_address = I2C1_BASE;  //all
	i2c1_info.clk_freq = I2C_PCLK;  //set_speed
	i2c1_info.xfer_param.target_dev_addr = SLAVE_ADDRESS;  //set_addr
	i2c1_info.xfer_param.len = 5;  //����read write����
	i2c1_info.xfer_param.buffer = byte_receive_data;  //&byte_write_data; 

	i2c2_info.base_address = I2C2_BASE;
	i2c2_info.clk_freq = I2C_PCLK;
	i2c2_info.xfer_param.target_dev_addr = SLAVE_ADDRESS;
	i2c2_info.xfer_param.len = 5;
	i2c2_info.xfer_param.buffer = byte_write_data; //&byte_receive_data;

	icp.xfer_mode = I2C_POLLING_MODE;
	icp.dev_addr_as_slave = SLAVE_ADDRESS;
	icp.speed = I2C_SPEED_MODE_STANDARD;  

	snps_i2c_close(&i2c1_info);
	snps_i2c_close(&i2c2_info);
	write_ahb32(I2C1_BASE+I2C_ENABLE, 0x0);
	write_ahb32(I2C2_BASE+I2C_ENABLE, 0x0);
	
	snps_i2c_config(&i2c1_info, &icp);        //i2c1������  ����ģʽ/��������/�ٶ�/TAR
	snps_i2c_slave_config(&i2c2_info, &icp);  //i2c2���ӻ�
	printf("TAR: 0x%x ,SAR : 0x%x \r\n",read_ahb32(i2c1_info.base_address +  I2C_TAR),read_ahb32(i2c2_info.base_address +  I2C_SAR));

	printf("I2C2_RAW_INTR_STAT:0x%x\r\n",read_ahb32(i2c2_info.base_address + I2C_RAW_INTR_STAT));		
	write_ahb32(i2c1_info.base_address + I2C_DATA_CMD, I2C_CMD);	//i2c1��

	delay_ms(5);
	printf("read-I2C2_RAW_INTR_STAT:0x%x\r\n",read_ahb32(i2c2_info.base_address + I2C_RAW_INTR_STAT)); //����status����ֱ�Ӷ��Ĵ��������ݸ��²���ʱ

	while((read_ahb32(i2c2_info.base_address + I2C_RAW_INTR_STAT) & I2C_RD_REQ) || (read_ahb32(i2c2_info.base_address + I2C_RAW_INTR_STAT) & I2C_TX_ABRT))
	{
		read_ahb32(i2c2_info.base_address + I2C_CLR_RD_REQ);
		read_ahb32(i2c2_info.base_address + I2C_CLR_TX_ARBT);
		printf("clear-I2C2_RAW_INTR_STAT:0x%x\r\n",read_ahb32(i2c2_info.base_address + I2C_RAW_INTR_STAT));  //�����ж�
	}
	snps_i2c_write_1(&i2c2_info);    //i2c2д
	printf("transter finish\r\n");

	delay_ms(1);                                                 //��һ�����ݾ�����I2C_DATA_CMD�Ĵ�������дһ����ָ��
	write_ahb32(i2c1_info.base_address + I2C_DATA_CMD, I2C_CMD );//| I2C_STOP);//�����Ƿ���������ֹͣ�����������ڲ�stop�������sclֱ���յ�д����ʱ����ֹͣ�������γɿ�ʼ����
	write_ahb32(i2c1_info.base_address + I2C_DATA_CMD, I2C_CMD );
//		delay_ms(5);
//		printf("clear-I2C2_RAW_INTR_STAT:0x%x\r\n",read_ahb32(i2c2_info.base_address + I2C_RAW_INTR_STAT));		//���жϺ��ڴ����ֽ������жϲ����ٴ�����
	write_ahb32(i2c1_info.base_address + I2C_DATA_CMD, I2C_CMD );
	write_ahb32(i2c1_info.base_address + I2C_DATA_CMD, I2C_CMD | I2C_STOP);//��ֹͣ������������һ�εĴ����ˣ�����ֹͣ����˵�������ű��εģ�Ч��������
//		delay_ms(5);
//		printf("clear-I2C2_RAW_INTR_STAT:0x%x\r\n",read_ahb32(i2c2_info.base_address + I2C_RAW_INTR_STAT));		//���жϺ��ڴ����ֽ������жϲ����ٴ�����
//		
//		write_ahb32(i2c1_info.base_address + I2C_DATA_CMD, I2C_CMD );
//		delay_ms(5);
//		printf("clear-I2C2_RAW_INTR_STAT:0x%x\r\n",read_ahb32(i2c2_info.base_address + I2C_RAW_INTR_STAT));  //�ڴ����ֽ�����I2C_CLR_RD_REQ��I2C_CLR_TX_ARBT�ٴ�����


	printf("I2C1_DATA_CMD:0x%x  ",read_ahb32(i2c1_info.base_address +  I2C_DATA_CMD));
	delay_ms(1);
	printf("0x%x ",read_ahb32(i2c1_info.base_address +  I2C_DATA_CMD));
	delay_ms(1);
	printf("0x%x ",read_ahb32(i2c1_info.base_address +  I2C_DATA_CMD));
	delay_ms(1);
	printf("0x%x ",read_ahb32(i2c1_info.base_address +  I2C_DATA_CMD));
	delay_ms(1);
	printf("0x%x \r\n",read_ahb32(i2c1_info.base_address +  I2C_DATA_CMD));

	printf("\r\n");
	snps_i2c_close(&i2c1_info);
	snps_i2c_close(&i2c2_info);
}


