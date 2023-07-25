#include "sys.h"
#include "delay.h"
#include "sample_efc.h"
#include "stm32f10x.h"
#include "usart.h"
/*******************************************************************************
 * @brief   sample_efc_test_t13
 * Function eflash test
 * @author  taohu
 * @date    2022-06-23
 * @param   
		
 * @return void
*******************************************************************************/   
void page_pgm(void)
{
	//pre_program
	sample_timing_cfg(TPE_PP/BUS_PRD);   //SET TIMING
	
	//set PEP
	while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
	write_ahb32(0x1003000C,0x5a000070);
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位

	//pgm
	while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
	write_ahb32(0x1003000C,0x5a0000c0);
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位

	//ER
	sample_timing_cfg(TPE_E/BUS_PRD);
	while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
	write_ahb32(0x1003000C,0x5a000080);
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位

	//切换到读模式
	write_ahb32(0x1003000C,0x5a000000);
	printf("read word1:0x%x\r\n",read_ahb32(0x10000000));

	
	//CLEAR PAGE BUFFER
	while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
	write_ahb32(0x1003000C,0x5a000010);
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位
	
#if 1
	//write page latch all
	while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
	write_ahb32(0x1003000C,0x5a000020);
	write_ahb32(0x10000000,0xAA);
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位
#endif
	
#if 0
		//write page byte to byte 
		while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
		write_ahb32(0x1003000C,0x5a000000);
		
		{
			u32 i;
			for(i=0;i<64;i++)
			{	
				write_ahb32(0x10000000+4*i,i*4);
			}
		}
		
		while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位
#endif
	
		//PGM
		sample_timing_cfg(TPE_P/BUS_PRD);
		while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
		write_ahb32(0x1003000C,0x5a0000C0);
		//printf("PGM FLS_STAT4:0x%x\r\n",EFC->fls_stat);
		while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位
}

void page_erase(void)
{
	//pre_program
	sample_timing_cfg(TPE_PP/BUS_PRD);	 //SET TIMING

	//set PEP
	while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
	write_ahb32(0x1003000C,0x5a000070);
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位

	//pgm
	while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
	write_ahb32(0x1003000C,0x5a0000c0);
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位

	//ER
	sample_timing_cfg(TPE_E/BUS_PRD);
	while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
	write_ahb32(0x1003000C,0x5a000080);
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位
}


void sample_efc_t0(void)
{	
	u32 i;

	sample_unlock_ctrl();
	sample_timing_cfg(TPE_PP/BUS_PRD);
	
	for(i=0;i<17;i++)
	{
		printf("0x%x:0x%x\r\n",i*4,read_ahb32(EFC_BASE+i*4));
	}
	printf("\r\n");
	
	sample_trm_rld();

	page_pgm();
	//page_erase();

	//切换到读模式
	write_ahb32(0x1003000C,0x5a000000);

	for(i=0;i<64;i++)
	{
		printf("0x%x:read word:0x%x\r\n",i*4,read_ahb32(0x10000000+(4*i)));
	}
	

}