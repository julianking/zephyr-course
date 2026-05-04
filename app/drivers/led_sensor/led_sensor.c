#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include "led_sensor.h"

#define DT_DRV_COMPAT my_led_sensor

LOG_MODULE_REGISTER(led_sensor, LOG_LEVEL_INF);

#define LED_NODE DT_ALIAS(app_led)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

struct my_led_struct {
    uint16_t blink_delay;
};

static int led_my_channel_get(const struct device *dev,
                                enum sensor_channel chan,
                                struct sensor_value *val)
{
    struct my_led_struct *led_data = dev->data;
    
    LOG_INF("led_my_channel_get invoked -> turning LED OFF");
    gpio_pin_set_dt(&led, 0);

    k_msleep(led_data->blink_delay);

    return 0;
}


static int led_my_sample_fetch(const struct device *dev,
				                enum sensor_channel chan)
{
    struct my_led_struct *led_data = dev->data;

    LOG_INF("led_my_sample_fetch invoked -> turning LED ON");
    gpio_pin_set_dt(&led, 1);

    k_msleep(led_data->blink_delay);

    return 0;
}


int my_led_sensor_set_blink_rate(const struct device *dev, uint16_t ms)
{
    struct my_led_struct *led_data = dev->data;
    led_data->blink_delay = ms;

    LOG_INF("LED delay set to %d", ms);

    return 0;
}


static int led_my_init(const struct device *dev)
{
    struct my_led_struct *led_data = dev->data;

    if (!gpio_is_ready_dt(&led)) {
        LOG_ERR("LED GPIO driver not ready");
        return -1;
    }
    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) {
        LOG_ERR("LED could not be initialized properly");
        return -1;
    }

    led_data->blink_delay = 200;    // default blink delay, may be overwritten with my_led_sensor_set_blink_rate()

    return 0;
}


static DEVICE_API(sensor, api_homework_l6) = {
    .sample_fetch = led_my_sample_fetch,
    .channel_get = led_my_channel_get
};


#define LED_SENSOR_DEFINES(inst)                        \
    static struct my_led_struct my_led_struct_##inst;   \
                                                        \
    DEVICE_DT_INST_DEFINE(inst,                         \
                          led_my_init,                  \
                          NULL,                         \
                          &my_led_struct_##inst,        \
                          NULL,                         \
                          POST_KERNEL,                  \
                          80,                           \
                          &api_homework_l6);

DT_INST_FOREACH_STATUS_OKAY(LED_SENSOR_DEFINES)