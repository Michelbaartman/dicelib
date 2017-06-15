#ifndef GYRO_MPU6050_HPP
#define GYRO_MPU6050_HPP

/// MPU_addr is the regular i2c adress of the MPU. if it does not work, try running the i2c scanner by Nick Gammon: http://www.gammon.com.au/forum/?id=10896
/// Special thanks to Dyllan Rakiman for helping out with the early stages of the library.


class gyro_mpu6050
{
private:
    // Adresses for the various modules on the mpu6050.
    const uint8_t mpu_addr = 0x68; 
    const uint8_t pwr_addr = 0x6B;
    const uint8_t accelX_H = 0x3B; const uint8_t accelX_L = 0x3C;
    const uint8_t accelY_H = 0x3D; const uint8_t accelY_L = 0x3E;
    const uint8_t accelZ_H = 0x3F; const uint8_t accelZ_L = 0x40;
    const uint8_t temp_H = 0x41; const uint8_t temp_L = 0x42;
    const uint8_t gyroX_H = 0x43; const uint8_t gyroX_L = 0x44;
    const uint8_t gyroY_H = 0x45; const uint8_t gyroY_L = 0x46;
    const uint8_t gyroZ_H = 0x47; const uint8_t gyroZ_L = 0x48;
    
    const uint8_t wakeup[2] =   { pwr_addr, 0b00000001 };
    const uint8_t sleep[2] =    { pwr_addr, 0b01000001 };
    
    uint16_t accelX, accelY, accelZ;
    uint16_t tmp;
    uint16_t gyroX, gyroY, gyroZ;
    
    hwlib::i2c_bus_bit_banged_scl_sda mpu6050;
    
public:
    gyro_mpu6050( hwlib::i2c_bus_bit_banged_scl_sda mpu ) : mpu6050 ( mpu ) {
        hwlib::cout << "mpu6050 found\n";
        mpu6050.write(mpu_addr, wakeup, 2);
    }
    
    void get_gyroXYZ(uint16_t & x, uint16_t & y, uint16_t & z){
        uint8_t bytes[6];
        mpu6050.write(mpu_addr, & gyroX_H, 1);
        mpu6050.read(mpu_addr, bytes, 6);
        x = bytes[0]<<8|bytes[1];
		y = bytes[2]<<8|bytes[3];
		z = bytes[4]<<8|bytes[5];
    }
    
    void get_accelXYZ(uint16_t & x, uint16_t & y, uint16_t & z){
        uint8_t bytes[6];
        mpu6050.write(mpu_addr, & accelX_H, 1);
        mpu6050.read(mpu_addr, bytes, 6);
        x = bytes[0]<<8|bytes[1];
		y = bytes[2]<<8|bytes[3];
		z = bytes[4]<<8|bytes[5];
    }
    
    void take_temp(uint16_t & temperature){
        uint8_t bytes[2];
        mpu6050.write(mpu_addr, & temp_H, 1);
        mpu6050.read(mpu_addr, bytes, 2);
        temperature = bytes[0]<<8|bytes[1]/380;
    }
    
    void update_all(){
        take_temp(tmp);
        get_accelXYZ(accelX, accelY, accelZ);
        get_gyroXYZ(gyroX, gyroY, gyroZ);
    }
    
    
    // getters
    uint16_t get_tmp(){ return tmp; }
    uint16_t get_gyroX(){ return gyroX; }
    uint16_t get_gyroY(){ return gyroY; }
    uint16_t get_gyroZ(){ return gyroZ; }
    uint16_t get_accelX(){ return accelX; }
    uint16_t get_accelY(){ return accelY; }
    uint16_t get_accelZ(){ return accelZ; }
    
    
};

#endif // GYRO_MPU6050_HPP
