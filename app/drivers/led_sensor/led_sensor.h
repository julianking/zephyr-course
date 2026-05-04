#ifndef __MY_LED_SENSOR_H__
#define __MY_LED_SENSOR_H__

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#ifdef __cplusplus
extern "C" {
#endif

int my_led_sensor_set_blink_rate(const struct device *dev, uint16_t ms);

#ifdef __cplusplus
}
#endif

#endif