#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "../drivers/led_sensor/led_sensor.h"

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(led_sensor0));
    struct sensor_value val = {0};

    if (!device_is_ready(driver))
        return -1;


    while (1) {

       k_msleep(1000);
    }
    return 0;
}
