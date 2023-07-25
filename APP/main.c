#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "unity.h"
#include "malloc.h"

const unsigned char boot_s[25]={"MCUF103 Boot UP!\r\n"};

 int main(void)
 {
    uart_init(115200);

	while(1)
	{

	}
 }

