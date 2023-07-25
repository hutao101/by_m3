#ifndef __sample_dma_H__
#define __sample_dma_H__

#include "snps_ahb_dma.h"
#include "sample_i2c.h"
#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"


#define read_ahb32(mreg)	 *(volatile unsigned int *)(mreg)
#define write_ahb32(mreg,val)   *(volatile unsigned int *)(mreg) = (val)

/************************************************************************
*以下为DMA测试用例，需要注意的是有些函数是公用的，比如IIC驱动函数、
* DMA中断函数，涉及到IIC的为避免硬件错误，需每次重新下载程序前手动复位，因为
* IIC驱动函数中使能IIC后未将IIC关闭
*
**************************************************************************/

void sample_dma_t1(void);    //t1 m2m 32bit

void sample_dma_t2(int channel);    //t2 p2m 32bit

void sample_dma_t3(int channel);		//t3 m2p 8bit
void sample_dma_t3_8ch(void);

void sample_dma_t4(int channel);		//t4 m2m 16bit
void sample_dma_t5(int channel);		//t5 m2m 8bit

void sample_dma_t6(int channel);		//t6 p2m 16bit

void sample_dma_t7(int channel);		//t7 p2m 8bit
void sample_dma_t7_8ch(void);
void sample_dma_t7_before_iic_enable(int channel);

void sample_dma_t8(int channel);		//t8 m2p 16bit
void sample_dma_t8_8ch(void);

void sample_dma_t9(int channel);		//t9 m2p 32bit
void sample_dma_t9_8ch(void);
void sample_dma_t9_before_iic_enable(int channel);

void sample_dma_t10(int channel);   //t10 m2p 32-16bit
void sample_dma_t10_8ch(void);

void sample_dma_t11(int channel);		//t11 m2p 32-32bit
void sample_dma_t11_8ch(void);		

void sample_dma_t12(int channel);		//t12 p2m 16-32bit
void sample_dma_t12_8ch(void);

void sample_dma_t13(int channel);		//t13 p2m 32-32bit
void sample_dma_t13_8ch(void);

//该函数包含公共中断函数，需在t14、t15、t16注释多余的中断服务函数
void sample_dma_t14(int channel);    //t14 m2p 8-8bit中断测试（不包含错误中断） 调用该函数需分配通道0
//void sample_dma_t14_8ch(void);

void sample_dma_t15(int channel);    //t15 m2m 8-8bit 中断测试（包含错误中断）  调用该函数需分配通道0

void sample_dma_t16(int channel);     //t16 p2m 8-8bit 中断测试（不包含错误中断）调用该函数需分配通道0

void sample_dma_t18(int channel);  //t18 p2m 8-8bit iic1-iic2

void sample_dma_t19(int channel);  //t19 p2m 8-8 iic2-iic1

void sample_dma_t20(int channel);   //t22 m2p iic1->iic2
 
void sample_dma_t21(int channel);  //t21 m2p iic2->iic1

#endif
