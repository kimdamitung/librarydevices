#if !defined(_PCF8575_H_)
#define _PCF8575_H_

#include <driver/i2c.h>
#include <stdint.h>

#define MAX_KEY     4
#define ADDRESS     0x20

class Pin;

class pcf8575{
    private:
        /* data */
        gpio_num_t sda;
        gpio_num_t  scl;
        i2c_port_t port;
        uint8_t data[2];
    public:
        pcf8575(gpio_num_t sda, gpio_num_t scl, i2c_port_t port);
        ~pcf8575();
        uint16_t getIO16bit();
        void setIO16bit(uint16_t value);
        int OUTPUT(int pin, int value);
        Pin PIN(int pin);
        void read();
        void write();
        char setBoard16Bit(const char keyboards[MAX_KEY][MAX_KEY]);
};

class Pin{
    private:
        /* data */
        pcf8575 *pcf;
        int pin;
    public:
        Pin(pcf8575 *pcf, int pin);
        ~Pin();
        int get_value();
        void set_value(int value);
};

#endif // _PCF8575_H_
