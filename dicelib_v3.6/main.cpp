/*! \mainpage mpu6050 and gyrodice libraries.
 *
 * \section Introduction
 * My name is Michel Baartman and I wrote these libraries to create custom dice that would be able to roll any number and are lightweight enough to fit on smaller arduino models.
 * The entire idea behind this project was to create custom dice that would be able to roll any number and have a library lightweight enough to fit onto smaller Arduino models for a pocketsize (truly) custom dice.
 * I also wanted to create a dice that would give feedback back to the user in sound as well, so that blind people might enjoy dice a bit more.
 * I want to give a shoutout to Dylan Rakiman for figuring out how to wake the gyroscope up.
 * If you are having issues waking up the gyroscope, try checking if the mpu_addr in gyro_mpu6050 is the right one by running the i2c scanner by Nick Gammon: http://www.gammon.com.au/forum/?id=10896, the default address is 0x68.
 * So far it has only been tested on an Arduino DUE and is not nearly as leightweight as I want it to be.
 *
 * \section gyro_mpu6050
 * This (hardware focused) library will allow you to wake up your mpu6050 controller, which is always asleep on startup, and extract raw data from it.
 * 
 * \section gyro_dice
 * This (software focused) library will use and work with the data given from the gyro_mpu6050 library, making it usable and turning its data into a dice-like simulation.
 * \subsection gyro_dice_d6
 * Simulates a 6 sided dice. So far uses fixed gyroscope values but I am working on a calibration function.
 * \subsection [NYI]gyro_dice_d20
 * Simulates a 20 sided dice. NYI
 * \subsection gyro_dice_d0
 * Simulates a random number generator dice. Checks wether the dice is shaking or not and once its halted prints a random number on the display.
 * \subsection gyro_dice_m8
 * Simulates a magic 8 ball. CHecks wether the dice is shaking or not and prints a random answer out of 22 possible answers.
 *
 * \section dependencies
 * These libraries are dependant on the HWLIB library which is required to manipulate the pins to our bidding.
 */
/** @file main.cpp */ 
#include "hwlib.hpp"
#include "gyro_mpu6050.hpp"
#include <cmath>
#include "gyro_dice.hpp"
 
/*! \brief main
 * Sets up the oled and gyroscope mpu6050 which then afterwards are
 * used to simulate the dice.
 */
int main( void ){	
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    
    //remove need for hwlib:: for target calls
    namespace target = hwlib::target;
    
    //setup oled window for number printing
    auto oled_scl = target::pin_oc( target::pins::d9 );
    auto oled_sda = target::pin_oc( target::pins::d8 );
    auto oled_i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( oled_scl, oled_sda );
    auto oled_display = hwlib::glcd_oled( oled_i2c_bus, 0x3c );  
    auto oled_font = hwlib::font_default_8x8();
    auto oled_window = hwlib::window_ostream( oled_display, oled_font );

    // setup beeper for sound feedback
    auto beeper = target::pin_out( target::pins::d12 );
    
    // setup mpu6050 gyroscope
    auto gyro_scl = target::pin_oc( target::pins::d10 );
    auto gyro_sda = target::pin_oc( target::pins::d11 );
    auto gyro_i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( gyro_scl, gyro_sda ); 
    auto gyro = gyro_mpu6050( gyro_i2c_bus );
    
    // setup dice, currently available: _d0, _d6, _d20*, _m8*
    // gyro_dice_d0 | 0 sided dice, shake generates a random number
    // auto dice = gyro_dice_d0( gyro, oled_window, beeper, 100 );
    
    // gyro_dice_d6 | 6 sided dice, based on fixed gyroscope data and displays current side on led
    // auto dice = gyro_dice_d6( gyro, oled_window, beeper );
    
    // gyro_dice_m8 | magic 8 ball, shake generates a random answers
    auto dice = gyro_dice_m8( gyro, oled_window, beeper);
    
    // main loop
    while(1){
        dice.update_dice();
        dice.check_side();
        //dice.print_xyz(); // testing purposes
        hwlib::wait_ms(33);
    }
}
