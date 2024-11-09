#include "mpu6050.hpp"

#if ORIGINAL_OUTPUT == 0
	#if  ACC_FULLSCALE  == 2
		#define AccAxis_Sensitive (float)(16384)
	#elif ACC_FULLSCALE == 4
		#define AccAxis_Sensitive (float)(8192)
	#elif ACC_FULLSCALE == 8
		#define AccAxis_Sensitive (float)(4096)
	#elif ACC_FULLSCALE == 16
		#define AccAxis_Sensitive (float)(2048)
	#endif 
		
	#if   GYRO_FULLSCALE == 250
		#define GyroAxis_Sensitive (float)(131.0)
	#elif GYRO_FULLSCALE == 500
		#define GyroAxis_Sensitive (float)(65.5)
	#elif GYRO_FULLSCALE == 1000
		#define GyroAxis_Sensitive (float)(32.8)
	#elif GYRO_FULLSCALE == 2000
		#define GyroAxis_Sensitive (float)(16.4)
	#endif
		
#else
	#define AccAxis_Sensitive  (1)
	#define GyroAxis_Sensitive (1)
#endif

// object kalma filter
KalmaFilter::KalmaFilter(float dt){
    this->dt = dt;
}

KalmaFilter::~KalmaFilter(){}

float KalmaFilter::filter(float accel, float gyro){
    angle += (gyro - q_bias) * dt;
    angle_err = accel - angle;
    pdot[0] = q_angle - pp[0][1] - pp[1][0];
    pdot[1] = -pp[1][1];
    pdot[2] = -pp[1][1];
    pdot[3] = q_gyro;
    pp[0][0] += pdot[0] * dt;
    pp[0][1] += pdot[1] * dt;
    pp[1][0] += pdot[2] * dt;
    pp[1][1] += pdot[3] * dt;
    pct_0 = c_0 * pp[0][0];
    pct_1 = c_0 * pp[1][0];
    e = r_angle + c_0 * pct_0;
    k_0 = pct_0 / e;
    k_1 = pct_1 / e;
    t_0 = pct_0;
    t_1 = c_0 * pp[0][1];
    pp[0][0] -= k_0 * t_0;
    pp[0][1] -= k_0 * t_1;
    pp[1][0] -= k_1 * t_0;
    pp[1][1] -= k_1 * t_1;
    angle += k_0 * angle_err;
    q_bias += k_1 * angle_err;
    gyro_y= gyro - q_bias;
    return angle;
}

// object mpu6050
mpu6050::mpu6050(gpio_num_t sda, gpio_num_t scl, i2c_port_t port){
    this->sda = sda;
    this->scl = scl;
    this->port = port;
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = this->sda,
        .scl_io_num = this->scl,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {
            .clk_speed = 400000
        },
        .clk_flags = 0
    };
    i2c_param_config(this->port, &conf);
    i2c_driver_install(this->port, conf.mode, 0, 0, 0);
}

mpu6050::~mpu6050(){
    i2c_driver_delete(this->port);
}

bool mpu6050::write(uint8_t address, uint8_t registers, uint8_t data){
    i2c_cmd_handle_t commands = i2c_cmd_link_create();
    i2c_master_start(commands);
    i2c_master_write_byte(commands, address << 1, true);
    i2c_master_write_byte(commands, registers, true);
    i2c_master_write_byte(commands, data, true);
    i2c_master_stop(commands);
    esp_err_t ret = i2c_master_cmd_begin(this->port, commands, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(commands);
    if(ret == ESP_OK)
        return true;
    return false;
}

bool mpu6050::read(uint8_t address, uint8_t data, uint8_t *buffer, uint32_t lenghts){
    i2c_cmd_handle_t commands = i2c_cmd_link_create();
    i2c_master_start(commands);
    i2c_master_write_byte(commands, address << 1,true);
    i2c_master_write_byte(commands, data, true);
    i2c_master_stop(commands);
    int ret = i2c_master_cmd_begin(this->port, commands, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(commands);
    if (ret == ESP_FAIL)
        return false;
    commands = i2c_cmd_link_create();
    i2c_master_start(commands);
    i2c_master_write_byte(commands, address << 1 | 1, true);
    while(lenghts) {
        i2c_master_read_byte(commands, buffer, lenghts == 1 ? I2C_MASTER_ACK : I2C_MASTER_NACK);
        buffer++;
        lenghts--;
    }
    i2c_master_stop(commands);
    ret = i2c_master_cmd_begin(port, commands, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(commands);
    if (ret == ESP_FAIL)
        return false;
    return true;
}

uint8_t mpu6050::readbyte(uint8_t address, uint8_t registers){
    i2c_cmd_handle_t commands = i2c_cmd_link_create();
    i2c_master_start(commands);
    i2c_master_write_byte(commands, address << 1, 1);
    i2c_master_write_byte(commands, registers, 1);
    i2c_master_stop(commands);
    i2c_master_cmd_begin(port, commands, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(commands);
    uint8_t data;
    commands = i2c_cmd_link_create();
    i2c_master_start(commands);
    i2c_master_write_byte(commands, address << 1 | 1, true);
    i2c_master_read_byte(commands, &data, I2C_MASTER_ACK);
    i2c_master_stop(commands);
    i2c_master_cmd_begin(port, commands, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(commands);
    return data;
}

bool mpu6050::init(){
    if(!this->write(ADDRESS, PWR_MGMT_1 , 0x00))
        return false;
    if(!this->write(ADDRESS, SMPLRT_DIV, 0x07))
        return false;
    if(!this->write(ADDRESS, CONFIG, 0x07))
        return false;
    if (!this->write(ADDRESS, GYRO_CONFIG , 0x18))
        return false;
    if (!this->write(ADDRESS, ACCEL_CONFIG, 0x01))
        return false;
    return true;
}

float mpu6050::getAccX() {
    uint8_t r[2];
    this->read(ADDRESS, ACCEL_XOUT_H, r, 2);
    short accx = r[0] << 8 | r[1];
    return (float)accx / AccAxis_Sensitive;
}

float mpu6050::getAccY() {
    uint8_t r[2];
    this->read(ADDRESS, ACCEL_YOUT_H, r, 2);
    short accy = r[0] << 8 | r[1];
    return (float)accy / AccAxis_Sensitive;
}

float mpu6050::getAccZ() {
    uint8_t r[2];
    this->read(ADDRESS, ACCEL_ZOUT_H, r, 2);
    short accz = r[0] << 8 | r[1];
    return (float)accz / AccAxis_Sensitive;
}

float mpu6050::getGyroX() {
    uint8_t r[2];
    this->read(ADDRESS, GYRO_XOUT_H, r, 2);
    short gyrox = r[0] << 8 | r[1];
    return (float)gyrox / GyroAxis_Sensitive;
}

float mpu6050::getGyroY() {
    uint8_t r[2];
    this->read(ADDRESS, GYRO_YOUT_H, r, 2);
    short gyroy = r[0] << 8 | r[1];
    return (float)gyroy / GyroAxis_Sensitive;
}

float mpu6050::getGyroZ() {
    uint8_t r[2];
    this->read(ADDRESS, GYRO_ZOUT_H, r, 2);
    short gyroz = r[0] << 8 | r[1];
    return (float)gyroz / GyroAxis_Sensitive;
}

short mpu6050::getTemp() {
    uint8_t r[2];
    this->read(ADDRESS, TEMP_OUT_H, r, 2);
    return r[0] << 8 | r[1];
}

extern "C"{
    // marco object kalma filter
    KalmaFilter *kalma_filter_init(float dt) {
        KalmaFilter* kalma = (KalmaFilter*)malloc(sizeof(KalmaFilter));
        if (kalma != NULL) {
            // kalma->setGyroY(0);
            kalma->setAngle(0);
            kalma->setQBias(0);
            kalma->setAngleErr(0);
            kalma->setQAngle(0.1);
            kalma->setQGyro(0.1);
            kalma->setRAngle(0.5);
            kalma->setDT(dt);
            kalma->setC0(1);
            kalma->setPct0(0);
            kalma->setPct1(0);
            kalma->setE(0);
            kalma->setK0(0);
            kalma->setK1(0);
            kalma->setT0(0);
            kalma->setT1(0);
            kalma->setPdot(0, 0);
            kalma->setPdot(1, 0);
            kalma->setPdot(2, 0);
            kalma->setPdot(3, 0);
            kalma->setPp(0, 0, 1);
            kalma->setPp(0, 1, 0);
            kalma->setPp(1, 0, 0);
            kalma->setPp(1, 1, 1);
        }
        return kalma;
    }

    float kalma_filter(KalmaFilter *kalma_filter, float accel, float gyro){
        return kalma_filter->filter(accel, gyro);
    }

    // marco object mpu6050
    mpu6050 *mpu6050_init(gpio_num_t sda, gpio_num_t scl, i2c_port_t port){
        return new mpu6050(sda, scl, port);
    }

    void mpu6050_deinit(mpu6050 *mpu6050){
        delete mpu6050;
    }

    float mpu6050_get_accel_x(mpu6050* mpu6050){
        if(mpu6050 == NULL)
            return -1;
        else
            return mpu6050->getAccX();
    }

    float mpu6050_get_accel_y(mpu6050* mpu6050){
        if(mpu6050 == NULL)
            return -1;
        else
            return mpu6050->getAccY();
    }

    float mpu6050_get_accel_z(mpu6050* mpu6050){
        if(mpu6050 == NULL)
            return -1;
        else
            return mpu6050->getAccZ();
    }

    float mpu6050_get_gyro_x(mpu6050* mpu6050){
        if(mpu6050 == NULL)
            return -1;
        else
            return mpu6050->getGyroX();
    }

    float mpu6050_get_gyro_y(mpu6050* mpu6050){
        if(mpu6050 == NULL)
            return -1;
        else
            return mpu6050->getGyroY();
    }

    float mpu6050_get_gyro_z(mpu6050* mpu6050){
        if(mpu6050 == NULL)
            return -1;
        else
            return mpu6050->getGyroZ();
    }

    short mpu6050_get_temp(mpu6050* mpu6050){
        if(mpu6050 == NULL)
            return -1;
        else
            return mpu6050->getTemp();
    }
}