#ifndef GYRO_DICE_HPP
#define GYRO_DICE_HPP
#define degconvert 57.2957786 //there are like 57 degrees in a radian.
#define display_width 128
#define display_height 64

class gyro_dice
{
private:
    int acX, acY, acZ;
    int gyX, gyY, gyZ;
    int roll, pitch;
    int dif = 66000; // not really sure why I am still using this
    int mut = 100; // multiplier
    
    gyro_mpu6050 mpu;
    hwlib::glcd_oled display;
    hwlib::target::pin_out beeper;
    
public:
    gyro_dice( gyro_mpu6050 mpu, hwlib::glcd_oled display, hwlib::target::pin_out beeper ) : mpu (mpu), display (display), beeper (beeper) {
        display.clear();
        update();
    }
    
    virtual void update(){
        mpu.update_all();
        acX = (dif - mpu.get_accelX())/mut;
        acY = (dif - mpu.get_accelY())/mut;
        acZ = (dif - mpu.get_accelZ())/mut;
        gyX = (dif - mpu.get_gyroX())/mut;
        gyY = (dif - mpu.get_gyroY())/mut;
        gyZ = (dif - mpu.get_gyroZ())/mut;
        roll = atan2(acY, acZ)*degconvert;
        pitch = atan2(-acX, sqrt(acY*acY + acZ*acZ))*degconvert;
    }
    
    void beep(){
        beeper.set(1);
        hwlib::wait_ms(100);
        beeper.set(0);
        hwlib::wait_ms(100);
    }
    
    void console_xyz(){
        hwlib::cout << "|acX: " << acX;
        hwlib::cout << "|acY: " << acY;
        hwlib::cout << "|acZ: " << acZ;
//        hwlib::cout << "|gyX: " << int(gyX);
//        hwlib::cout << "|gyY: " << int(gyY);
//        hwlib::cout << "|gyZ: " << int(gyZ);
        hwlib::cout << "|roll: " << roll << "|pitch: " << pitch;
        hwlib::cout << "|\n";
        hwlib::wait_ms(30);
    }
    
    virtual void check_side(){};
    virtual void set_side(){};
    
    void clear(){ display.clear(); }
    int get_acX(){ return acX; }
    int get_acY(){ return acY; }
    int get_acZ(){ return acZ; }
    hwlib::glcd_oled get_display(){ return display; }
};

class gyro_dice_d6: public gyro_dice{
private:
    // {sides{acX, acY, acZ, roll, pitch}}
    int d6[6][5] = {{488, 6, 650, 0, -36},      //1
                    {650, 170, 7, 87, -75},    //2
                    {625, 656, 160, 75, -42},       //3
                    {10, 10, 490, 0, -14},     //4
                    {658, 497, 650, 37, -37},    //5
                    {160, 650, 10, 22, -80}};     //6
                    
    int curSide = 0;
    int sens = 135; // sensitivity of snapping to a sides
                    
public:
    gyro_dice_d6( gyro_mpu6050 mpu, hwlib::glcd_oled display, hwlib::target::pin_out beep) 
    : gyro_dice(mpu, display, beep){}
    
    void set_side(int side){
        if(side != curSide){
            int scale = 4;
            curSide = side;;
            clear();
            print_font(get_display(), side, display_width/2+(scale*2), display_height/2+(scale*2), scale);
            for(int i = 0; i < side; i++){
                //dice_beep();
            }
        }
    }
    
    void check_side(){
        int count = 0;
        int checkValues[3] = {get_acX(), get_acY(), get_acZ()};
        for(int i = 0; i < 6; i++){
            for(int n = 0; n < 3; n++){
                if(d6[i][n] > checkValues[n]+sens || d6[i][n] < checkValues[n]-sens){
                    break;
                }
                count ++;
            }
            if(count > 2){
                set_side(i+1);
                break;
            }
            count = 0;
        }
    }
};

class gyro_dice_d0: public gyro_dice
{
private:
    int curSide = 0;
public:
    gyro_dice_d0( gyro_mpu6050 mpu, hwlib::glcd_oled display, hwlib::target::pin_out beep) 
    : gyro_dice(mpu, display, beep){}
    
    void check_side(){
        //int checkValues[3] = {get_acX(), get_acY(), get_acZ()};
    }
};

#endif // GYRO_DICE_HPP
