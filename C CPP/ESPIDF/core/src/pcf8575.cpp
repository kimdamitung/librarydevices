#include "pcf8575.hpp"
#include <driver/i2c.h>
#include <stdio.h>
// object PCF8575

pcf8575::pcf8575(gpio_num_t sda, gpio_num_t scl, i2c_port_t port): sda(sda), scl(scl), port(port){
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda,
        .scl_io_num = scl,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {
            .clk_speed = 400000
        },
        .clk_flags = 0
    };
    i2c_param_config(port, &conf);
    i2c_driver_install(port, conf.mode, 0, 0, 0);
    data[0] = 0x00;
    data[1] = 0x00;
}

pcf8575::~pcf8575(){
    i2c_driver_delete(port);
}

uint16_t pcf8575::getIO16bit(){
    this->read();
    return (data[0] | (data[1] << 1));
}

void pcf8575::setIO16bit(uint16_t value){
    data[0] = (uint8_t)(value & 0xFF);
    data[1] = (uint8_t)((value >> 8) & 0xFF);
    this->write();
}

int pcf8575::OUTPUT(int pin, int value){
    if(!(pin >= 0 && pin <= 7) && !(pin >= 10 && pin <= 17)){
        printf("Pin must be between 0 and 7 or 10 and 17");
        return -1;
    }
    if(pin >= 10)
        pin -= 2;
    if(value == -1){
        this->read();
        return (data[pin / 8] >> (pin % 8)) & 1;
    }
    if(value)
        data[pin / 8] |= (1 << (pin % 8));
    else
        data[pin / 8] &= ~(1 << (pin % 8));
    this->write();
    return 0;
}

Pin pcf8575::PIN(int pin){
    return Pin(this, pin);
}

void pcf8575::read(){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ADDRESS << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, data, 2, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t err = i2c_master_cmd_begin(port, cmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmd);
    if (err != ESP_OK) {
        printf("Error reading from PCF8575: %s\n", esp_err_to_name(err));
    }
}

void pcf8575::write(){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, data, 2, true);
    i2c_master_stop(cmd);
    esp_err_t err = i2c_master_cmd_begin(port, cmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmd);
    if (err != ESP_OK) {
        printf("Error reading from PCF8575: %s\n", esp_err_to_name(err));
    }
}

char pcf8575::setBoard16Bit(const char keyboards[MAX_KEY][MAX_KEY]){
    uint16_t matrix[MAX_KEY] = {0xFFFE, 0xFFFD, 0xFFFB, 0xFFF7};
    for(int i = 0; i < MAX_KEY; ++i){
        this->setIO16bit(matrix[i]);
        uint16_t value = this->getIO16bit();
        if(value == 0xFFFF)
            continue;
        for(int j = 0; j < MAX_KEY; ++j){
            if(keyboards[i][j] != '\0'){
                if(value >> j & 1)
                    return keyboards[i][j];
            }
        }
    }
    this->setIO16bit(0xFFFF);
    return '\0';
}

// object Pin

Pin::Pin(pcf8575* pcf, int pin): pcf(pcf), pin(pin){}

Pin::~Pin(){}


extern "C"{
    pcf8575* pcf8575_init(gpio_num_t sda, gpio_num_t scl, i2c_port_t port){
        return new pcf8575(sda, scl, port);
    }

    void pcf8575_destroy(pcf8575* pcf8575){
        delete pcf8575;
    }

    uint16_t pcf8575_getIO16bit(pcf8575* pcf8575){
        return pcf8575->getIO16bit();
    }

    void pcf8575_setIO16bit(pcf8575* pcf8575, uint16_t value){
        pcf8575->setIO16bit(value);
    }

    int pcf8575_OUTPUT(pcf8575* pcf8575, int pin, int value){
        return pcf8575->OUTPUT(pin, value);
    }

    Pin* pcf8575_PIN(pcf8575* pcf8575, int pin){
        return new Pin(pcf8575, pin);
    }

    void pcf8575_PIN_destroy(Pin* pin) {
        delete pin;
    }

    void pcf8575_read(pcf8575* pcf8575){
        pcf8575->read();
    }

    void pcf8575_write(pcf8575* pcf8575){
        pcf8575->write();
    }

    char pcf8575_setBoard16Bit(pcf8575* pcf8575, const char keyboards[MAX_KEY][MAX_KEY]){
        return pcf8575->setBoard16Bit(keyboards);
    }
}