#pragma once

#include <stdint.h>
#include "driver/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mpu6050{
    gpio_num_t sda;
    gpio_num_t scl;
    i2c_port_t port;
}mpu6050;

mpu6050* mpu6050_init(gpio_num_t sda, gpio_num_t scl, i2c_port_t port);
void mpu6050_deinit(mpu6050* mpu6050);
void mpu6050_write_byte(mpu6050* mpu6050, uint8_t address, uint8_t registers, uint8_t data);
uint8_t mpu6050_read_byte(mpu6050* mpu6050, uint8_t address, uint8_t registers);
void mpu6050_read_bytes(mpu6050* mpu6050, uint8_t address, uint8_t registers, uint8_t count, uint8_t *destination);
float mpu6050_get_gyroscope(mpu6050* mpu6050);
float mpu6050_get_accelerometer(mpu6050* mpu6050);

#ifdef __cplusplus
}
#endif
