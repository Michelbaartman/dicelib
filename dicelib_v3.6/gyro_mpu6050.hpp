/** @file gyro_mpu6050.hpp */ 
#ifndef GYRO_MPU6050_HPP
#define GYRO_MPU6050_HPP

// mpu_addr is the regular i2c adress of the MPU. if it does not work, try running the i2c scanner by Nick Gammon: http://www.gammon.com.au/forum/?id=10896
// Special thanks to Dylan Rakiman for figuring out how to wake the gyroscope up!

/*! \brief gyro_mpu6050, Gyroscope class
 */
class gyro_mpu6050
{
private:
    // main i2c adress of the module
    const uint8_t mpu_addr  = 0x68;
    
    // power manager adress of the module. default is set to sleep
    const uint8_t pwr_addr  = 0x6B;
    
    // adresses for the various registers on the mpu6050.
    const uint8_t accelX_H  = 0x3B;  const uint8_t accelX_L  = 0x3C;
    const uint8_t accelY_H  = 0x3D;  const uint8_t accelY_L  = 0x3E;
    const uint8_t accelZ_H  = 0x3F;  const uint8_t accelZ_L  = 0x40;
    const uint8_t temp_H    = 0x41;  const uint8_t temp_L    = 0x42;
    const uint8_t gyroX_H   = 0x43;  const uint8_t gyroX_L   = 0x44;
    const uint8_t gyroY_H   = 0x45;  const uint8_t gyroY_L   = 0x46;
    const uint8_t gyroZ_H   = 0x47;  const uint8_t gyroZ_L   = 0x48;
    
    // MPU6050 commands
    const uint8_t wakeup[2] =   { pwr_addr, 0x01 };
    const uint8_t sleep[2] =    { pwr_addr, 0x41 };
    
    // variables for the current state of the MPU6050 onboard modules
    uint16_t accelX, accelY, accelZ;    // Accelerometer 
    uint16_t tmp;                       // Temperature
    uint16_t gyroX, gyroY, gyroZ;       // Gyroscope
    
    // i2c bus of the module
    hwlib::i2c_bus_bit_banged_scl_sda & mpu6050;
    
public:
    /*! \brief gyro_mpu6050
     * Gyroscope mpu6050 constructor, requests the i2c bus/adress of the gyroscope. 
     * Also wakes up the gyroscope.
     */
    gyro_mpu6050( hwlib::i2c_bus_bit_banged_scl_sda & mpu ) : mpu6050 ( mpu ) {
        mpu6050.write(mpu_addr, wakeup, 2); // wakes up the mpu6050
    }
    
    /*! \brief update_gyroXYZ
     * Obtains raw gyroscope data from the mpu6050
     * each axis exists of 2 bytes, a high and a low byte, then mashes them together
     */
    void update_gyroXYZ(uint16_t & x, uint16_t & y, uint16_t & z){
        uint8_t bytes[2];
        // get X
        mpu6050.write(mpu_addr, & gyroX_H, 1);
        mpu6050.read(mpu_addr, bytes, 2);   x = bytes[0]<<8|bytes[1];
        // get Y
        mpu6050.write(mpu_addr, & gyroY_H, 1);
        mpu6050.read(mpu_addr, bytes, 2);   y = bytes[0]<<8|bytes[1];
        // get Z
        mpu6050.write(mpu_addr, & gyroZ_H, 1);
        mpu6050.read(mpu_addr, bytes, 2);   z = bytes[0]<<8|bytes[1];
    }
    
    /*! \brief update_accelXYZ
     * Obtains raw accelerometer data from the mpu6050
     */
    void update_accelXYZ(uint16_t & x, uint16_t & y, uint16_t & z){
        uint8_t bytes[2];
        // get X
        mpu6050.write(mpu_addr, & accelX_H, 1);
        mpu6050.read(mpu_addr, bytes, 2);   x = bytes[0]<<8|bytes[1];
        // get Y
        mpu6050.write(mpu_addr, & accelY_H, 1);
        mpu6050.read(mpu_addr, bytes, 2);   y = bytes[0]<<8|bytes[1];
        // get Z
        mpu6050.write(mpu_addr, & accelZ_H, 1);
        mpu6050.read(mpu_addr, bytes, 2);   z = bytes[0]<<8|bytes[1];
    }
    
    /*! \brief update_temperature 
     * Obtains raw temperature data from the mpu6050
     */
    void update_temp(uint16_t & tmp){
        uint8_t bytes[2];
        // get tmp
        mpu6050.write(mpu_addr, & temp_H, 1);
        mpu6050.read(mpu_addr, bytes, 2);   tmp = bytes[0]<<8|bytes[1];
    }
    
    /*! \brief update_all
     * Updates all data of the gyroscope
     */
    void update_all(){
        update_accelXYZ(accelX, accelY, accelZ);
        update_gyroXYZ(gyroX, gyroY, gyroZ);
        update_temp(tmp);
    }
    
    // getters
    uint16_t get_tmp()      { return tmp; } ///< returns gyroscope tmp
    uint16_t get_gyroX()    { return gyroX; } ///< returns gyroscope X
    uint16_t get_gyroY()    { return gyroY; } ///< returns gyroscope Y
    uint16_t get_gyroZ()    { return gyroZ; } ///< returns gyroscope Z
    uint16_t get_accelX()   { return accelX; } ///< returns accelerometer x
    uint16_t get_accelY()   { return accelY; } ///< returns accelerometer y
    uint16_t get_accelZ()   { return accelZ; } ///< returns accelerometer z
    
    
};

#endif // GYRO_MPU6050_HPP
