#ifndef GYRO_DICE_HPP
#define GYRO_DICE_HPP
#define degconvert 57.2957786 //there are like 57 degrees in a radian.

class gyro_dice
{
private:
    double acX, acY, acZ;
    double gyX, gyY, gyZ;
    double roll, pitch;
    double dif = 66000.00;
    double mul = 0.01;
    
    gyro_mpu6050 mpu;
public:
    gyro_dice( gyro_mpu6050 mpu ) : mpu (mpu) {
        update();
    }
    
    void update(){
        mpu.update_all();
        acX = (dif - mpu.get_accelX()) * mul;
        acY = (dif - mpu.get_accelY()) * mul;
        acZ = (dif - mpu.get_accelZ()) * mul;
        gyX = (dif - mpu.get_gyroX()) * mul;
        gyY = (dif - mpu.get_gyroY()) * mul;
        gyZ = (dif - mpu.get_gyroZ()) * mul;
        roll = atan2(acY, acZ)*degconvert;
        pitch = atan2(-acX, sqrt(acY*acY + acZ*acZ))*degconvert;
    }
    
    void printXYZ(){
        hwlib::cout << "|acX: " << int(acX);
        hwlib::cout << "|acY: " << int(acY);
        hwlib::cout << "|acZ: " << int(acZ);
        hwlib::cout << "|gyX: " << int(gyX);
        hwlib::cout << "|gyY: " << int(gyY);
        hwlib::cout << "|gyZ: " << int(gyZ);
        hwlib::cout << "|roll: " << int(roll) << "|pitch: " << int(pitch);
        hwlib::cout << "|\n";
        hwlib::wait_ms(30);
    }
};

#endif // GYRO_DICE_HPP
