#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

static int my_board_init(void)
{
    printk("Board Initialized\n");
    
    return 0;
}

SYS_INIT(my_board_init, PRE_KERNEL_1, 0);