#include "mpu6050.hpp"
#include "driver/i2c.h"

mpu6050::mpu6050(gpio_num_t sda, gpio_num_t scl, i2c_port_t port) : sda(sda), scl(scl), port(port){
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
}

mpu6050::~mpu6050(){
    i2c_driver_delete(port);
}

void mpu6050::_write_byte(uint8_t address, uint8_t registers, uint8_t data){
    i2c_cmd_handle_t commands = i2c_cmd_link_create();
    if(commands == NULL){
        return;
    }
    i2c_master_start(commands);
    i2c_master_write_byte(commands, address << 1 | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(commands, registers, true);
    i2c_master_write_byte(commands, data, true);
    i2c_master_stop(commands);
    i2c_master_cmd_begin(port, commands, pdMS_TO_TICKS(100)); 
    i2c_cmd_link_delete(commands);
}

uint8_t mpu6050::_read_byte(uint8_t address, uint8_t registers){
    uint8_t data = 0;
    i2c_cmd_handle_t commands = i2c_cmd_link_create();
    if(commands == NULL){
        return -1;
    }
    i2c_master_start(commands);
    i2c_master_write_byte(commands, address << 1 | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(commands, registers, true);
    i2c_master_stop(commands);
    i2c_master_cmd_begin(port, commands, pdMS_TO_TICKS(100));
    i2c_cmd_link_create();
    if(commands == NULL){
        return -1;
    }
    i2c_master_start(commands);
    i2c_master_write_byte(commands, address << 1 | I2C_MASTER_READ, true);
    i2c_master_read_byte(commands, &data, I2C_MASTER_LAST_NACK);
    i2c_master_stop(commands);
    i2c_master_cmd_begin(port, commands, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(commands);
    return data;
}

void mpu6050::_read_bytes(uint8_t address, uint8_t registers, uint8_t count, uint8_t *destination){
    i2c_cmd_handle_t commands = i2c_cmd_link_create();
    if(commands == NULL){
        return;
    }
    i2c_master_start(commands);
    i2c_master_write_byte(commands, address << 1 | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(commands, registers, true);
    i2c_master_stop(commands);
    i2c_master_cmd_begin(port, commands, pdMS_TO_TICKS(10));
    i2c_cmd_link_create();
    if(commands == NULL){
        return;
    }
    i2c_master_start(commands);
    i2c_master_write_byte(commands, address << 1 | I2C_MASTER_READ, true);
    for (uint8_t i = 0; i < count; i++) {
        if (i == count - 1) {
            i2c_master_read_byte(commands, &destination[i], I2C_MASTER_LAST_NACK);
        } else {
            i2c_master_read_byte(commands, &destination[i], I2C_MASTER_ACK);
        }
    }
    i2c_master_stop(commands);
    i2c_master_cmd_begin(port, commands, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(commands);
}

float mpu6050::_get_gyroscope(){
    switch (gyroscope_scale){
        case GFS_250DPS:
            return 250.0 / 32768.0;
            break;
        case GFS_500DPS:
            return 500.0 / 32768.0;
            break;
        case GFS_1000DPS:
            return 1000.0 / 32768.0;
            break;
        case GFS_2000DPS:
            return 2000.0 / 32768.0;
            break;
        default:
            return 0;
            break;
    }
}

float mpu6050::_get_accelerometer(){
    switch (accelerometer_scale){
        case AFS_2G:
            return 2.0 / 32768.0;
            break;
        case AFS_4G:
            return 4.0 / 32768.0;
            break;
        case AFS_8G:
            return 8.0 / 32768.0;
            break;
        case AFS_16G:
            return 16.0 / 32768.0;
            break;
        default:
            return 0;
            break;
    }
}

void mpu6050::_read_gyroscope(int16_t *destination){
    uint8_t data[6];
    _read_bytes(ADDRESS, GYRO_XOUT_H, 6, &data[0]);
    for (uint8_t i = 0; i < 3; i++) {
        destination[i] = (int16_t)(data[i * 2] << 8) | data[i * 2 + 1];
    }
}

void mpu6050::_read_accelerometer(int16_t *destination){
    uint8_t data[6];
    _read_bytes(ADDRESS, ACCEL_XOUT_H, 6, &data[0]);
    for (uint8_t i = 0; i < 3; i++) {
        destination[i] = (int16_t)(data[i * 2] << 8) | data[i * 2 + 1];
    }
}

extern "C" {
    mpu6050* mpu6050_init(gpio_num_t sda, gpio_num_t scl, i2c_port_t port){
        return new mpu6050(sda, scl, port);
    }

    void mpu6050_deinit(mpu6050* mpu6050){
        delete mpu6050;
    }

    void mpu6050_write_byte(mpu6050* mpu6050, uint8_t address, uint8_t registers, uint8_t data){
        if(mpu6050 == NULL){
            return;
        }else{
            mpu6050->_write_byte(address, registers, data);
        }
    }

    uint8_t mpu6050_read_byte(mpu6050* mpu6050, uint8_t address, uint8_t registers){
        if(mpu6050 == NULL){
            return -1;
        }else{
            return mpu6050->_read_byte(address, registers);
        }
    }

    void mpu6050_read_bytes(mpu6050* mpu6050, uint8_t address, uint8_t registers, uint8_t count, uint8_t *destination){
        if(mpu6050 == NULL){
            return;
        }else{
            mpu6050->_read_bytes(address, registers, count, destination);
        }
    }

    float mpu6050_get_gyroscope(mpu6050* mpu6050){
        if(mpu6050 == NULL){
            return -1;
        }else{
            return mpu6050->_get_gyroscope();
        }
    }

    float mpu6050_get_accelerometer(mpu6050* mpu6050){
        if(mpu6050 == NULL){
            return -1;
        }else{
            return mpu6050->_get_accelerometer();
        }
    }

}