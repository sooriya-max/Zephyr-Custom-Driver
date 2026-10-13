#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

static int my_init(void)
{
    printk("My Init Just Ran\n");
    return 0;
}

SYS_INIT(my_init, POST_KERNEL, 0);

int main(void)
{
    const struct device* chip = DEVICE_DT_GET(DT_NODELABEL(mychippp));

    if(!device_is_ready(chip))
    {
        printk("The Chip Did not Run\n");
        return 0;
    }
    printk("MyChip is Ready\n");

    struct sensor_value val;

    sensor_sample_fetch(chip);
    sensor_channel_get(chip, SENSOR_CHAN_ACCEL_X, &val);
    printk("sample = %d\n", val.val1);

    printk("main started\n");
    return 0;
}