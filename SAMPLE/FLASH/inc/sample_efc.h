#ifndef __SAMPLE_EFC_H
#define __SAMPLE_EFC_H
#include "stdio.h"	
#include "sys.h" 
#include "sample_dbgmcu_conf.h" 


//#define PAGE_BUFF_W2W     

//EFC BUS CLK
#define BUS_CLK 36   //MHZ
#define BUS_PRD (1000/BUS_CLK)   //MHZ


//key value
#define FLS_KEY1       0x45670123
#define FLS_KEY2       0xcdef89ab

#define BANK_ODD       1
#define BANK_EVEN      0


typedef struct
{
	//只能32位访问
	
	__IO uint32_t fls_key0;    //FLS_KEY0
	__IO uint32_t fls_key1;
	__IO uint32_t fls_conf;
	__IO uint32_t fls_ctrl;
	__IO uint32_t fls_stat;

	//TIM0 0x14   //0x10001
	__IO uint32_t Taw:6;    //5:0
	__IO uint32_t rev0:10;  //15:6
	__IO uint32_t Tawl:6;   //21:16
	__IO uint32_t rev1:10;  //31:22

	//TIM1 0x18   //0x10001
	__IO uint32_t Tpw:5;    //4:0
	__IO uint32_t rev2:11;  //15:5
	__IO uint32_t Tpwl:5;   //20:16
	__IO uint32_t rev3:11;  //31:21

	//TIM2 0x1c  //0x3640e
	__IO uint32_t Tpe:20;    //19:0
	__IO uint32_t rev4:12;  //31:20

	//TIM3 0x20	//0x00de
	__IO uint16_t Ts0;    
	__IO uint16_t Ts1;

	//TIM4 0x24
	__IO uint16_t Ts2p;    
	__IO uint16_t Tps3;

	//TIM5 0x28 
	__IO uint32_t Ts3:20;    //19:0
	__IO uint32_t rev5:12;  //31:20

	//TIM6 0x2c
	__IO uint32_t Tpd1f:9;    //8:0
	__IO uint32_t rev6:7;    //15:9
	__IO uint32_t Tpd2f:9;   //24:16
	__IO uint32_t rev7:7;  //31:25

	//TIM7 0x30
	__IO uint32_t Tpd2s:10;    //9:0
	__IO uint32_t rev8:6;    //15:10
	__IO uint32_t Trw:10;   //25:16
	__IO uint32_t rev9:6;  //31:26

	//TIM8 0x34
	__IO uint32_t Trcs:20;    //19:0
	__IO uint32_t rev10:12;  //31:20

	__IO uint32_t fls_trim;
	__IO uint32_t fls_opt0;
	__IO uint32_t fls_opt1;
}EFC_InitTypeDef;


//寄存器地址定义
#define FLSAH_BASE  0x10000000
#define EFC_BASE    0x40002000
#define EFC         ((EFC_InitTypeDef *) EFC_BASE)


#if 0
#define FLS_KEY0    EFC_BASE+0x00
#define FLS_KEY1    EFC_BASE+0x04
#define FLS_CONF    EFC_BASE+0x08
#define FLS_CTRL    EFC_BASE+0x0C
#define FLS_STAT    EFC_BASE+0x10
#define FLS_TIM0    EFC_BASE+0x14
#define FLS_TIM1    EFC_BASE+0x18
#define FLS_TIM2    EFC_BASE+0x1C
#define FLS_TIM3    EFC_BASE+0x20
#define FLS_TIM4    EFC_BASE+0x24
#define FLS_TIM5    EFC_BASE+0x28
#define FLS_TIM6    EFC_BASE+0x2C
#define FLS_TIM7    EFC_BASE+0x30
#define FLS_TIM8    EFC_BASE+0x34
#define FLS_TRIM    EFC_BASE+0x38
#define FLS_OPT0    EFC_BASE+0x3C
#define FLS_OPT1    EFC_BASE+0x40
#endif

//寄存器位定义

//FLS_CFG
#define FLS_LOCK      1<<31
#define OPT_LOCK      1<<30
#define BUS_ERR       1<<5
#define DIS_PREF      1<<4
#define RD_TURBO      1<<3
#define RD_NOWAIT     1<<2
#define ERR_INTEN     1<<1
#define CMD_INTEN     1<<0

//FLS_CTRL
#define PAGE_NUM      0x3ff<<8
#define OP_MODE       0xf<<4
#define FLS_SLEEP     1<<3
#define FLS_RESET     1<<2
#define FLS_PD2       1<<1
#define FLS_PD1       1<<0


//FLS_STAT
#define OP_ERR         1<<11
#define RD_PORT_ERR    1<<10
#define PGM_PORT_ERR   1<<9
#define NM_LOCK_ERR    1<<8
#define SM_LOCK_ERR    1<<7
#define PWR_MODE_ERR   1<<6
#define OP_MODE_ERR    1<<5
#define ARER_ERR       1<<4
#define UNLOCK_FAIL    1<<3
#define RD_PROT        1<<2
#define FLS_BUSY       1<<1
#define CMD_DONE       1<<0

//FLS_TRIM
#define RLD_PASS       1<<16
#define TRIM_DATA      0xFFFF<<0

//枚举类型

//op_mode 枚举类型
typedef enum
{
	eNM_MODE       = 0x0 << 4,
	eCLR_PAGE_LT   = 0x1 << 4,
	eWR_PAGE_ALL   = 0x2 << 4,
	eTRM_DATA_RLD  = 0x4 << 4,
	ePRE_PGM       = 0x7 << 4,
	ePAGE_ER	   = 0x8 << 4,
	eSECTOR_ER     = 0x9 << 4,
	eBANK_ER       = 0xa << 4,
	eBULK_ER       = 0xb << 4,
	ePAGE_PGM      = 0xc << 4,
	eSECTOR_PGM    = 0xd << 4,
	eBANK_PGM_HALF = 0xe << 4,
	eBANK_PGM 	   = 0xf << 4,
} T_eEFC_OP;
	

typedef enum
{
	eMOD_PD1       = 0x3,
	eMOD_PD2       = 0x2,
	eMOD_RST   	   = 0x4,
	eMOD_TRM_RLD   = 0x0,
	eMOD_SLP       = 0x8,
	eMOD_IDLE      = 0x0,
	eMOD_ACTIVE    = 0x0,
} T_eEFC_PM;

//TIMING
#define TAA     50
#define TCY_R   50   //大于Taa
#define TCY_RLD 100
#define TDOH    0
#define TAW     10
#define TAWL    10
#define TAS     2	
#define TAH     2	
#define TOZ     2	
#define TOE     2	
#define TWCY    40	
#define TPW     16	
#define TPWL    16	
#define TPAS    3	
#define TPAH    3	
#define TDS     16	
#define TDH     3		
#define TS1     15000	
#define TS2P    5000	
#define TBW     100*TCY_RLD	
#define TPE_E   6000*1000	
#define TPE_P   2000*1000	
#define TPE_PP  200*1000
#define TPS3    60*1000
#define TS3     5*1000	
#define TS0     6*1000	
#define TPD12   0*1000	
#define TPD1F   3*1000	
#define TPD2F   3*1000	
#define TPD2S   6*1000
#define TRW     100	
#define TRCS    1000	
#define TACTV   6000	
#define TACTV_TURBO   3000	

//函数声明
void op_page_clr(u32 page_num);
void op_page_write_byte(u32 addr,u32 byte);
void op_pgm(T_eEFC_OP pgm,u32 page_num);

void sample_efc_nvic_cfg(void);
void sample_efc_it_cfg(u32 reg);

void sample_set_opm(T_eEFC_OP op_mod,u32 page_num);
void sample_set_pwrm(T_eEFC_PM pm_mod);

void sample_timing_cfg(u32 hv_time);
void sample_trm_rld(void);

void sample_unlock_ctrl(void);
void sample_unlock_opt(void);

//read
u32 sample_word_read(u32 mreg);
void sample_page_read(u32 page_num);
void sample_read_cnf(u32 mreg);

//erase
void sample_page_er(u32 page_num);
void sample_sector_er(u32 sct_num);
void sample_bank_er(u32 bank_num);
void sample_bulk_er(void);

//PGM
void sample_page_pgm(u32 page_num,u32 byte);
void sample_sector_pgm(u32 sector_num,u32 byte);
void sample_bank_pgm(u32 bank_num,u32 byte);
void sample_hlaf_bank_pgm(u32 bank_num,u32 chk,u32 byte);
void sample_opt_pgm(OB_TypeDef opt);

//status reg clr
void sample_stat_clr(void);

//verify
void sample_page_vrf(u32 page_num, u32 size);

//case
void sample_efc_t1(void);
void sample_efc_t1_1(void);
void sample_efc_t2(void);
void sample_efc_t3(void);
void sample_efc_t4(void);
void sample_efc_t5(void);
void sample_efc_t6(void);
void sample_efc_t7(void);
void sample_efc_t8(void);
void sample_efc_t9(void);
void sample_efc_t10(void);
void sample_efc_t11(void);
void sample_efc_t12(void);
void sample_efc_t13(void);
void sample_efc_t14(void);
void sample_efc_t15(void);
void sample_efc_t16(void);
void sample_efc_t17(void);
void sample_efc_t18(void);
void sample_efc_t19(void);
void sample_efc_t20(void);
void sample_efc_t21(void);
void sample_efc_t22(void);
void sample_efc_t23(void);
void sample_efc_t24(void);
void sample_efc_t25(void);
void sample_efc_t26(void);
void sample_efc_t27(void);
void sample_efc_t28(void);
void sample_efc_t29(void);
void sample_efc_t30(void);
void sample_efc_t31(void);
void sample_efc_t32(void);
void sample_efc_t33(void);
void sample_efc_t34(void);
void sample_efc_t35(void);
void sample_efc_t36(void);
void sample_efc_t37(void);
void sample_efc_t38(void);
void sample_efc_t39(void);
void sample_efc_t40(void);
void sample_efc_t41(void);
void sample_efc_t42(void);
void sample_efc_t43(void);
void sample_efc_t44(void);
void sample_efc_t45(void);
void sample_efc_t46(void);
void sample_efc_t47(void);
void sample_efc_t48(void);
void sample_efc_t49(void);


#endif




