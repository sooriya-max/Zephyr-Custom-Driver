#define DT_DRV_COMPAT zblock_mychip

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

struct mychip_data
{
    uint32_t init_count;
    int32_t sample; //This is for a shared variable to store and load data across other functions to use
};

struct mychip_config 
{
    uint32_t my_value;                 
};

static int mychip_sample_fetch(const struct device* dev, enum sensor_channel chan)
{
    struct mychip_data *fetch = dev -> data;
    fetch -> sample += fetch -> init_count;
    return 0;
}

static int mychip_channel_get(const struct device* dev, enum sensor_channel chan, struct sensor_value* val)
{
    struct mychip_data *send = dev -> data;
    val -> val1 = send -> sample;
    val -> val2 = 0;
    return 0;
}

static DEVICE_API(sensor, mychip_api) = {
    .sample_fetch = mychip_sample_fetch,
    .channel_get  = mychip_channel_get,
};

static int mychip_init(const struct device *dev)
{
    const struct mychip_config *cfg = dev->config;
    struct mychip_data *dta = dev -> data;

    dta -> init_count ++;
    printk("My Chip Just Ran %s, my_value = %u, my_data = %d\n", dev->name, cfg->my_value, dta -> init_count);
    return 0;
}

/* Fix 2: config + DEVICE_DT_INST_DEFINE both live inside the macro, joined with '\' */
#define MYCHIP_INIT(inst) static const struct mychip_config mychip_config_##inst = {        \
        .my_value = DT_INST_PROP(inst, my_value),                                           \
    };                                                                                      \
                          static struct mychip_data mychip_data_##inst;                     \
    DEVICE_DT_INST_DEFINE(inst, mychip_init, NULL, &mychip_data_##inst,                     \
                          &mychip_config_##inst, POST_KERNEL, 90, &mychip_api);

DT_INST_FOREACH_STATUS_OKAY(MYCHIP_INIT)