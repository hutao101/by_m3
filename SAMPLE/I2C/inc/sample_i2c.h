#ifndef __SAMPLE_I2C_H__
#define __SAMPLE_I2C_H__

#include "stdio.h"	
#include "sys.h" 
#include "stm32f10x.h"

//#define I2C1_IRQ 18
//#define I2C2_IRQ 19
#define I2C_PCLK							50 // in Mhz
#define I2C_FIFO_DEPTH						16

/* I2C Slave Address in slave mode */
#define SLAVE_ADDRESS						0xb
#define SLAVE_ADDRESS_10						0x3ab

/* supprot speed define */
#define I2C_SPEED_MODE_MAX					4
#define I2C_MAX_SPEED						3400000	// 3.4Mhz
#define I2C_SPEED_MODE_FAST_PLUS			3
#define I2C_FAST_PLUS_SPEED					1000000 //1Mhz
#define I2C_SPEED_MODE_FAST					2
#define I2C_FAST_SPEED						400000	//400Khz
#define I2C_SPEED_MODE_STANDARD				1
#define I2C_STANDARD_SPEED					100000  //100Khz, CLK is 10us

/* I2C register define  */
#define  I2C_CON							0x00
#define  I2C_TAR							0x04
#define  I2C_SAR							0x08
#define  I2C_HS_MADDR						0x0c
#define  I2C_DATA_CMD						0x10
#define  I2C_SS_SCL_HCNT					0x14
#define  I2C_SS_SCL_LCNT					0x18
#define  I2C_FS_SCL_HCNT					0x1c
#define  I2C_FS_SCL_LCNT					0x20
#define  I2C_HS_SCL_HCNT					0x24
#define  I2C_HS_SCL_LCNT					0x28
#define  I2C_INTR_STAT						0x2c
#define  I2C_INTR_MASK						0x30
#define  I2C_RAW_INTR_STAT					0x34
#define  I2C_RX_TL							0x38
#define  I2C_TX_TL							0x3c
#define  I2C_CLR_INTR						0x40
#define  I2C_CLR_RX_UNDER					0x44
#define  I2C_CLR_RX_OVER					0x48
#define  I2C_CLR_TX_OVER					0x4c
#define  I2C_CLR_RD_REQ						0x50
#define  I2C_CLR_TX_ARBT					0x54
#define  I2C_CLR_RX_DONE					0x58
#define  I2C_CLR_ACTIVITY					0x5c
#define  I2C_CLR_STOP_DET					0x60
#define  I2C_CLR_START_DET					0x64
#define  I2C_CLR_GEN_CALL					0x68
#define  I2C_ENABLE							0x6c

/* IC_ENABLE register bits */
#define I2C_ENABLE_ABORT					0x0002
#define I2C_ENABLE_0B						0x0001

#define  I2C_STATUS							0x70

/* I2C status register definitions */
#define I2C_STATUS_SA						0x0040
#define I2C_STATUS_MA						0x0020     //主机是活动状态
#define I2C_STATUS_RFF						0x0010   //接收FIFO满
#define I2C_STATUS_RFNE						0x0008
#define I2C_STATUS_TFE						0x0004
#define I2C_STATUS_TFNF						0x0002
#define I2C_STATUS_ACT						0x0001   //i2c是活动状态

#define  I2C_TXFLR							0x74
#define  I2C_RXFLR							0x78
#define  I2C_SDA_HOLD						0x7c
#define  I2C_TX_ABRT_SOURCE					0x80
#define  I2C_SLV_DATA_NACK_ONLY				0x84
#define  I2C_DMA_CR							0x88

/* IC_DMA_CR Register bits */
#define I2C_DMA_CR_TDMAE					0x2
#define I2C_DMA_CR_RDMAE					0x1

#define  I2C_DMA_TDLR						0x8c
#define  I2C_DMA_RDLR						0x90
#define  I2C_SDA_SETUP						0x94
#define  I2C_ACK_GENERAL_CALL				0x98
#define  I2C_ENABLE_STATUS					0x9c     //使不使能由IC_ENABLE寄存器决定，读I2C_ENABLE_STATUS判断状态
#define  I2C_FS_SPKLEN						0xa0
#define  I2C_HS_SPKLEN						0xa4
#define  I2C_CLR_RESTART_DET				0xa8
#define  I2C_SCL_STUCK_AT_LOW_TIMEOUT		0xac
#define  I2C_SDA_STUCK_AT_LOW_TIMEOUT 		0xb0
#define  I2C_CLR_SCL_STUCK_DET				0xb4
#define  I2C_DEVICE_ID						0xb8
#define  I2C_SMBUS_CLK_LOW_SEXT				0xbc
#define  I2C_SMBUS_CLK_LOW_MEXT				0xc0
#define  I2C_SMBUS_THIGH_MAX_IDLE_COUNT		0xc4
#define  I2C_SMBUS_INTR_STAT				0xc8
#define  I2C_SMBUS_INTR_MASK				0xcc
#define  I2C_SMBUS_RAW_INTR_STAT			0xd0
#define  I2C_CLR_SMBUS_INTR					0xd4
#define  I2C_OPTIONAL_SAR					0xd8
#define  I2C_SMBUS_UDID_LSB					0xdc
#define  I2C_COMP_PARAM_1					0xf4
#define  I2C_COMP_VERSION					0xf8
#define  I2C_COMP_TYPE						0xfc

/* I2C data buffer and command register definitions */
#define I2C_CMD								0x0100 //主机读
#define I2C_STOP							0x0200
#define I2C_RESTART							0x0400

/* I2C control register definitions */
#define I2C_CON_SD							0x0040   //slave disable
#define I2C_CON_RE							0x0020   //restart enable
#define I2C_CON_10BITADDRMASTER				0x0010
#define I2C_CON_10BITADDR_SLAVE				0x0008
#define I2C_CON_SPD_MSK						0x0006  //high speed
#define I2C_CON_SPD_SS						0x0002  //standard speed
#define I2C_CON_SPD_FS						0x0004  //fast or fast plus mode
#define I2C_CON_SPD_HS						0x0006
#define I2C_CON_MM							0x0001   //master mode
#define I2C_CON_BUS_CLEAR							0x0800 

/*
 * High and low times in different speed modes (in ns).
 * You can find it in spec.
 */
#define MIN_SS_SCL_HIGHTIME					4000
#define MIN_SS_SCL_LOWTIME					4700
#define MIN_FS_SCL_HIGHTIME					600
#define MIN_FS_SCL_LOWTIME					1300
#define MIN_FS_PLUS_SCL_HIGHTIME			260
#define MIN_FS_PLUS_SCL_LOWTIME				500
#define MIN_HS_SCL_HIGHTIME					60
#define MIN_HS_SCL_LOWTIME					160

/* I2C interrupt status register bits definitions */
#define I2C_SCL_STUCK_AT_LOW    			0x4000
#define I2C_MST_ON_HOLD		    			0x2000
#define I2C_RESTART_DET          			0x1000
#define I2C_GEN_CALL		            	0x0800
#define I2C_START_DET			            0x0400
#define I2C_STOP_DET						0x0200
#define I2C_ACTIVITY						0x0100
#define I2C_RX_DONE							0x0080
#define I2C_TX_ABRT							0x0040
#define I2C_RD_REQ							0x0020
#define I2C_TX_EMPTY						0x0010
#define I2C_TX_OVER							0x0008
#define I2C_RX_FULL							0x0004
#define I2C_RX_OVER							0x0002
#define I2C_RX_UNDER						0x0001

/* IC_TX_ABRT_SOURCE */
#define I2C_ABRT_7B_ADDR_NOACK				0x1
#define send_data   0x32

#define snps_i2c_printf(fmt, arg ...)		do {} while (0)

 /* Contains I2C xfer mode */
enum i2c_xfer_mode {
	I2C_POLLING_MODE,
	I2C_INTR_MODE,
	I2C_DMA_MODE
};

#define I2C_READ_XFER							0
#define I2C_WRITE_XFER				  	1

//typedef enum IRQ
//{
#define	I2C1_IRQ	            18     /*!< I2C1 Interrupt           */
#define  I2C2_IRQ             19      /*!< I2C2 Interrupt					  */	
//}IRQ_Type;

/*
 * i2c_cfg_param: I2C cconfig parameters.
 * For all modes, slave_mode_en, xfer_mode, speed must be set.
 * For interrupt mode, tx_tl, rx_tl must be set.
 * For DMA mode, dma_rdlr, dma_tdlr must be set.
 * For slave mode, slave_mode_en must be set to 1, and dev_addr_as_slave
 * must be set.
 */
struct i2c_cfg_param {
	
	u8 slave_mode_en; //When I2C as a slave, set this parameter to 1, otherwise set to 0	
	u16 dev_addr_as_slave;  // Setting this when slave_mode_en is set to 1.	
	enum i2c_xfer_mode xfer_mode;  //Indicates which mode will be used. Support I2C_POLLING_MODE,I2C_INTR_MODE, I2C_DMA_MODE.	
	u32 speed;   //I2C xfer speed, in general, support 100000 (100K), 400000 (400K),1000000 (1000K), 3400000 (3.4M).
	u8 tx_tl;   // I2C TX trigger level,Setting this when xfer mode is set to I2C_INTR_MODE.
	u8 rx_tl;   //I2C RX trigger level, Setting this when xfer mode is set to I2C_INTR_MODE.
	u8 dma_rdlr;  //I2C DMA TX trigger level,Setting this when xfer mode is set to I2C_DMA_MODE.
	u8 dma_tdlr;  // I2C DMA TX trigger level,Setting this when xfer mode is set to I2C_DMA_MODE.
};

/*
 * i2c_xfer_param: Contains some configuration items for I2C transport.
 *
 * For all modes, these parameters should be set: mode, i2c_slave_en, speed,
 * addr_10bit_en, target_dev_addr, addr_len, addr_offset, buffer, len.
 *
 * For DMA mode, dst_msize must be set.
 */
struct i2c_xfer_param {
	
	u8 addr_10bit_en;  //Indicates that the length of the device address is 10 bit(set to 1).Generally, it is 7 bit(set to 0).
	u16 target_dev_addr;  //Represents the device address of the slave device. NOTE: If thedevice 
	                      //address length of the slave device is 10bit, you need to setaddr_10bit_en to 1.
	u8 addr_len;   //Addr_offset (or register addr) length of slave device, generally 1.For some special devices, such as 
	               //at24c64, it needs two bytes torepresent the address offset, and the length is 2.	
	u32 addr_offset;  //Addr_offset (or device chip register addr).
	u8 read_write; // 0==read ; 1 == write;	
	u32 len;  //Length of data, in byte (8 bit)
	u32 *buffer;  //The data (address) you want to transfer.
	
};


 struct snps_i2c_info {
	u32 id;				/* the ID of I2C */
	u32 base_address;			/* the base-address of the I2C */
//	enum IRQ irq_num;			/* the interrupt ID of I2C */
	u32 clk_freq;			/* the clock freq number of I2C */
	u8 fifo_depth;			/* the fifo depth of I2C */
	enum i2c_xfer_mode xfer_mode;	/* I2C transfer mode */

	struct i2c_xfer_param xfer_param;
	u32 i2c_handler_tx_len;
	u32 i2c_handler_rx_len;
};


void snps_i2c_request_irq(struct snps_i2c_info *i2c_info);
void snps_i2c_free_irq(struct snps_i2c_info *i2c_info);   //不使能I2C1_IRQ中断服务函数
void snps_i2c_wait_for_bb(struct snps_i2c_info *i2c_info);  //Waiting for bus busy
u32 snps_i2c_isBusy(struct snps_i2c_info *i2c_info) ;

u32 snps_i2c_isEnabled(struct snps_i2c_info *i2c_info);
void snps_i2c_enable(struct snps_i2c_info *i2c_info);
void snps_i2c_disable(struct snps_i2c_info *i2c_info);
void snps_i2c_close(struct snps_i2c_info *i2c_info);

void snps_i2c_set_addr(struct snps_i2c_info *i2c_info);  //Setting the target slave address
void snps_i2c_slave_set_addr(struct snps_i2c_info *i2c_info, u32 dev);
u8 snps_i2c_slave_config(struct snps_i2c_info *i2c_info,struct i2c_cfg_param *i2c_cfg);  //Init function slave 发送 mode
void snps_i2c_set_speed(struct snps_i2c_info *i2c_info, u32 speed);
u8 snps_i2c_config(struct snps_i2c_info *i2c_info, struct i2c_cfg_param *i2c_cfg);  //snps_i2c_init - Init function

u8 snps_i2c_read(struct snps_i2c_info *i2c_info );
u8 snps_i2c_write(struct snps_i2c_info *i2c_info );  // Writing to I2C FIFO
void snps_i2c_flush_rxfifo(struct snps_i2c_info *i2c_info);  //Flushes the I2C RX FIFO
void snps_i2c_unmaskIrq(struct snps_i2c_info *i2c_info, u32 unmask_irq);
void snps_i2c_write_1(struct snps_i2c_info *i2c_info );
void i2c1_nvic_init(void);
void i2c2_nvic_init(void);

void sample_i2c_t1(void);
void sample_i2c_t2(void);
void sample_i2c_t3(void);
void sample_i2c_t4(void);
void sample_i2c_t5(void);
void sample_i2c_t6(void);
void sample_i2c_t7(void);
void sample_i2c_t8(void);
void sample_i2c_t9(void);
void sample_i2c_t10(void);
void sample_i2c_t11(void);
void sample_i2c_t12(void);
void sample_i2c_t13(void);
void sample_i2c_t14(void);
void sample_i2c_t15(void);
void sample_i2c_t15_1(void);
void sample_i2c_t16(void);
void sample_i2c_t16_1(void);
void sample_i2c_t17(void);
void sample_i2c_t17_1(void);
void sample_i2c_t18(void);
void sample_i2c_t18_1(void);
void sample_i2c_t19(void);
void sample_i2c_t19_1(void);
void sample_i2c_t20(void);
void sample_i2c_t20_1(void);
void sample_i2c_t21(void);
#endif


