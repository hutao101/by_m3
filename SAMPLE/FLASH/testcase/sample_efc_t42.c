#include "sys.h"
#include "delay.h"
#include "sample_efc.h"
#include "stm32f10x.h"

/*******************************************************************************
 * @brief   sample_efc_test_t1
 * Function eflash test
 * @author  taohu
 * @date    2022-06-23
 * @param   
		
 * @return void
*******************************************************************************/   
void sample_efc_t42(void)
{	
		u32 i,j;
	
		sample_unlock_ctrl();
		sample_timing_cfg(TPE_PP/BUS_PRD);
		
		sample_trm_rld();
		
	
		//512次PAGE PGM，CHIP值都为0x12345678
		for(i=255;i<256;i++)
		{
			sample_page_pgm(i,0x12345678);	
		}
		
		//非对齐读00
		printf("A[1:0] = 00\r\n");
		for(i=255;i<256;i++)
		{
			printf("%d:----------------------\r\n",i);
			for(j=0;j<64;j++)
			{
				printf("0x%x:read word:0x%x\r\n",(j*4),sample_word_read((0x10000000 + (i<<8))+(4*j)));
			}
		}
		
		//非对齐读01
		printf("A[1:0] = 01\r\n");
		for(i=255;i<256;i++)
		{
			printf("%d:----------------------\r\n",i);
			for(j=0;j<64;j++)
			{
				printf("0x%x:read word:0x%x\r\n",(j*4)+1,sample_word_read((0x10000000 + (i<<8))+(4*j)+1));
			}
		}
		
		//非对齐读02
		printf("A[1:0] = 02\r\n");
		for(i=255;i<256;i++)
		{
			printf("%d:----------------------\r\n",i);
			for(j=0;j<64;j++)
			{
				printf("0x%x:read word:0x%x\r\n",(j*4)+2,sample_word_read((0x10000000 + (i<<8))+(4*j)+2));
			}
		}
		
		//非对齐读03
		printf("A[1:0] = 03\r\n");
		for(i=255;i<256;i++)
		{
			printf("%d:----------------------\r\n",i);
			for(j=0;j<64;j++)
			{
				printf("0x%x:read word:0x%x\r\n",(j*4)+3,sample_word_read((0x10000000 + (i<<8))+(4*j)+3));
			}
		}
}
