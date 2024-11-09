#include <stdio.h>
#include <math.h>
//include driver espidf
//include user
#include "i2c.h"
#include "mpu6050.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void){
    mpu6050* mpu6050 = mpu6050_init(GPIO_NUM_8, GPIO_NUM_9, I2C_NUM_0);
    i2c_scanner();
    float ax, ay, az, gx, gy, gz;
    float pitch, roll;
    float fpitch, froll;
    KalmaFilter* pfilter = kalma_filter_init(0.005);
    KalmaFilter* rfilter = kalma_filter_init(0.005);
    uint32_t lasttime = 0;
    int count = 0;
    while (true){
        /* code */
        ax = -mpu6050_get_accel_x(mpu6050);
        ay = -mpu6050_get_accel_y(mpu6050);
        az = -mpu6050_get_accel_z(mpu6050);
        gx = mpu6050_get_gyro_x(mpu6050);
        gy = mpu6050_get_gyro_y(mpu6050);
        gz = mpu6050_get_gyro_z(mpu6050);
        pitch = atan(ax / az) * 57.2958;
        roll = atan(ay / az) * 57.2958;
        fpitch = kalma_filter(pfilter, pitch, gy);
        froll = kalma_filter(rfilter, roll, -gx);
        count++;
        if(esp_log_timestamp() / 1000 != lasttime){
            lasttime = esp_log_timestamp() / 1000;
            count = 0;
            printf("pitch: %f, roll: %f, fpitch: %f, froll: %f\n", pitch, roll, fpitch, froll);
            printf("ax: %f, ay: %f, az: %f, gx: %f, gy: %f, gz: %f\n", ax, ay, az, gx, gy, gz);
        }
    }
}
