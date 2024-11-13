#include <stdio.h>
#include <stdlib.h>
//include driver espidf
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//include user
#include "RGB.hpp"

extern "C" void app_main() {
    while (true){
        /* code */
        RGB led(GPIO_NUM_48, 1);
        led.fill(255, 0, 0);
        led.show();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        led.fill(0, 255, 0);
        led.show();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        led.fill(0, 0, 255);
        led.show();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        led.fill(255, 185, 16);
        led.show();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        led.fill(0, 255, 255);
        led.show();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        led.fill(255, 255, 255);
        led.show();
    }
}