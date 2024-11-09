#pragma once

#include <stdint.h>
#include "driver/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct KalmaFilter{
    float gyro_y;
    float angle;
    float q_bias;
    float angle_err;
    float q_angle;
    float q_gyro;
    float r_angle;
    float dt;
    char c_0;
    float pct_0, pct_1, e;
    float k_0, k_1, t_0, t_1;
    float pdot[4];
    float pp[2][2];
} KalmaFilter;

KalmaFilter *kalma_filter_init(float dt);
float kalma_filter(KalmaFilter *kalma_filter, float accel, float gyro);

typedef struct mpu6050{
    gpio_num_t sda;
    gpio_num_t scl;
    i2c_port_t port;
}mpu6050;

mpu6050* mpu6050_init(gpio_num_t sda, gpio_num_t scl, i2c_port_t port);
void mpu6050_deinit(mpu6050* mpu6050);
float mpu6050_get_accel_x(mpu6050* mpu6050);
float mpu6050_get_accel_y(mpu6050* mpu6050);
float mpu6050_get_accel_z(mpu6050* mpu6050);
float mpu6050_get_gyro_x(mpu6050* mpu6050);
float mpu6050_get_gyro_y(mpu6050* mpu6050);
float mpu6050_get_gyro_z(mpu6050* mpu6050);
short mpu6050_get_temp(mpu6050* mpu6050);

#ifdef __cplusplus
}
#endif
