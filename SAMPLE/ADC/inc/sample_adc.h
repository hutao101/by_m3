#ifndef _SAMPLE_ADC_H_
#define _SAMPLE_ADC_H_

#define read_ahb32(mreg)	 *(volatile unsigned int *)(mreg)
#define write_ahb32(mreg,val)   *(volatile unsigned int *)(mreg) = (val)

#include "sys.h"
#include "delay.h"
#include "stm32f10x.h"

//ADC REG
#define  ADC_CTRL   (ADC1_BASE + 0x00)
#define  ADC_MODE   (ADC1_BASE + 0x04)
#define  ADC_STAT   (ADC1_BASE + 0x08)
#define  ADC_DATA   (ADC1_BASE + 0x0C)
#define  ADC_CH0    (ADC1_BASE + 0x10)
#define  ADC_CH1    (ADC1_BASE + 0x14)
#define  ADC_CH2    (ADC1_BASE + 0x18)


//ADC_CTRL[30:28]
#define ADC_TrigConv_T1_CC1           ((uint32_t)0x00000000) /*!< For ADC1 and ADC2 */
#define ADC_TrigConv_T1_CC2           ((uint32_t)0x10000000) /*!< For ADC1 and ADC2 */
#define ADC_TrigConv_T1_CC3           ((uint32_t)0x20000000) /*!< For ADC1 and ADC2 */
#define ADC_TrigConv_T2_CC2           ((uint32_t)0x30000000) /*!< For ADC1 and ADC2 */
#define ADC_TrigConv_T3_TRGO          ((uint32_t)0x40000000) /*!< For ADC1 and ADC2 */
#define ADC_TrigConv_T4_CC4           ((uint32_t)0x50000000) /*!< For ADC1 and ADC2 */
#define ADC_TrigConv_EXTI             ((uint32_t)0x60000000) /*!< For ADC1 and ADC2 */
#define ADC_TrigConv_SW               ((uint32_t)0x70000000) /*!< For ADC1 and ADC2 */

/* ADC_CTRL[27]:EXTTRIG mask */
#define ADC_EXTTRIG_Set             ((uint32_t)1<<27)
#define ADC_EXTTRIG_Reset           ((uint32_t)(~(1<<27)))

/* ADC_CTRL[26]:Software start mask */
#define ADC_EXTTRIG_SWSTART_Set     ((uint32_t)1<<26)
#define ADC_EXTTRIG_SWSTART_Reset   ((uint32_t)(~(1<<26)))


#define  ADC_CTRL_LEFT           0x2000000
#define  ADC_CTRL_underint       0x800000
#define  ADC_CTRL_overint        0x400000
#define  ADC_CTRL_datareadyint   0x200000

#define  ADC_CTRL_FIFO_0         0x00000
#define  ADC_CTRL_FIFO_1         0x10000
#define  ADC_CTRL_FIFO_2         0x20000
#define  ADC_CTRL_FIFO_3         0x30000
#define  ADC_CTRL_FIFO_4         0x40000
#define  ADC_CTRL_FIFO_5         0x50000
#define  ADC_CTRL_FIFO_6         0x60000
#define  ADC_CTRL_FIFO_7         0x70000
#define  ADC_CTRL_FIFO_8         0x80000
#define  ADC_CTRL_FIFO_9         0x90000
#define  ADC_CTRL_FIFO_10        0xa0000
#define  ADC_CTRL_FIFO_11        0xb0000
#define  ADC_CTRL_FIFO_12        0xc0000
#define  ADC_CTRL_FIFO_13        0xd0000
#define  ADC_CTRL_FIFO_14        0xe0000
#define  ADC_CTRL_FIFO_15        0xf0000
#define  ADC_CTRL_FIFO_16        0x100000

#define  ADC_CTRL_Discount1      0x2000
#define  ADC_CTRL_Discount2      0x4000
#define  ADC_CTRL_Discount3      0x6000
#define  ADC_CTRL_Discount4      0x8000
#define  ADC_CTRL_Discount5      0xa000
#define  ADC_CTRL_Discount6      0xc000
#define  ADC_CTRL_Discount7      0xe000

#define  ADC_CTRL_BIAS_DOWN      0x80 
#define  ADC_CTRL_SAR_DOWN       0x40
#define  ADC_CTRL_DIFFER         0x20

#define  ADC_MODE_DISCON         0x8
#define  ADC_MODE_SCAN           0x4
#define  ADC_MODE_CONTIN         0x2
#define  ADC_MODE_SINGLE         0x1

#define ADC_CH2_CHALEN_1         0x000000
#define ADC_CH2_CHALEN_2         0x100000
#define ADC_CH2_CHALEN_3         0x200000
#define ADC_CH2_CHALEN_4         0x300000
#define ADC_CH2_CHALEN_5         0x400000
#define ADC_CH2_CHALEN_6         0x500000
#define ADC_CH2_CHALEN_7         0x600000
#define ADC_CH2_CHALEN_8         0x700000
#define ADC_CH2_CHALEN_9         0x800000
#define ADC_CH2_CHALEN_10        0x900000
#define ADC_CH2_CHALEN_11        0xa00000
#define ADC_CH2_CHALEN_12        0xb00000
#define ADC_CH2_CHALEN_13        0xc00000
#define ADC_CH2_CHALEN_14        0xd00000
#define ADC_CH2_CHALEN_15        0xe00000
#define ADC_CH2_CHALEN_16        0xf00000

#define ADC_STAT_BUSY            0x100
#define ADC_STAT_underflow       0x4
#define ADC_STAT_overflow        0x2
#define ADC_STAT_ready           0x1


void adc1_nvic_init(u32 channel);
void adc_systick_init(u32 reload);
void adc_tim_nvic_init(u32 channel);

void sample_adc_t1(void);
void sample_adc_t2(void);
void sample_adc_t3(void);
void sample_adc_t4(void);
void sample_adc_t5(void);
void sample_adc_t6(void);
void sample_adc_t6_1(void);
void sample_adc_t6_2(void);
void sample_adc_t7(void);
void sample_adc_t7_1(void);
void sample_adc_t7_2(void);
void sample_adc_t7_3(void);
void sample_adc_t7_4(void);
void sample_adc_t7_5(void);
void sample_adc_t7_6(void);
void sample_adc_t7_7(void);
void sample_adc_t8(void);
void sample_adc_t9(void);
void sample_adc_t10(void);
void sample_adc_t11(void);
void sample_adc_t12(void);
void sample_adc_t13(void);
#endif

