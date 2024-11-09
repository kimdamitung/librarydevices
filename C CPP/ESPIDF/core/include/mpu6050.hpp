#if !defined(_MPU6050_H_)
#define _MPU6050_H_

#include "driver/i2c.h"
#include <stdint.h>

#define	SMPLRT_DIV		0x19
#define	CONFIG			0x1A
#define	GYRO_CONFIG		0x1B
#define	ACCEL_CONFIG	0x1C
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	
#define	WHO_AM_I		0x75	
#define	ADDRESS	        0x68	
#define ORIGINAL_OUTPUT 0
#define ACC_FULLSCALE   2
#define GYRO_FULLSCALE  250

class KalmaFilter{
    private:
        float gyro_y;
        float angle = 0;
        float q_bias = 0;
        float angle_err = 0;
        float q_angle = 0.1;
        float q_gyro = 0.1;
        float r_angle = 0.5;
        float dt = 0.005;
        char c_0 = 1;
        float pct_0=0, pct_1=0, e=0;
        float k_0=0, k_1=0, t_0=0, t_1=0;
        float pdot[4] = {0, 0, 0, 0};
        float pp[2][2] = {{1, 0}, {0, 1}};
    public:
        KalmaFilter(float dt);
        ~KalmaFilter();
        // setter
        void setGyroY(float gyroY) { gyro_y = gyroY; }
        void setAngle(float ang) { angle = ang; }
        void setQBias(float bias) { q_bias = bias; }
        void setAngleErr(float err) { angle_err = err; }
        void setQAngle(float qAng) { q_angle = qAng; }
        void setQGyro(float qGyro) { q_gyro = qGyro; }
        void setRAngle(float rAng) { r_angle = rAng; }
        void setDT(float deltaT) { dt = deltaT; }
        void setC0(char c) { c_0 = c; }
        void setPct0(float pct0) { pct_0 = pct0; }
        void setPct1(float pct1) { pct_1 = pct1; }
        void setE(float error) { e = error; }
        void setK0(float k0) { k_0 = k0; }
        void setK1(float k1) { k_1 = k1; }
        void setT0(float t0) { t_0 = t0; }
        void setT1(float t1) { t_1 = t1; }
        void setPdot(int index, float value) { 
            if(index >= 0 && index < 4) pdot[index] = value; 
        }
        void setPp(int i, int j, float value) { 
            if(i >= 0 && i < 2 && j >= 0 && j < 2) pp[i][j] = value; 
        }
        // function
        float filter(float accel, float gyro);
};

class mpu6050{
    private:
        /* data */
        gpio_num_t sda;
        gpio_num_t scl;
        i2c_port_t port;
    public:
        mpu6050(gpio_num_t sda, gpio_num_t scl, i2c_port_t port);
        ~mpu6050();
        bool write(uint8_t address, uint8_t registers, uint8_t data);
        bool read(uint8_t address, uint8_t data, uint8_t *buffer, uint32_t lenghts);
        uint8_t readbyte(uint8_t address, uint8_t registers);
        bool init();
        float getAccX();
        float getAccY();
        float getAccZ();
        float getGyroX();
        float getGyroY();
        float getGyroZ();
        short getTemp();
};

#endif // _MPU6050_H_