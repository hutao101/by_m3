#ifndef _SPI_H_
#define _SPI_H_

#include "stm32f10x.h"
#include "sample_spi.h"

#define SPI2_MASTER_BASE             (APB1PERIPH_BASE + 0x1C00)
#define SPI2_SLAVE_BASE        (APB1PERIPH_BASE + 0x3400)

//#define SPI1_MASTER_BASE             (APB2PERIPH_BASE + 0x1400)
//#define SPI1_SLAVE_BASE             (APB2PERIPH_BASE + 0x3000)

/*SPI FIFO_DEPTH = 32	*/
#define FIFO_DEPTH		32

/* GPIO base*/
#define GPIO_BASE 0x40010000

#define GPIO_SWPORTA_DR 0x0
#define GPIO_SWPORTA_DDR 0x4
#define GPIO_EXT_PORTA 0x50

#define GPIO_SWPORTB_DDR 0x10
#define GPIO_EXT_PORTB   0x54
#define GPIO_SWPORTB_DR  0xc

/*SPI register offset*/
#define DW_SPI_CTRL0		0x00
/*x : 0x3 - 0xf, x offent set 0x7, 0xf*/
#define SPI_DFS16(x)		((x-1) << 0)
#define SPI_DFS_16_MASK		0xf
#define SPI_FRF_SPI			(0 << 4)
#define SPI_FRF_SSP			(1 << 4)
#define SPI_FRF_MICROWIRE	(2 << 4)
#define SPI_FRF_RESV		(3 << 4)
#define SPI_MODE0			(0 << 6)
#define SPI_MODE1			(1 << 6)
#define SPI_MODE2			(2 << 6)
#define SPI_MODE3			(3 << 6)
#define SPI_TMOD_MASK		(3 << 8)
#define SPI_TMOD_TR			(0 << 8)
#define SPI_TMOD_TO			(1 << 8)
#define SPI_TMOD_RO			(2 << 8)
#define SPI_TMOD_EPROMREAD	(3 << 8)
#define SPI_SLAVE_DISABLE	(1 << 10)
#define SPI_SRL_ENABLE		(1 << 11)
/*x offent set 8, 16, 32*/
#define SPI_SPI_FRF_MASK	(3 << 21)
#define SPI_STD_SPI_FRF		(0 << 21)
#define SPI_DUAL_SPI_FRF	(1 << 21)
#define SPI_QUAD_SPI_FRF	(2 << 21)
#define SPI_OCTAL_SPI_FRF	(3 << 21)

/*
 *Number of Data Frames,
 *This register is only valid when TMOD set to SPI_TMOD_RO and
 *SPI_TMOD_EPROMREAD mode
 */
#define DW_SPI_CTRL1		0x04

/*Enables serial transfer*/
#define DW_SPI_SSIENR		0x08
#define SPI_SSI_EN			(1)

#define DW_SPI_MWCR			0x0c
/*Slave Enable Register has the same function as CS*/
#define DW_SPI_SER			0x10
#define SPI_SLV_SEL			(1 << 0)
#define DW_SPI_BAUDR		0x14
/*trigger level*/
#define DW_SPI_TXFTLR		0x18
#define DW_SPI_RXFTLR		0x1c
/*contains the number of valid data entries in the FIFO memory.*/
#define DW_SPI_TXFLR		0x20
#define DW_SPI_RXFLR		0x24
/*current transfer, FIFO and any transmission/reception errors status*/
#define DW_SPI_SR			0x28
#define SR_BUSY				BIT(0)
#define SR_TF_NOT_FULL		BIT(1)
#define SR_TF_EMPT			BIT(2)
#define SR_RF_NOT_EMPT		BIT(3)
#define SR_RF_FULL			BIT(4)
#define SR_TX_ERR			BIT(5)
#define SR_DCOL				BIT(6)
/*interrupt-related registers*/
#define DW_SPI_IMR			0x2c
/*reports the status of interrupts after they have been masked.*/
#define DW_SPI_ISR			0x30
#define DW_SPI_RISR			0x34
/*Bit fields in ISR, IMR, RISR, 6 bits*/
#define SPI_INT_TXEI		BIT(0)
#define SPI_INT_TXOI		BIT(1)
#define SPI_INT_RXUI		BIT(2)
#define SPI_INT_RXOI		BIT(3)
#define SPI_INT_RXFI		BIT(4)
#define SPI_INT_MSTI		BIT(5)
/*interrupt clear registers*/
#define DW_SPI_TXOICR		0x38
#define DW_SPI_RXOICR		0x3c
#define DW_SPI_RXUICR		0x40
#define DW_SPI_MSTICR		0x44
/*This register is set if any of the interrupts below are active*/
#define DW_SPI_ICR			0x48

/*DMA related registers*/
#define DW_SPI_DMACR		0x4c
#define TDMAE_EN			0x2
#define RDMAE_EN			0x1
#define DW_SPI_DMATDLR		0x50
#define DW_SPI_DMARDLR		0x54
/*Identification Register*/
#define DW_SPI_IDR			0x58
/*coreKit version ID Register*/
#define DW_SPI_VERSION		0x5c
/*Data Register*/
#define DW_SPI_DR			0x60

#define DW_SPI_RX_SAMPLE_DLY 0xf0

#define DW_SPI_CTRLR0		0xf4

#define DW_SPI_TXD_DRIVE_EDGE  0xf8

#define DW_SPI_RSVD    0xfc

/*transmit instruction/address mode*/
#define SPI_TRANS_TYPE_0	(0)
#define SPI_TRANS_TYPE_1	(1)
#define SPI_TRANS_TYPE_2	(2)
/* x * 4-bit Address Width, x : 0 - 0xf*/
#define SPI_ADDR_L(x)		(x << 2)
/*x : 0 - 3*/
#define SPI_INST_L(x)		(x << 8)
/*x : 0 - 0xfffff*/
#define SPI_WAIT_CYCS(x)	(x << 11)

#define SPI_PLCK			38000000
#define SNPS_SPI_BAUD		500000//0//500khz--ok

void snps_spi_disable(u32 base);
void snps_spi_enable(u32 base);

void sample_ready_key_init(void);

void printf_reg(u32 addr_base);

extern u8 ready_key_flag;
extern u8 RXF_flag ;
extern u8 TXO_flag ;
extern u8 RXO_flag ;
#endif
