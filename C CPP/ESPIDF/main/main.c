#include <stdio.h>
#include <math.h>
//include driver espidf
//include user
#include "i2c.h"
#include "mpu6050.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "st7735.h"

void app_main(void){
    st7735* st7735 = st7735Constructor(GPIO_NUM_4, GPIO_NUM_3, GPIO_NUM_2, GPIO_NUM_6, GPIO_NUM_7);
    st7735setImages(st7735, 0, 0, 128, 160, (uint8_t*)data);
}
