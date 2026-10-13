#define DT_DRV_COMPAT zblock_mychip

#include <zephyr/kernel.h>
#include <zephyr/device.h>

struct mychip_config {
    uint32_t my_value;                 /* Fix 1: was "myvalue" */
};

static int mychip_init(const struct device *dev)
{
    const struct mychip_config *cfg = dev->config;
    printk("My Chip Just Ran %s, my_value = %u\n", dev->name, cfg->my_value);
    return 0;
}

/* Fix 2: config + DEVICE_DT_INST_DEFINE both live inside the macro, joined with '\' */
#define MYCHIP_INIT(inst) static const struct mychip_config mychip_config_##inst = {              \
        .my_value = DT_INST_PROP(inst, my_value),                           \
    };                                                                      \
    DEVICE_DT_INST_DEFINE(inst, mychip_init, NULL, NULL,                    \
                          &mychip_config_##inst, POST_KERNEL, 90, NULL);

DT_INST_FOREACH_STATUS_OKAY(MYCHIP_INIT)