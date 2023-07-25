
#ifndef SNPS_DMAC_H
#define SNPS_DMAC_H

#include "stm32f10x.h"

#define AHB_DMAC_BASE		DMA1_BASE

// Macro definitions for DMA controller limits
#define DMAC_MAX_CHANNELS    8
#define DMAC_MAX_INTERRUPTS  5
#define DMAC_MAX_MULT_SIZE   16 

// Macro definitions for the flow control mode of the DMA Controller
#define DMAC_DMA_FC_ONLY     0x0
#define DMAC_SRC_FC_ONLY     0x1
#define DMAC_DST_FC_ONLY     0x2
#define DMAC_ANY_FC          0x3


#define DMAC_MAX_CH_MASK  	0xffff
#define DMAC_CH_MASK  		0x0
#define DMAC_CH_EN_MASK     0xff
#define DMAC_CH_ALL_MASK    0xffff

// get a dmac_channel from a channel index
#define DMAC_CH_NUM(ch_idx)     (0x101 << (ch_idx))

typedef void (*dmac_callback)();


#define   DMAC_NUM_CHANNELS				8
#define   DMAC_NUM_MASTER				1
#define   DMAC_CH_FIFO_DEPTH			64
#define   DMAC_CH_MAX_MULT_SZIE			16
#define   DMAC_MAX_NUM_HS_INT			16

/*****/

/* Channel registers                                    */  
/* ch is the channel number, range0~7, channel 0~channel 7   */
#define DMAC_SAR_L(ch)           						(0x000 + ch * 0x58 )
#define DMAC_SAR_H(ch)           						(0x004 + ch * 0x58 )
#define DMAC_DAR_L(ch)           						(0x008 + ch * 0x58 )
#define DMAC_DAR_H(ch)           						(0x00c + ch * 0x58 )
#define DMAC_LLP_L(ch)           						(0x010 + ch * 0x58 )
#define DMAC_LLP_H(ch)           						(0x014 + ch * 0x58 )
#define DMAC_CTL_L(ch)           						(0x018 + ch * 0x58 )
#define DMAC_CTL_H(ch)           						(0x01c + ch * 0x58 )
#define DMAC_SSTAT_L(ch)         						(0x020 + ch * 0x58 )
#define DMAC_SSTAT_H(ch)         						(0x024 + ch * 0x58 )
#define DMAC_DSTAT_L(ch)         						(0x028 + ch * 0x58 )
#define DMAC_DSTAT_H(ch)         						(0x02c + ch * 0x58 )
#define DMAC_SSTATAR_L(ch)       						(0x030 + ch * 0x58 )
#define DMAC_SSTATAR_H(ch)       						(0x034 + ch * 0x58 )
#define DMAC_DSTATAR_L(ch)       						(0x038 + ch * 0x58 )
#define DMAC_DSTATAR_H(ch)       						(0x03c + ch * 0x58 )
#define DMAC_CFG_L(ch)           						(0x040 + ch * 0x58 )
#define DMAC_CFG_H(ch)           						(0x044 + ch * 0x58 )
#define DMAC_SGR_L(ch)           						(0x048 + ch * 0x58 )
#define DMAC_SGR_H(ch)           						(0x04c + ch * 0x58 )
#define DMAC_DSR_L(ch)           						(0x050 + ch * 0x58 )
#define DMAC_DSR_H(ch)           						(0x054 + ch * 0x58 )

/* Interrupt Raw Status Registers */
#define DMAC_RAW_TFR_L                					0x2c0     
#define DMAC_RAW_TFR_H                					0x2c4          
#define DMAC_RAW_BLOCK_L              					0x2c8     
#define DMAC_RAW_BLOCK_H              					0x2cc          
#define DMAC_RAW_SRCTRAN_L            					0x2d0     
#define DMAC_RAW_SRCTRAN_H            					0x2d4          
#define DMAC_RAW_DSTTRAN_L            					0x2d8     
#define DMAC_RAW_DSTTRAN_H            					0x2dc          
#define DMAC_RAW_ERR_L                					0x2e0     
#define DMAC_RAW_ERR_H											0x2e4

/* Interrupt Status Registers */
#define DMAC_STATUS_TFR_L              			0x2e8
#define DMAC_STATUS_TFR_H              			0x2ec
#define DMAC_STATUS_BLOCK_L            			0x2f0
#define DMAC_STATUS_BLOCK_H            			0x2f4
#define DMAC_STATUS_SRCTRAN_L          			0x2f8
#define DMAC_STATUS_SRCTRAN_H          			0x2fc
#define DMAC_STATUS_DSTTRAN_L          			0x300
#define DMAC_STATUS_DSTTRAN_H          			0x304
#define DMAC_STATUS_ERR_L              			0x308
#define DMAC_STATUS_ERR_H              			0x30c

/* Interrupt Mask Registers */
#define DMAC_MASK_TFR_L      					0x310
#define DMAC_MASK_TFR_H      								0x314
#define DMAC_MASK_BLOCK_L    								0x318
#define DMAC_MASK_BLOCK_H    								0x31c
#define DMAC_MASK_SRCTRAN_L  								0x320
#define DMAC_MASK_SRCTRAN_H  								0x324
#define DMAC_MASK_DSTTRAN_L  								0x328
#define DMAC_MASK_DSTTRAN_H  								0x32c
#define DMAC_MASK_ERR_L      								0x330
#define DMAC_MASK_ERR_H											0x334

/* Interrupt Clear Registers */
#define DMAC_CLEAR_TFR_L     								0x338      
#define DMAC_CLEAR_TFR_H     								0x33c          
#define DMAC_CLEAR_BLOCK_L   								0x340      
#define DMAC_CLEAR_BLOCK_H   								0x344           
#define DMAC_CLEAR_SRCTRAN_L 								0x348      
#define DMAC_CLEAR_SRCTRAN_H 								0x34c           
#define DMAC_CLEAR_DSTTRAN_L 								0x350      
#define DMAC_CLEAR_DSTTRAN_H 								0x354           
#define DMAC_CLEAR_ERR_L     								0x358      
#define DMAC_CLEAR_ERR_H     					0x35c           
#define DMAC_STATUS_INT_L    					0x360      
#define DMAC_STATUS_INT_H    					0x364

/* Software Handshaking Registers */
#define DMAC_REQ_SRC_REG_L               		0x368        
#define DMAC_REQ_SRC_REG_H               		0x36c             
#define DMAC_REQ_DST_REG_L               		0x370        
#define DMAC_REQ_DST_REG_H               		0x374             
#define DMAC_SGL_RQ_SRC_REG_L            		0x378        
#define DMAC_SGL_RQ_SRC_REG_H            		0x37c             
#define DMAC_SGL_RQ_DST_REG_L            		0x380        
#define DMAC_SGL_RQ_DST_REG_H            		0x384             
#define DMAC_LST_SRC_REG_L               		0x388        
#define DMAC_LST_SRC_REG_H               		0x38c             
#define DMAC_LST_DST_REG_L               		0x390        
#define DMAC_LST_DST_REG_H						0x394

/* Misc Registers */
#define DMAC_DMA_CFG_REG_L               		0x398         
#define DMAC_DMA_CFG_REG_H               		0x39c              
#define DMAC_CH_EN_REG_L                 		0x3a0         
#define DMAC_CH_EN_REG_H                 		0x3a4              
#define DMAC_DMA_ID_REG_L                		0x3a8         
#define DMAC_DMA_ID_REG_H                		0x3ac              
#define DMAC_DMA_TEST_REG_L              		0x3b0         
#define DMAC_DMA_TEST_REG_H              		0x3b4              
#define DMAC_OLD_VERSION_ID_L            		0x3b8         
#define DMAC_OLD_VERSION_ID_H            		0x3bc              
#define DMAC_RESERVED_LOW                		0x3c0         
#define DMAC_RESERVED_HIGH               		0x3c4                            
#define DMAC_COMP_PARAMS_6_L             		0x3c8         
#define DMAC_COMP_PARAMS_6_H             		0x3cc              
#define DMAC_COMP_PARAMS_5_L             		0x3d0         
#define DMAC_COMP_PARAMS_5_H             		0x3d4              
#define DMAC_COMP_PARAMS_4_L             		0x3d8         
#define DMAC_COMP_PARAMS_4_H             		0x3dc             
#define DMAC_COMP_PARAMS_3_L             		0x3e0         
#define DMAC_COMP_PARAMS_3_H             		0x3e4              
#define DMAC_COMP_PARAMS_2_L             		0x3e8         
#define DMAC_COMP_PARAMS_2_H             		0x3ec              
#define DMAC_COMP_PARAMS_1_L             		0x3f0         
#define DMAC_COMP_PARAMS_1_H             		0x3f4             
#define DMAC_VERSION_ID_L 						0x3f8         
#define DMAC_VERSION_ID_H						0x3fc
 

//burst request for the DMAC handshake interface define 
#define USART1_RX_DMA_BREQ		0
#define USART1_TX_DMA_BREQ		1
#define USART2_RX_DMA_BREQ		2
#define USART2_TX_DMA_BREQ		3
#define USART3_RX_DMA_BREQ		4
#define USART3_TX_DMA_BREQ		5
#define SPI1_RX_DMA_BREQ		6
#define SPI1_TX_DMA_BREQ		7
#define SPI2_RX_DMA_BREQ		8
#define SPI2_TX_DMA_BREQ		9
#define I2C1_RX_DMA_BREQ		10
#define I2C1_TX_DMA_BREQ		11
#define I2C2_RX_DMA_BREQ		12
#define I2C2_TX_DMA_BREQ		13
#define USB_DMA_BREQ			14
 

//#define AHBDMA_DEBUG_PRINTF
#ifdef AHBDMA_DEBUG_PRINTF
    #define snps_dma_printf(fmt, arg ...)       uart_printf(fmt, arg ...) 
#else
	#define snps_dma_printf(fmt, arg ...)		do {} while (0)
#endif

#define IRQ_TYPE_ALL			0x1f
#define IRQ_TYPE_TFR			0x1
#define IRQ_TYPE_BLOCK			0x2
#define IRQ_TYPE_SRCTRAN		0x4
#define IRQ_TYPE_DSTTRAN		0x8
#define IRQ_TYPE_ERROR			0x10

#define MEM2MEM_MAX_BLK_BYTES	512	//128 Single trans with 32 bits width,total 512 bytes 
#define MEMPER_MAX_BLK_BYTES	128	//128 Single trans with 8 bits width,total 128 bytes 

#define DMA_TRANS_TYPE_MEM2MEM 		0
#define DMA_TRANS_TYPE_MEM2PER		1	
#define DMA_TRANS_TYPE_PER2MEM		2

#define TR_WIDTH_8BITS				0
#define TR_WIDTH_16BITS				1
#define TR_WIDTH_32BITS				2


void dw_dmac_enable(void)	 ;

void dw_dmac_disable(void);

bool dw_dmac_isEnabled(void) ;

int dw_dmac_checkChannelBusy(int channel) ;

void dw_dmac_enableChannel(int channel)	;

void dw_dmac_disableChannel(int channel)	;

bool dw_dmac_isChannelEnabled(int channel);

void dw_dmac_clearIrq(int channel, int irq_type);

void dw_dmac_maskIrq(int channel, int irq_type)	;

void dw_dmac_unmaskIrq(int channel, int irq_type)  ;

int dw_dmac_getIdleChannel(void);

void dw_dmac_init(void);




int dmac_mem2per_channel_cfg(int channel);
int dmac_per2mem_channel_cfg(int channel);


void dmac_ch_close(int channel);
int dmac_checkdone(int channel);
#endif /* SNPS_AHB_DMAC_H */



