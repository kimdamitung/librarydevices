#include <stdio.h>
//include driver espidf
//include user
#include "i2c.h"
#include "mpu6050.h"
#include "esp_log.h"

static const char *MPU6050 = "mpu6050";

void app_main(void){
    mpu6050* mpu6050 = mpu6050_init(GPIO_NUM_8, GPIO_NUM_9, I2C_NUM_0);
    i2c_scanner();
    while (true){
        /* code */
        ESP_LOGD(MPU6050, "Giá trị gyroscope: %.2f", mpu6050_get_gyroscope(mpu6050));
        ESP_LOGD(MPU6050, "Giá trị accelerometer: %.2f", mpu6050_get_accelerometer(mpu6050));
        printf("Giá trị gyroscope: %.2f\n", mpu6050_get_gyroscope(mpu6050));
        printf("Giá trị accelerometer: %.2f\n", mpu6050_get_accelerometer(mpu6050));
        vTaskDelay(1000);
    }
}
