#include <zephyr/kernel.h>
#include <zephyr/init.h>

static int my_init(void)
{
    printk("My Init Just Ran\n");
    return 0;
}

SYS_INIT(my_init, POST_KERNEL, 0);

int main(void)
{
    printk("main started\n");
    return 0;
}