#include "hwlib.hpp"
#include "gyro_mpu6050.hpp"
#include "gyro_font.hpp"
#include <cmath>
#include "gyro_dice.hpp"

int main( void ){	
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    
    //remove need for hwlib:: for target calls
    namespace target = hwlib::target;
    
    //setup oled
    auto oled_scl = target::pin_oc( target::pins::d9 );
    auto oled_sda = target::pin_oc( target::pins::d8 );
    auto oled_i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( oled_scl, oled_sda );
    auto oled_display = hwlib::glcd_oled( oled_i2c_bus, 0x3c );  
    
    // Draw a small cube to represent code and oled activity.
    oled_display.clear();
    //cubePrint(oled_display, 15, 10, 10);
    fontPrint(oled_display, 0, 20, 20, 5);
    fontPrint(oled_display, 1, 60, 20, 5);
    //setup gyro
    auto gyro_scl = target::pin_oc( target::pins::d10 );
    auto gyro_sda = target::pin_oc( target::pins::d11 );
    auto gyro_i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( gyro_scl, gyro_sda ); 
    auto gyro = gyro_mpu6050( gyro_i2c_bus );
    auto dice = gyro_dice(gyro);
    
    while(1){
        dice.update();
        dice.printXYZ();
    }
}
