#include "sample_i2c.h"

#define I2C_SPEED_CALCULATION_FACTOR		1000//1521
#define read_ahb32(mreg)	 *(volatile unsigned int *)(mreg)
#define write_ahb32(mreg,val)   *(volatile unsigned int *)(mreg) = (val)
	

u32 snps_i2c_isBusy(struct snps_i2c_info *i2c_info)
{
    u32 reg;
	reg = read_ahb32(i2c_info->base_address + I2C_STATUS);
	if((reg & I2C_STATUS_ACT) == 0x0)
        return 0;
    else
        return 1;
}


//u32 snps_i2c_isEnabled(struct snps_i2c_info *i2c_info)
//{
//    u32 reg;
//	reg = read_ahb32(i2c_info->base_address + I2C_ENABLE);
//	if((reg & I2C_ENABLE_0B) == I2C_ENABLE_0B)
//	  return 1;
//	else
//    	return 0;
//}
 /**********************************************************************/

void snps_i2c_enable(struct snps_i2c_info *i2c_info)
{   
	u32 reg;
  reg = read_ahb32(i2c_info->base_address + I2C_ENABLE);
	write_ahb32(i2c_info->base_address + I2C_ENABLE, reg|I2C_ENABLE_0B);
	while (1) {
		reg = read_ahb32(i2c_info->base_address +  I2C_ENABLE_STATUS);
		if ((reg & I2C_ENABLE_0B) != 0)
			break;
	}
}


/**********************************************************************/
void snps_i2c_disable(struct snps_i2c_info *i2c_info)
{
	u32 reg;
  
    while(snps_i2c_isBusy(i2c_info) != 0);
	reg = read_ahb32(i2c_info->base_address + I2C_ENABLE);
    write_ahb32(i2c_info->base_address + I2C_ENABLE, reg & (~I2C_ENABLE_0B));
	while (1) {
		reg = read_ahb32(i2c_info->base_address +  I2C_ENABLE_STATUS);
		if ((reg & I2C_ENABLE_0B) == 0)
			break;
	}
}


/*
 * snps_i2c_set_speed - Set the I2C speed mode(standard, high, fast, fast plus)
 * @i2c:     the I2C information
 * @i2c_spd:    required I2C speed mode
 *
 * Set the I2C speed mode(standard, high, fast, fast plus)
 */
void snps_i2c_set_speed(struct snps_i2c_info *i2c_info, u32 speed)
{
	u32 reg = 0;
	u32 temp = 0;

	if (speed >= I2C_MAX_SPEED)
		speed = I2C_SPEED_MODE_MAX;
	else if (speed >= I2C_FAST_PLUS_SPEED)
		speed = I2C_SPEED_MODE_FAST_PLUS;
	else if (speed >= I2C_FAST_SPEED)
		speed = I2C_SPEED_MODE_FAST;
	else
		speed = I2C_SPEED_MODE_STANDARD;

	/* to set speed cltr must be disabled */
	snps_i2c_disable(i2c_info);

	reg = (read_ahb32(i2c_info->base_address +  I2C_CON)) & (~I2C_CON_SPD_MSK);   //取反前[2:1]位为1，其他都为0，取反后[2:1]为0其他位为1，所以之前位的设置不变

	switch (speed) {
		case I2C_SPEED_MODE_MAX:
			reg |= I2C_CON_SPD_HS;
			temp = (i2c_info->clk_freq * MIN_HS_SCL_HIGHTIME)/I2C_SPEED_CALCULATION_FACTOR;
			write_ahb32(i2c_info->base_address + I2C_HS_SCL_HCNT, temp);
			temp = (i2c_info->clk_freq * MIN_HS_SCL_LOWTIME)/I2C_SPEED_CALCULATION_FACTOR;
			write_ahb32(i2c_info->base_address + I2C_HS_SCL_LCNT, temp);
			break;
		case I2C_SPEED_MODE_STANDARD:
			reg |= I2C_CON_SPD_SS;
			temp = (i2c_info->clk_freq * MIN_SS_SCL_HIGHTIME)/I2C_SPEED_CALCULATION_FACTOR;  //(50*4000)/1000=200
			write_ahb32(i2c_info->base_address + I2C_SS_SCL_HCNT, 60);//temp);
			temp = (i2c_info->clk_freq * MIN_SS_SCL_LOWTIME)/I2C_SPEED_CALCULATION_FACTOR;   //(50*4700)/1000=235
			write_ahb32(i2c_info->base_address + I2C_SS_SCL_LCNT, 60);//temp);
			break;
		case I2C_SPEED_MODE_FAST_PLUS:
			reg |= I2C_CON_SPD_FS;
			temp = (i2c_info->clk_freq * MIN_FS_PLUS_SCL_HIGHTIME)/I2C_SPEED_CALCULATION_FACTOR;
			write_ahb32(i2c_info->base_address + I2C_FS_SCL_HCNT, temp);
			temp = (i2c_info->clk_freq * MIN_FS_PLUS_SCL_LOWTIME)/I2C_SPEED_CALCULATION_FACTOR;
			write_ahb32(i2c_info->base_address + I2C_FS_SCL_LCNT, temp);
			break;
		case I2C_SPEED_MODE_FAST:
		default:
			reg |= I2C_CON_SPD_FS;
			temp = (i2c_info->clk_freq * MIN_FS_SCL_HIGHTIME)/I2C_SPEED_CALCULATION_FACTOR;
			write_ahb32(i2c_info->base_address + I2C_FS_SCL_HCNT, temp);
			temp = (i2c_info->clk_freq * MIN_FS_SCL_LOWTIME)/I2C_SPEED_CALCULATION_FACTOR;
			write_ahb32(i2c_info->base_address + I2C_FS_SCL_LCNT, temp);
			break;
		}
	write_ahb32(i2c_info->base_address + I2C_CON, reg);	
}


/*
 * snps_i2c_init - Init function
 * @i2c:     the I2C information
 * @icp:     contains I2C config parameters
 * Initialization function.
 */
u8 snps_i2c_config(struct snps_i2c_info *i2c_info, struct i2c_cfg_param *i2c_cfg)
{
	u32 reg = 0;
	i2c_info->xfer_mode = i2c_cfg->xfer_mode;

	snps_i2c_disable(i2c_info);/* disable i2c */
	
	reg = I2C_CON_SD | I2C_CON_RE | I2C_CON_MM | I2C_CON_BUS_CLEAR;  //| I2C_CON_RE | I2C_CON_SPD_SS
	write_ahb32(i2c_info->base_address + I2C_CON, reg);
	snps_i2c_set_speed(i2c_info, i2c_cfg->speed);
	snps_i2c_set_addr(i2c_info);
	snps_i2c_wait_for_bb(i2c_info);
	
	snps_i2c_enable(i2c_info);
	return 0;
}


u8 snps_i2c_config_re(struct snps_i2c_info *i2c_info, struct i2c_cfg_param *i2c_cfg)
{
	u32 reg = 0;
	i2c_info->xfer_mode = i2c_cfg->xfer_mode;

	snps_i2c_disable(i2c_info);/* disable i2c */
	
	reg = I2C_CON_SD  | I2C_CON_RE | I2C_CON_MM;  //| I2C_CON_RE | I2C_CON_SPD_SS
	write_ahb32(i2c_info->base_address + I2C_CON, reg);
	snps_i2c_set_speed(i2c_info, i2c_cfg->speed);
	snps_i2c_set_addr(i2c_info);
	snps_i2c_wait_for_bb(i2c_info);
	
	snps_i2c_enable(i2c_info);

	return 0;
}


/*
 * snps_i2c_setaddress - Setting the target slave address
 * @i2c:     the I2C information
 * @ixp:     contains xfer parameters
 *
 * Setting the target slave address.
 */
void snps_i2c_set_addr(struct snps_i2c_info *i2c_info)
{
	u32 reg = 0;

	/* disable i2c */
	snps_i2c_disable(i2c_info);

	if (i2c_info->xfer_param.target_dev_addr > 0x7f) //|| (i2c_info->xfer_param.addr_10bit_en)) 
		{
		write_ahb32(i2c_info->base_address + I2C_TAR, i2c_info->xfer_param.target_dev_addr | (0x1 << 12));
		reg = read_ahb32(i2c_info->base_address + I2C_CON);
		reg |= I2C_CON_10BITADDRMASTER;
		write_ahb32(i2c_info->base_address + I2C_CON, reg);
	} else {
		write_ahb32(i2c_info->base_address + I2C_TAR, i2c_info->xfer_param.target_dev_addr);
	}
	/* enable I2C */
//	snps_i2c_enable(i2c_info);
}


/*
 * snps_i2c_wait_for_bb - Waiting for bus busy
 * @i2c:    the I2C information
 *
 * Waiting for bus busy
 */
void snps_i2c_wait_for_bb(struct snps_i2c_info *i2c_info)
{
	u32 reg = 0;

	while (1) {
		reg = read_ahb32(i2c_info->base_address +  I2C_STATUS);
		if (((reg & I2C_STATUS_MA) == 0)|| ((reg & I2C_STATUS_TFE) != 0))  //不是主机模式或传输FIFO空则返回
			break;
	}
}


/* Flushes the I2C RX FIFO */
void snps_i2c_flush_rxfifo(struct snps_i2c_info *i2c_info)
{
	u32 reg = 0;
	reg = read_ahb32(i2c_info->base_address + I2C_STATUS);
	while ((reg & I2C_STATUS_RFNE) != 0)     //RX FIFO不是空的
	{
		read_ahb32(i2c_info->base_address +  I2C_DATA_CMD);
		reg = read_ahb32(i2c_info->base_address + I2C_STATUS);
	}
}


/* waiting for all the transfer is finished after a STOP command issued. */
 u8 snps_i2c_xfer_finish(struct snps_i2c_info *i2c_info)
{
	snps_i2c_wait_for_bb(i2c_info);  //不是主机模式或传输FIFO空则返回

	snps_i2c_flush_rxfifo(i2c_info);  //RX FIFO不空,则读I2C_DATA_CMD和I2C_STATUS

	/* disable I2C */
	snps_i2c_disable(i2c_info);
	return 0;
}


 void snps_i2c_close(struct snps_i2c_info *i2c_info)
{
	snps_i2c_flush_rxfifo(i2c_info);

	/* disable I2C */
	snps_i2c_disable(i2c_info);
}


/*
 * snps_i2c_slave_setaddress - Setting the source slave address
 * @i2c:    the I2C information
 * @dev:       source I2C slave address
 *
 * Setting the source slave address.
 */
 void snps_i2c_slave_set_addr(struct snps_i2c_info *i2c_info, u32 dev)
{
	u32 reg = 0;

	/* disable I2C */
	snps_i2c_disable(i2c_info);

	write_ahb32(i2c_info->base_address + I2C_SAR, dev);
	if (dev > 0x7f) {
		reg = read_ahb32(i2c_info->base_address + I2C_CON);
		reg |= I2C_CON_10BITADDR_SLAVE;
		write_ahb32(i2c_info->base_address + I2C_CON, reg);
	}
}


/*
 * snps_i2c_slave_init - Init function slave mode
 * @i2c:     the I2C information
 * @icp:     contains I2C config parameters
 *
 * Initialization function slave mode.
 */
 u8 snps_i2c_slave_config(struct snps_i2c_info *i2c_info,struct i2c_cfg_param *i2c_cfg)
{
	u32 reg = 0;

	i2c_info->xfer_mode = i2c_cfg->xfer_mode;
	snps_i2c_disable(i2c_info);

	reg = I2C_CON_SPD_SS;
	write_ahb32(i2c_info->base_address + I2C_CON, reg);
//	write_ahb32(i2c_info->base_address + I2C_SAR, SLAVE_ADDRESS);
	snps_i2c_set_speed(i2c_info, i2c_cfg->speed);
	snps_i2c_slave_set_addr(i2c_info, i2c_cfg->dev_addr_as_slave);
	
	snps_i2c_enable(i2c_info);


	return 0;
}


/*
 * snps_i2c_read - Reading from I2C FIFO
 * @i2c:    the I2C information
 * @ixp:     contains xfer parameters
 *
 * Reading from i2c FIFO.
 */
u8 snps_i2c_read(struct snps_i2c_info *i2c_info )
{
	u32 status = 0;
	u32 len = i2c_info->xfer_param.len;
	u32 *buffer = i2c_info->xfer_param.buffer;

//	snps_i2c_set_addr(i2c_info);
//	snps_i2c_wait_for_bb(i2c_info);

		while (len) 
        {       
//			status = read_ahb32(i2c_info->base_address +  I2C_STATUS);
//			if ((status & I2C_STATUS_TFNF) != 0)   //若TX FIFO 没有满？？有什么用
//            {     
//				if (len == 1)                          //len代表要不要发送停止位？？
//					write_ahb32(i2c_info->base_address + I2C_DATA_CMD, (I2C_CMD | I2C_STOP));    //主机读和置起停止位
//				else														  
//					write_ahb32(i2c_info->base_address + I2C_DATA_CMD, I2C_CMD);
     
//				while (1) 
//                {
//					status = read_ahb32(i2c_info->base_address +  I2C_STATUS);
//					if ((status &I2C_STATUS_RFNE) != 0)   //若RX FIFO不空
//         {   
						*buffer = read_ahb32(i2c_info->base_address + I2C_DATA_CMD) & 0xff;
				    printf("rx:%x  ",*buffer);						
						buffer++;   //直到RX FIFO空
						len--;
//						break;
//					}
//				}
//			}
	     }
	return 1;//snps_i2c_xfer_finish(i2c_info);
}

/*
 * snps_i2c_write - Writing to I2C FIFO
 * @i2c:    the I2C information
 * @ixp:     contains xfer parameters
 *
 * Writing to I2C FIFO.
 */
u8 snps_i2c_write(struct snps_i2c_info *i2c_info )
{
	u32 len = i2c_info->xfer_param.len;
	u32 *buffer = i2c_info->xfer_param.buffer;
	u32 status = 0 ;

//	snps_i2c_disable(i2c_info);  //写数据进I2C_DATA_CMD必须使能i2c，要不数据会被清空
		while (len) 
        {
			status = read_ahb32(i2c_info->base_address + I2C_STATUS);     
			if ((status & I2C_STATUS_TFNF)!= 0)    //若TX FIFO没有满
            {  
				if (--len == 0)     //先自减1再参与运算，当len-1时--len=0
					write_ahb32(i2c_info->base_address + I2C_DATA_CMD, *buffer | I2C_STOP);  //当len=1发送数据和停止位，默认主机写
				else
					write_ahb32(i2c_info->base_address + I2C_DATA_CMD, *buffer);
//				snps_i2c_disable(i2c_info);
//        write_ahb32(i2c_info->base_address + I2C_ENABLE,  (~I2C_ENABLE_0B));	
				buffer++;   //直到TX FIFO满
			} 
		}

	return snps_i2c_xfer_finish(i2c_info);
}

void snps_i2c_write_1(struct snps_i2c_info *i2c_info )
{	
	u32 len = i2c_info->xfer_param.len;
	u32 *buffer = i2c_info->xfer_param.buffer;
	u32 status = 0 ;

		while (len) 
    {			
			status = read_ahb32(i2c_info->base_address + I2C_STATUS);            
			if ((status & I2C_STATUS_TFNF)!= 0)    //若TX FIFO没有满
      {  
//				  printf("tx:%x  ",*buffer);
					write_ahb32(i2c_info->base_address + I2C_DATA_CMD, *buffer);
//					write_ahb32(i2c_info->base_address + I2C_ENABLE,  (~I2C_ENABLE_0B));
			  	buffer++;   //直到TX FIFO满
					len--;
			} 
		}
//      	snps_i2c_wait_for_bb(i2c_info);  //不是主机模式或传输FIFO空则返回

//	    snps_i2c_flush_rxfifo(i2c_info);
}


void snps_i2c_unmaskIrq(struct snps_i2c_info *i2c_info, u32 unmask_irq)
{
    u32 reg;

    reg = read_ahb32(i2c_info->base_address + I2C_INTR_MASK);
    // avoid bus write if irq already enabled
    if((unmask_irq & reg) != unmask_irq) {
        reg |= unmask_irq;
        write_ahb32(i2c_info->base_address + I2C_INTR_MASK, reg);
    }
}


void i2c1_nvic_init(void)
{	
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = 14;//18;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); 	
}


void i2c2_nvic_init(void)
{	
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = 15;//19;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); 	
}


void I2C1_IRQHandler(void)//I2C1_IRQHandler
{
	printf("i2c1 enter irq:\r\n");
//	printf("I2C_INTR_MASK0:0x%x  ",read_ahb32(0x40019430));
//	printf("I2C_INTR_STAT0:0x%x  ",read_ahb32(0x4001942c));

	if(read_ahb32(0x4001942c)&0x1)
	{
		printf("I2C_RX_UNDER irq\r\n");
	}
	
	if(read_ahb32(0x4001942c)&0x2)
	{
		printf("I2C_RX_OVER irq\r\n");
	}
	
	if(read_ahb32(0x4001942c)&0x4)
	{
		printf("I2C_RX_FULL irq\r\n");
	}
	
	if(read_ahb32(0x4001942c)&0x8)
	{
		printf("I2C_TX_OVER irq\r\n");
	}
		
	if(read_ahb32(0x4001942c)&0x10)
	{
		printf("I2C_TX_EMPTY irq\r\n");
	}
	
	if(read_ahb32(0x4001942c)&0x20)
	{
		printf("I2C_RD_REQ irq\r\n");
	}
	
	if(read_ahb32(0x4001942c)&0x40)
	{
		printf("I2C_TX_ABRT irq\r\n");
	}
	
	if(read_ahb32(0x4001942c)&0x80)
	{
		printf("I2C_RX_DONE irq\r\n");
	}
	
	if(read_ahb32(0x4001942c)&0x100)
	{
		printf("I2C_ACTIVITY irq\r\n");
	}
	
	if(read_ahb32(0x4001942c)&0x200)
	{
		printf("I2C_STOP_DET irq\r\n");
	}
	
	if(read_ahb32(0x4001942c)&0x400)
	{
		printf("I2C_START_DET irq\r\n");
	}
	
	if(read_ahb32(0x4001942c)&0x800)
	{
		printf("I2C_GEN_CALL irq\r\n");
	}
	
	if(read_ahb32(0x4001942c)&0x1000)
	{
		printf("I2C_RESTART_DET irq\r\n");
	}
	
	if(read_ahb32(0x4001942c)&0x2000)
	{
		printf("I2C_MASTER_ON_HOLD irq\r\n");
	}
	
	if(read_ahb32(0x4001942c)&0x4000)
	{
		printf("I2C_SCL_STUCK_AT_LOW irq\r\n");
	}
	write_ahb32(0x40019430,~read_ahb32(0x4001942c)&read_ahb32(0x40019430));
//	printf("I2C_INTR_MASK0:0x%x\r\n",read_ahb32(0x40019430));
}



void I2C2_IRQHandler(void)//I2C2_IRQHandler
{	
	u32 reg;
	printf("i2c2 enter irq:\r\n");
//	printf("I2C_INTR_MASK0:0x%x  ",read_ahb32(0x40019830));
//	printf("I2C_INTR_STAT0:0x%x  ",read_ahb32(0x4001982c));
			
	if(read_ahb32(0x4001982c)&0x1)
	{
		printf("I2C_RX_UNDER irq\r\n");
	}
	
	if(read_ahb32(0x4001982c)&0x2)
	{
		printf("I2C_RX_OVER irq\r\n");
	}
	
	if(read_ahb32(0x4001982c)&0x4)
	{
		printf("I2C_RX_FULL irq\r\n");
	}
	
	if(read_ahb32(0x4001982c)&0x8)
	{
		printf("I2C_TX_OVER irq\r\n");
	}
		
	if(read_ahb32(0x4001982c)&0x10)
	{
		printf("I2C_TX_EMPTY irq\r\n");
	}
	
	if(read_ahb32(0x4001982c)&0x20)
	{
		printf("I2C_RD_REQ irq\r\n");
	}
	
	if(read_ahb32(0x4001982c)&0x40)
	{
		printf("I2C_TX_ABRT irq\r\n");
	}
	
	if(read_ahb32(0x4001982c)&0x80)
	{
		printf("I2C_RX_DONE irq\r\n");
	}
	
	if(read_ahb32(0x4001982c)&0x100)
	{
		printf("I2C_ACTIVITY irq\r\n");
	}
	
	if(read_ahb32(0x4001982c)&0x200)
	{
		printf("I2C_STOP_DET irq\r\n");
	}
	
	if(read_ahb32(0x4001982c)&0x400)
	{
		printf("I2C_START_DET irq\r\n");
	}
	
	if(read_ahb32(0x4001982c)&0x800)
	{
		printf("I2C_GEN_CALL irq\r\n");
	}
	
	if(read_ahb32(0x4001982c)&0x1000)
	{
		printf("I2C_RESTART_DET irq\r\n");
	}
	
	if(read_ahb32(0x4001982c)&0x2000)
	{
		printf("I2C_MASTER_ON_HOLD irq\r\n");
	}
	
	if(read_ahb32(0x4001982c)&0x4000)
	{
		printf("I2C_SCL_STUCK_AT_LOW irq\r\n");
	}
	
	write_ahb32(0x40019830,~read_ahb32(0x4001982c)&read_ahb32(0x40019830));
//	printf("I2C_INTR_MASK0:0x%x\r\n",read_ahb32(0x40019830));
}
