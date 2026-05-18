#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>
#include "led_sensor.h"

const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(led_sensor0));

static int cmd_led_sensor_fetch(const struct shell *sh, int argc, char **argv)
{
    sensor_sample_fetch(dev);
    shell_info(sh, "LED turned ON");

    return 0;
}

static int cmd_led_sensor_read(const struct shell *sh, int argc, char **argv)
{
    struct sensor_value val;

    sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
    shell_info(sh, "LED turned OFF");

    return 0;
}

static int cmd_led_sensor_info(const struct shell *sh, int argc, char **argv)
{
    shell_info(sh, "LED device name: %s, device state: %s (init_res: %d)",
                dev->name, dev->state->initialized?"Initialized":"Not initialized", dev->state->init_res);

    return 0;
}

static int cmd_led_sensor_setval(const struct shell *sh, int argc, char **argv)
{
    const uint32_t MIN_VAL = 100;
    const uint32_t MAX_VAL = 2000;
    
    /* not needed since arg count is enforced with SHELL_CMD_ARG
    if (argc != 2) {
        shell_error(sh, "Invalid argument count!");
        return -1;
    }
    */

    uint32_t val = strtoul(argv[1], NULL, 10);

    if (val < MIN_VAL || val > MAX_VAL) {
        shell_error(sh, "Invalid argument value! Should be between %d and %d (ms)", MIN_VAL, MAX_VAL);
        return -1;
    }

    my_led_sensor_set_blink_rate(dev, val);
    shell_info(sh, "LED sleep time set to %d ms", val);

    return 0;
}


SHELL_STATIC_SUBCMD_SET_CREATE(led_sensor_subcmd,
    SHELL_CMD_ARG(fetch, NULL, "LED sensor ON", cmd_led_sensor_fetch, 1, 0),
    SHELL_CMD_ARG(read, NULL, "LED sensor OFF", cmd_led_sensor_read, 1, 0),
    SHELL_CMD_ARG(info, NULL, "LED device info", cmd_led_sensor_info, 1, 0),
    SHELL_CMD_ARG(set, NULL, "Set LED sleep time in ms", cmd_led_sensor_setval, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(led_sensor, &led_sensor_subcmd, "LED sensor commands", NULL);