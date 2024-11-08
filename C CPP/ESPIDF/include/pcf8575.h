#pragma once

#include <stdint.h>
#include "driver/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_KEY     4

typedef struct Pin Pin;

typedef struct pcf8575{
    gpio_num_t sda;
    gpio_num_t scl;
    i2c_port_t port;
}pcf8575;

pcf8575* pcf8575_init(gpio_num_t sda, gpio_num_t scl, i2c_port_t port);
void pcf8575_destroy(pcf8575* pcf8575);
uint16_t pcf8575_getIO16bit(pcf8575* pcf8575);
void pcf8575_setIO16bit(pcf8575* pcf8575, uint16_t value);
int pcf8575_OUTPUT(pcf8575* pcf8575, int pin, int value);
Pin* pcf8575_PIN(pcf8575* pcf8575, int pin);
void pcf8575_PIN_destroy(Pin* pin);
void pcf8575_read(pcf8575* pcf8575);
void pcf8575_write(pcf8575* pcf8575);
char pcf8575_setBoard16Bit(pcf8575* pcf8575, const char keyboards[MAX_KEY][MAX_KEY]);

#ifdef __cplusplus
}
#endif
