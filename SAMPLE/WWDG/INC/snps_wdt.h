/*
 * Header for SNPS WDT driver
 */

#ifndef __SNPS_WDT_H__
#define __SNPS_WDT_H__

#include "sample_wdt.h"
//#include "../CM3/snps_common.h"

#define WDT_BASE_ADDR				WWDG_BASE
#define SNPS_WDT_CLK_FREQ			38		 // Frequency in Mhz, FPGA is 38Mhz

/* registers for SNPS WDT */
#define SNPS_WDT_CR					0x00
/* interrupt mode */
#define SNPS_WDT_INTR_MODE			(0x1 << 1)
#define SNPS_WDT_RESET_MODE			(0x0 << 1)
#define SNPS_WDT_ENABLE				0x1

#define SNPS_WDT_TORR				0x04
#define SNPS_WDT_CCVR				0x08
#define SNPS_WDT_CRR				0x0c
/* WDT counter restart command */
#define COUNTER_RESTART_VALUE		0x76

#define SNPS_WDT_STAT				0x10
#define SNPS_WDT_EIO				0x14
#define SNPS_WDT_PROT_LEVEL			0x1c
#define SNPS_WDT_COMP_PARAM_5		0xe4
#define SNPS_WDT_COMP_PARAM_4		0xe8
#define SNPS_WDT_COMP_PARAM_3		0xec
#define SNPS_WDT_COMP_PARAM_2		0xf0
#define SNPS_WDT_COMP_PARAM_1		0xf4
#define SNPS_WDT_COMP_VERSION		0xf8
#define SNPS_WDT_COMP_TYPE			0xfc

/* SNPS_WDT_RELEASE_ADDR bit difine */
#define SNPS_WDT_IS_RST				BIT(0)
#define SNPS_WDT_GLB_EN				BIT(4)

/* SNPS_WDT_CTRL_GLOBAL bit difine */
#define TOP_WDT_RST_PMC_EN			BIT(0)
#define TOP_WDT_RST_AUDIO_EN		BIT(4)
#define TOP_WDT_RST_MEM_EN			BIT(8)
#define WDT_RST_PMC_BUS_EN			BIT(12)
#define WDT_RST_PMC_PCK_EN			BIT(13)
#define WDT_RST_PMC_GPC_EN			BIT(14)


/* Timeout period for initialization */
enum snps_wdt_period {
	USER0_OR_64K,
	USER1_OR_128K,
	USER2_OR_256K,
	USER3_OR_512K,
	USER4_OR_1M,
	USER5_OR_2M,
	USER6_OR_4M,
	USER7_OR_8M,
	USER8_OR_16M,
	USER9_OR_32M,
	USER10_OR_64M,
	USER11_OR_128M,
	USER12_OR_256M,
	USER13_OR_512M,
	USER14_OR_1G,
	USER15_OR_2G
};

/* Reset pulse length definitions */
enum snps_wdt_reset_pulse_length {
	PCLK_CYCLES_2,
	PCLK_CYCLES_4,
	PCLK_CYCLES_8,
	PCLK_CYCLES_16,
	PCLK_CYCLES_32,
	PCLK_CYCLES_64,
	PCLK_CYCLES_128,
	PCLK_CYCLES_256
};

struct wdt_cfg_param {
	/* Represents the timeout in us. */
	u32 timeout_us;
	/* Reset extension time, in ns. */
	u32 rst_time_ns;
	/* interrupt mode 0 - interrupt disable; 1 - interrupt enable*/
	u8 intr_en;
};


void snps_wdt_set_timeout(u32 timeout_us);

u32 snps_wdt_get_timeout(void);

u32 snps_get_counter(void)	  ;

void snps_wdt_restart(void)  ;

void snps_wdt_intr_clear(void) ;

bool snps_wdt_isenabled(void);

void snps_wdt_disable(void)	  ;

void snps_wdt_enable(void)	 ;

u8 snps_wdt_init(struct wdt_cfg_param *param);

#define true    TRUE
#define false   FALSE

#endif
