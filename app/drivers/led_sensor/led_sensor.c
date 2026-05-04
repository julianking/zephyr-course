#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT my_led_sensor

LOG_MODULE_REGISTER(led_sensor, LOG_LEVEL_INF);

#define LED_NODE DT_ALIAS(app_led)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);


static int led_my_channel_get(const struct device *dev,
                                enum sensor_channel chan,
                                struct sensor_value *val)
{
    LOG_INF("led_my_channel_get invoked -> turning LED OFF");
    gpio_pin_set_dt(&led, 0);
    return 0;
}


static int led_my_sample_fetch(const struct device *dev,
				                enum sensor_channel chan)
{
    LOG_INF("led_my_sample_fetch invoked -> turning LED ON");
    gpio_pin_set_dt(&led, 1);
    return 0;
}


static int led_my_init(const struct device *dev)
{
    if (!gpio_is_ready_dt(&led)) {
        LOG_ERR("LED GPIO driver not ready");
        return -1;
    }
    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) {
        LOG_ERR("LED could not be initialized properly");
        return -1;
    }

    return 0;
}


static DEVICE_API(sensor, api_homework_l6) = {
    .sample_fetch = led_my_sample_fetch,
    .channel_get = led_my_channel_get
};


DEVICE_DT_INST_DEFINE(0, led_my_init, NULL, NULL, NULL, POST_KERNEL, 80, &api_homework_l6);