#include "sys.h"
#include "delay.h"
#include "sample_efc.h"
#include "stm32f10x.h"
  
u32 page_buff[64];
/****************************************宏定义***************************************/
//C文件内使用宏定义



/****************************************枚举***************************************/
//C文件内使用枚举



/****************************************结构体***************************************/
//C文件内使用结构体


//功能函数
void sample_efc_nvic_cfg(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = 3;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);
}

void sample_efc_it_cfg(u32 reg)
{
	EFC->fls_conf &= ~(3 << 0);
	EFC->fls_conf |= reg;
}

void sample_set_opm(T_eEFC_OP op_mod,u32 page_num)
{
	u32 temp;
	temp = EFC->fls_ctrl;
	temp &= 0x0f;   		//高位清零
	temp |= (0x5a<<24) | op_mod | (page_num << 8);
	//printf("temp:0x%x\r\n",temp);
	EFC->fls_ctrl = temp;
}

void sample_set_pwrm(T_eEFC_PM pm_mod)
{
	u32 temp;
	temp = EFC->fls_ctrl;
	temp &= ~0xf;
	temp |= (0x5a<<24) | pm_mod;
	//printf("temp:0x%x\r\n",temp);
	EFC->fls_ctrl = temp;
}

void sample_unlock_ctrl(void)
{
	EFC->fls_key0 = FLS_KEY1;
	EFC->fls_key0 = FLS_KEY2;
}

void sample_unlock_opt(void)
{
	EFC->fls_key1 = FLS_KEY1;
	EFC->fls_key1 = FLS_KEY2;
}

void sample_timing_cfg(u32 hv_time)
{

#if 0
	//TIM0  0x14:0x10001
	EFC->Taw = ((TAW/BUS_PRD) + 1);    //5:0
	EFC->Tawl = ((TAWL/BUS_PRD) + 1);   //21:16

	//TIM1 0x18:0x10001
	EFC->Tpw = (TPW/BUS_PRD + 1);    //4:0
	EFC->Tpwl = (TPWL/BUS_PRD + 1);   //20:16

	//TIM2 0x1c:0x3640e
	//EFC->Tpe=TPE_PP/BUS_PRD;    //19:0
	EFC->Tpe=TPE_E/BUS_PRD;	//19:0
	//EFC->Tpe=TPE_P/BUS_PRD;	//19:0
	
	//TIM3  0x20:0x22b00de
	EFC->Ts0 = TS0/BUS_PRD;    
	EFC->Ts1 = TS1/BUS_PRD;

	//TIM4 0x24:0x8ae00b9
	EFC->Ts2p = TS2P/BUS_PRD;    
	EFC->Tps3 = TPS3/BUS_PRD;

	//TIM5  0x28:0xb9
	EFC->Ts3 = TS3/BUS_PRD;    //19:0

	//TIM6 0x2c:0x6f006f
	EFC->Tpd1f = TPD1F/BUS_PRD;    //8:0
	EFC->Tpd2f = TPD2F/BUS_PRD;   //24:16

	//TIM7 0x30:0x300de
	EFC->Tpd2s = TPD2S/BUS_PRD;    //9:0
	EFC->Trw = TRW/BUS_PRD;   //25:16

	//TIM8 0x34:0x90ad
	
	EFC->Trcs = TRCS/BUS_PRD;    //19:0
#endif
	write_ahb32(EFC_BASE+0x14, 0x20002);
	write_ahb32(EFC_BASE+0x18, 0x10001);
	
	write_ahb32(EFC_BASE+0x1c, hv_time);
	write_ahb32(EFC_BASE+0x20, 0x22b00de);
	write_ahb32(EFC_BASE+0x24, 0x8ae00b9);
	write_ahb32(EFC_BASE+0x28, 0xb9);
	write_ahb32(EFC_BASE+0x2c, 0x6f006f);
	write_ahb32(EFC_BASE+0x30, 0x400de);
	write_ahb32(EFC_BASE+0x34, 0x25);
}


void sample_trm_rld(void)
{
	u32 temp;
	//进入读模式前OP_MODE不能为读写擦操作
	//enter reset mode
	sample_set_pwrm(eMOD_RST);

	printf("ctrl:0x%x\r\n",EFC->fls_ctrl);
	EFC->fls_stat = EFC->fls_stat;
	//enter TRM RLD mode
	sample_set_pwrm(eMOD_SLP);
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位

	sample_set_pwrm(eMOD_IDLE);
	while((EFC->fls_stat & CMD_DONE) == 0);//IDLE状态CMD_DONE不会置位
	
	printf("fls_stat2:0x%x\r\n",EFC->fls_stat);
	
	//set op_mode
	sample_set_opm(eTRM_DATA_RLD,0);
	//printf("fls_stat3:0x%x\r\n",EFC->fls_stat);
	while((EFC->fls_stat & CMD_DONE) != 1);//IDLE状态CMD_DONE不会置位
	printf("fls_stat3:0x%x\r\n", EFC->fls_stat);
	printf("fls_stat4:0x%x\r\n",EFC->fls_stat);

	//is ldtpass ==1
	if(EFC->fls_trim & RLD_PASS)
	{
		printf("Trim reload success\r\n");
	}
	else
	{
		printf("Trim reload fail\r\n");
	}
	
}

void sample_read_cnf(u32 mreg)
{
	EFC->fls_conf &= ~(7 << 2);
	EFC->fls_conf |= mreg;
}

u32 sample_word_read(u32 mreg)
{
	sample_set_opm(eNM_MODE,0);
	return read_ahb32(mreg);
}

void sample_page_read(u32 page_num)
{
	u32 i,addr = FLSAH_BASE;
	sample_set_opm(eNM_MODE,page_num);
	addr |= page_num << 8;
	
	for(i=0;i<64;i++)
	{
		//page_buff[i] = sampel_word_read(addr+(i*4));
	}
}


//pre_program
void op_pre_pgm(u32 page_num)
{
	while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
	
	sample_set_opm(ePRE_PGM,page_num);
	
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位
}

//pgm
void op_pgm(T_eEFC_OP pgm,u32 page_num)
{
	while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
	
	sample_set_opm(pgm,page_num);
	printf("PGM FLS_STAT:0x%x\r\n",EFC->fls_stat);
	
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位
}

//erase
void op_erase(T_eEFC_OP er,u32 page_num)
{
	while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
	
	sample_set_opm(er,page_num);
	printf("ER FLS_STAT:0x%x\r\n",EFC->fls_stat);
	
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位
}

//Clear page latches
void op_page_clr(u32 page_num)
{
	while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
	
	sample_set_opm(eCLR_PAGE_LT,page_num);
	
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位
}

//page write all
void op_page_write_all(u32 page_num,u32 byte)
{
	while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
	
	sample_set_opm(eWR_PAGE_ALL,page_num);
	write_ahb32(FLSAH_BASE+ (page_num << 8),byte);
	
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位
}

void op_page_write_byte(u32 addr,u32 byte)   //addr地址+FLSAH_BASE地址
{
	u32 i;
	while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
	
	sample_set_opm(eNM_MODE,addr >> 8);
	
	for(i = 0;i < 64;i++)
	{
		write_ahb32(FLSAH_BASE + addr+(i*4),byte);
	}
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位
}


//ERASE
void sample_page_er(u32 page_num)
{
	//pre_program
	sample_timing_cfg(TPE_PP/BUS_PRD);
	op_pre_pgm(page_num);

	//pgm
	op_pgm(ePAGE_PGM,page_num);
	
	//ER
	sample_timing_cfg(TPE_E/BUS_PRD);
	op_erase(ePAGE_ER, page_num);
}

void sample_sector_er(u32 sct_num)
{
	//prep
	sample_timing_cfg(TPE_PP/BUS_PRD);
	op_pre_pgm(sct_num << 3);
	
	//pgm
	op_pgm(eSECTOR_PGM,sct_num << 3);

	//ER
	sample_timing_cfg(TPE_E/BUS_PRD);
	op_erase(eSECTOR_ER, sct_num << 3);
}

void sample_bank_er(u32 bank_num)
{
	//prep
	sample_timing_cfg((TPE_PP+200)/BUS_PRD);
	op_pre_pgm(bank_num << 8);
	
	//pgm
	op_pgm(eBANK_PGM,bank_num << 8);

	//ER
	sample_timing_cfg((TPE_E+200)/BUS_PRD);
	op_erase(eBANK_ER, bank_num << 8);
}

void sample_bulk_er(void)
{
	//prep
	sample_timing_cfg((TPE_PP+200)/BUS_PRD);
	op_pre_pgm(0 << 8);
	
	//pgm
	op_pgm(eBANK_PGM,0 << 8);

	//prep
	op_pre_pgm(1 << 8);
	
	//pgm
	op_pgm(eBANK_PGM,1 << 8);

	//ER
	sample_timing_cfg((TPE_E+200)/BUS_PRD);
	op_erase(eBULK_ER, 0);
}

void sample_page_pgm(u32 page_num,u32 byte)
{
	//pre+er
	sample_page_er(page_num);
	
	//CLEAR PAGE BUFFER
	op_page_clr(page_num);
	
#ifdef PAGE_BUFF_W2W
	op_page_write_byte((page_num<<8),byte);
	//write page byte to byte 
#else
	//write page latch all
	op_page_write_all(page_num,byte);
#endif

	//PGM operate
	sample_timing_cfg(TPE_P/BUS_PRD);
	op_pgm(ePAGE_PGM,page_num);
}

void sample_sector_pgm(u32 sector_num,u32 byte)
{
	//pre+er
	sample_sector_er(sector_num);

	//确认单个PAGE BUFF是否可以写整个SECTOR
	//CLEAR PAGE BUFFER
	op_page_clr(sector_num << 3);

	//write page latch all
	op_page_write_all(sector_num << 3,byte);

	//PGM operate
	sample_timing_cfg(TPE_P/BUS_PRD);
	op_pgm(eSECTOR_PGM,sector_num << 3);
}

void sample_bank_pgm(u32 bank_num,u32 byte)
{
	//pre+er
	sample_bank_er(bank_num);

	//确认单个PAGE BUFF是否可以写整个SECTOR
	//CLEAR PAGE BUFFER
	op_page_clr(bank_num << 8);

	//write page latch all
	op_page_write_all(bank_num << 8,byte);

	//PGM operate
	op_pgm(eBANK_PGM,bank_num << 8);
}

void sample_hlaf_bank_pgm(u32 bank_num,u32 chk,u32 byte)
{
	//pre+er
	sample_bank_er(bank_num);

	//确认单个PAGE BUFF是否可以写整个SECTOR
	//CLEAR PAGE BUFFER
	op_page_clr(bank_num << 8);

	//write page latch all
	op_page_write_all(bank_num << 8,byte);

	//PGM operate
	if(chk == 0)
		op_pgm(eBANK_PGM_HALF,bank_num << 8);
	else
		op_pgm(eBANK_PGM_HALF,(bank_num << 8)+1);
}

void sample_opt_pgm(OB_TypeDef opt)
{
	u32 i,temp[4] = {0};

	temp[0] = opt.USER | ((~opt.USER << 8)&0xFF00);
	temp[0] <<= 16;
	temp[0] |= opt.RDP | ((~opt.RDP << 8)&0xFF00);
	
	temp[1] = opt.Data1 | ((~opt.Data1 << 8)&0xFF00);
	temp[1] <<= 16;
	temp[1] |= opt.Data0 | ((~opt.Data0 << 8)&0xFF00);
	
	temp[2] = opt.WRP1 | ((~opt.WRP1 << 8)&0xFF00);
	temp[2] <<= 16;
	temp[2] |= opt.WRP0 | ((~opt.WRP0 << 8)&0xFF00);
	
	temp[3] = opt.WRP3 | ((~opt.WRP3 << 8)&0xFF00);
	temp[3] <<= 16;
	temp[3] |= opt.WRP2 | ((~opt.WRP2 << 8)&0xFF00);
	
	for(i=0;i<4;i++)
	{
		printf("temp[%d]:0x%x\r\n",i,temp[i]);
	}

	//pre+er
	sample_page_er(513); //擦除OPT PAGE
	
	//CLEAR PAGE BUFFER
	op_page_clr(513);
	while((EFC->fls_stat & FLS_BUSY) != 0);//等待BUSY释放
	
	sample_set_opm(eNM_MODE,513);
	
	for(i = 0;i < 4;i++)
	{
		write_ahb32(FLSAH_BASE + 0x20100+(i*4),temp[i]);
	}
	for(i = 4; i < 64;i++)
	{
		write_ahb32(FLSAH_BASE + 0x20100+(i*4),0);
	}
	while((EFC->fls_stat & CMD_DONE) == 0);//等待CMD_DONE置位

	//PGM operate
	sample_timing_cfg(TPE_P/BUS_PRD);
	op_pgm(ePAGE_PGM,513);
}

//Clear status reg
void sample_stat_clr(void)
{
	u32 temp;
	
	printf("fls_stat:0x%x\r\n",EFC->fls_stat); //读取FLASH状态
	temp = EFC->fls_stat;
	EFC->fls_stat = temp;
}


//verify
void sample_page_vrf(u32 page_num, u32 size)
{
	u32 i,j;
	
	//verify 
	for(i = 0;i < size;i++)
	{
		printf("%d:----------------------\r\n",i+page_num);
		
		for(j=0;j<64;j++)
		{
			printf("0x%x:read word:0x%x\r\n",j*4,sample_word_read((FLSAH_BASE + ((i+ page_num)<<8))+4*j));
		}
	}


}

void EFC_IRQHandler(void)
{
	u32 temp;
	printf("enter EFC_IRQ\r\n");
	printf("PGM FLS_STAT:0x%x\r\n",EFC->fls_stat);
	
	//清除中断
	temp = EFC->fls_stat;
	EFC->fls_stat =temp;
	printf("PGM FLS_STAT:0x%x\r\n",EFC->fls_stat);
}
