#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(led_sensor0));
    struct sensor_value val = {0};

    bool led_state = true;

    while (1) {
        if (led_state) {
            int ret = sensor_channel_get(driver, SENSOR_CHAN_ALL, &val);
        }
        else {
            int ret = sensor_sample_fetch(driver);
        }

        led_state = !led_state;
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
    return 0;
}
