#ifndef      I2C_H_
#define     I2C_H_

#include "driver/i2c.h"
#include "esp_log.h"

#define I2C_MASTER_SCL_IO           GPIO_NUM_9
#define I2C_MASTER_SDA_IO           GPIO_NUM_8
#define I2C_MASTER_NUM              I2C_NUM_0
#define I2C_MASTER_FREQ_HZ          100000     
#define I2C_MASTER_TX_BUF_DISABLE   0   
#define I2C_MASTER_RX_BUF_DISABLE   0

void i2c_master_init();
void i2c_scanner();

#endif      //I2C_H_