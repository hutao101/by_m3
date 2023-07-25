#include "malloc.h"	    

//内存池(32字节对齐)
__align(32) u8 mem1base[MEM1_MAX_SIZE];													//内部SRAM内存池
//__align(32) u8 mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000)));					//外部SRAM内存池
//内存管理表
u16 mem1mapbase[MEM1_ALLOC_TABLE_SIZE];													//内部SRAM内存池MAP
//u16 mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0X68000000+MEM2_MAX_SIZE)));	//外部SRAM内存池MAP
//内存管理参数	   
const u32 memtblsize[SRAMBANK]={MEM1_ALLOC_TABLE_SIZE};			//内存表大小
const u32 memblksize[SRAMBANK]={MEM1_BLOCK_SIZE};				//内存分块大小
const u32 memsize[SRAMBANK]={MEM1_MAX_SIZE};					//内存总大小


//内存管理控制器
struct _m_mallco_dev mallco_dev=
{
	mem_init,				    //内存初始化
	mem_perused,				//内存使用率
	mem1base,			//内存池
	mem1mapbase,	//内存管理状态表
	0,  		 				//内存管理未就绪
};

//复制内存
//*des:目的地址
//*src:源地址
//n:需要复制的内存长度(字节为单位)
void mem_cpy(void *des,void *src,u32 n)  
{  
    u8 *xdes=des;
	u8 *xsrc=src; 
    while(n--)*xdes++=*xsrc++;  
}  
//设置内存
//*s:内存首地址
//c :要设置的值
//count:需要设置的内存大小(字节为单位)
void mem_set(void *s,u8 c,u32 count)  
{  
    u8 *xs = s;  
    while(count--)*xs++=c;  
}	   
//内存管理初始化  
void mem_init(void)  
{  
    mem_set(mallco_dev.memmap[0], 0,memtblsize[0]*2);//内存状态表数据清零  
	mem_set(mallco_dev.membase[0], 0,memsize[0]);	//内存池所有数据清零  
	mallco_dev.memrdy[0]=1;								//内存管理初始化OK  
}  
//获取内存使用率
//返回值:使用率(0~100)
u8 mem_perused(void)  
{  
    u32 used=0;  
    u32 i;  
    for(i=0;i<memtblsize[0];i++)  
    {  
        if(mallco_dev.memmap[0][i])used++; 
    } 
    return (used*100)/(memtblsize[0]);  
}  
//内存分配(内部调用)
//size:要分配的内存大小(字节)
//返回值:0XFFFFFFFF,代表错误;其他,内存偏移地址 
u32 mem_malloc(u32 size)  
{  
    signed long offset=0;  
    u32 nmemb;	//需要的内存块数  
	u32 cmemb=0;//连续空内存块数
    u32 i;  
    if(!mallco_dev.memrdy[0])mallco_dev.init();//未初始化,先执行初始化 
    if(size==0)return 0XFFFFFFFF;//不需要分配
    nmemb=size/memblksize[0];  	//获取需要分配的连续内存块数
    if(size%memblksize[0])nmemb++;  
    for(offset=memtblsize[0]-1;offset>=0;offset--)//搜索整个内存控制区  
    {     
		if(!mallco_dev.memmap[0][offset])cmemb++;//连续空内存块数增加
		else cmemb=0;								//连续内存块清零
		if(cmemb==nmemb)							//找到了连续nmemb个空内存块
		{
            for(i=0;i<nmemb;i++)  					//标注内存块非空 
            {  
                mallco_dev.memmap[0][offset+i]=nmemb;  
            }  
            return (offset*memblksize[0]);//返回偏移地址  
		}
    }  
    return 0XFFFFFFFF;//未找到符合分配条件的内存块  
}  
//释放内存(内部调用) 
//offset:内存地址偏移
//返回值:0,释放成功;1,释放失败;  
u8 mem_free(u32 offset)  
{  
    int i;  
    if(!mallco_dev.memrdy[0])//未初始化,先执行初始化
	{
		mallco_dev.init();    
        return 1;//未初始化  
    }  
    if(offset<memsize[0])//偏移在内存池内. 
    {  
        int index=offset/memblksize[0];			//偏移所在内存块号码  
        int nmemb=mallco_dev.memmap[0][index];	//内存块数量
        for(i=0;i<nmemb;i++)  						//内存块清零
        {  
            mallco_dev.memmap[0][index+i]=0;  
        }  
        return 0;  
    }else return 2;//偏移超区了.  
}  
//释放内存(外部调用) 
//ptr:内存首地址 
void by_free(void *ptr)  
{  
	u32 offset;   
	if(ptr==NULL)return;//地址为0.  
 	offset=(u32)ptr-(u32)mallco_dev.membase[0];     
    mem_free(offset);	//释放内存      
}  
//分配内存(外部调用)
//size:内存大小(字节)
//返回值:分配到的内存首地址.
void *by_malloc(u32 size)  
{  
    u32 offset;   
	offset=mem_malloc(size);  	   	 	   
    if(offset==0XFFFFFFFF)return NULL;  
    else return (void*)((u32)mallco_dev.membase[0]+offset);  
}  
//重新分配内存(外部调用)
//*ptr:旧内存首地址
//size:要分配的内存大小(字节)
//返回值:新分配到的内存首地址.
void *by_realloc(void *ptr,u32 size)  
{  
    u32 offset;    
    offset=mem_malloc(size);   	
    if(offset==0XFFFFFFFF)return NULL;     
    else  
    {  									   
	    mem_cpy((void*)((u32)mallco_dev.membase[0]+offset),ptr,size);	//拷贝旧内存内容到新内存   
        by_free(ptr);  											  		//释放旧内存
        return (void*)((u32)mallco_dev.membase[0]+offset);  				//返回新内存首地址
    }  
}












