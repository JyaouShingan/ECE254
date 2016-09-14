#include <LPC17xx.h>
#include <RTL.h>
#include <stdio.h>
#include "uart_polling.h"
#include "../../RTX_CM3/INC/RTL_ext.h"

__task void task1()
{
	unsigned int i = 0;
	
	for(;;i++)
	{
		printf("Task1: %d\n", i);
		if(i > 10) os_tsk_delete_self();
		os_dly_wait(1);
	}
}

__task void task2()
{
	while(1)
	{
		printf("Task2: HelloWorld!\n");
		os_dly_wait(3);
	}
}

__task void task3()
{
	int count;
	while(1)
	{
		count = os_tsk_count_get();
		printf("Task3: Number of tasks: %d\n", count);
		os_dly_wait(5);
	}
}

__task void init(void)
{
	os_tsk_create(task1, 1);
	os_tsk_create(task2, 1);
	os_tsk_create(task3, 1);
	os_tsk_delete_self();
}

int main()
{
	SystemInit();
	uart0_init();
	os_sys_init(init);
}
