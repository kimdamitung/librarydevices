#include <stdio.h>
#include <stdlib.h>
//include driver espidf
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/uart.h"
//include user
#include "RGB.hpp"
#include "BUZZER.hpp"
#include "ESPNOW.hpp"

struct  data_t{
	/* data */
	int a;
	int b;
};

#if CONFIG_IDF_TARGET_ESP32S3

extern "C" void app_main(void) {
	uint8_t mac[6] ={0xd8, 0x3b, 0xda, 0x34, 0xf4, 0x64};
	while (true){
		/* code */
		ESPNOW espnow(mac, 1, true);
		data_t data = {
			.a = 10,
			.b = 20
		};
		espnow.setMessage(reinterpret_cast<const uint8_t*>(&data), sizeof(data));
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

#elif CONFIG_IDF_TARGET_ESP32C3

extern "C" void app_main(void){
	uint8_t mac[6] = {0xd0, 0xef, 0x76, 0x5d, 0x7f, 0xc8};
	ESPNOW espnow(mac, 1, false);
	const data_t& receivedData = espnow.getMessage();
	printf("Received Data:\n");
	printf("  Field a: %d\n", receivedData.a);
	printf("  Field b: %d\n", receivedData.b);
}

#endif