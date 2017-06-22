/** @file gyro_dice.hpp */ 
#ifndef GYRO_DICE_HPP
#define GYRO_DICE_HPP
#define degconvert 57.2957786 // value to convert radians into degrees
#define display_width 128
#define display_height 64

/*! \brief gyro_dice, Default dice simulation
 */
class gyro_dice
{
private:
    // dice variables
    int acX, acY, acZ;  // accelerometer
    int gyX, gyY, gyZ;  // gyroscope
    int roll, pitch;    // rotation
    int dif = 66000;    // !not really sure why I am still using this
    int mut = 100;      // multiplier
    
    // gyroscope, display, beeper
    gyro_mpu6050 gyro;
    hwlib::window_ostream display;
    hwlib::target::pin_out beeper;
    
public:
    /*! \brief gyro_dice
     * gyrodice constructor, requests a gyroscope, display and a beeper. 
     * Updates the dice on initialization.
     */
    gyro_dice( gyro_mpu6050 gyro, hwlib::window_ostream display, hwlib::target::pin_out beeper ) 
        : gyro (gyro), display (display), beeper (beeper) {
        update_dice();
    }
    
    /*! \brief update_dice
     * grabs gyroscope values and works them
     */
    void update_dice(){
        gyro.update_all();
        acX = (dif - gyro.get_accelX())/mut;
        acY = (dif - gyro.get_accelY())/mut;
        acZ = (dif - gyro.get_accelZ())/mut;
        gyX = (dif - gyro.get_gyroX())/mut;
        gyY = (dif - gyro.get_gyroY())/mut;
        gyZ = (dif - gyro.get_gyroZ())/mut;
        roll = atan2(acY, acZ)*degconvert;
        pitch = atan2(-acX, sqrt(acY*acY + acZ*acZ))*degconvert;
    }
    
    /*! \brief beep_dice
     * sends out a beep
     */
    void beep_dice(){
        beeper.set(1);
        hwlib::wait_ms(100);
        beeper.set(0);
        hwlib::wait_ms(100);
    }
    
    /*! \brief print_xyz
     * prints accelX, accelY, accelZ, roll & pitch variables on the display
     */
    void print_xyz(){
        display << "\f"
            << "acX: " << acX << "\n" 
            << "acY: " << acY << "\n" 
            << "acZ: " << acZ << "\n"
            << "rol: " << roll << "\n"
            << "pit: " << pitch << "\n"
            << hwlib::flush; 
    }
    
    /*! \brief print_text
     * prints text on the display
     */
    void print_text(char str[]){
        int strLen = 16;
        char lines[4][strLen];
        for(int i = 0; i < 4; i++){
            for(int n = 0; n < strLen; n++){
                lines[i][n] = str[(i*strLen) + n];
            }
        }
        display << "\f"
            << lines[0] << "\n"
            << lines[1] << "\n"
            << lines[2] << "\n"
            << lines[3] << "\n"
            << hwlib::flush;
    }
    
    /*! \brief print_number
     * prints a number on the display
     */
    void print_number(int i){
        display << "\f"
            << i
            << hwlib::flush;
    }
    
    virtual void check_side(){};
    virtual void set_side(){};
    
    // getters
    int get_acX(){ return acX; } ///< returns acX
    int get_acY(){ return acY; } ///< returns acY
    int get_acZ(){ return acZ; } ///< returns acZ
};

/*! \brief gyro_dice_d6:gyro_dice, d6 simulation
 */
class gyro_dice_d6: public gyro_dice{
private:
    // side coordinates for a 6 sided dice
    int d6[6][5] = {{488, 6, 650, 0, -36},      //1
                    {650, 170, 7, 87, -75},     //2
                    {625, 656, 160, 75, -42},   //3
                    {10, 10, 490, 0, -14},      //4
                    {658, 497, 650, 37, -37},   //5
                    {160, 650, 10, 22, -80}};   //6
                    
    int curSide = 0;
    int sens = 135; // sensitivity of snapping to a side
                    
public:
    /*! \brief gyro_dice_d6
     * gyrodice d6 constructor, requests a gyroscope, display and a beeper. 
     */
    gyro_dice_d6( gyro_mpu6050 gyro, hwlib::window_ostream display, hwlib::target::pin_out beeper ) 
    : gyro_dice( gyro, display, beeper ){}
    
    /*! \brief set_side
     * sets the current side of the dice to a new one.
     */
    void set_side(int side){
        if(side != curSide){
            curSide = side;
            print_number(side);
            for(int i = 0; i < side; i++){
                beep_dice();
            }
        }
    }
    
    /*! \brief check_side
     * compares acX, acY and acZ coordinates to the dice sides
     */
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
    
    /*! \brief calibrate
     * [NYI] dynamically recalibrate the dice
     */
    void calibrate(){};
};


/*! \brief gyro_dice_d0:gyro_dice, random number generator
 */
class gyro_dice_d0: public gyro_dice
{
private:
    int maxSides = 0;
    bool seedActivated = false;
    int pacX, pacY, pacZ;
    int sens = 100;
public:
    /*! \brief gyro_dice_d0
     * gyrodice d0 constructor, requests a gyroscope, display and a beeper. 
     * On initialization starts the internal arduino clock.
     */
    gyro_dice_d0( gyro_mpu6050 gyro, hwlib::window_ostream display, hwlib::target::pin_out beeper, int maxSides ) 
    : gyro_dice( gyro, display, beeper ), maxSides (maxSides){
        hwlib::now_us();
        pacX = get_acX(),
        pacY = get_acY(),
        pacZ = get_acZ();
    }
    
    /*! \brief check_side
     * Checks wether a seed has already been generated, if not then generates a seed based on arduino clock.
     * Afterwards checks if the device is being shaken. if it is, prints a random number based on dice sides.
     */
    void check_side(){
        if( seedActivated == false ){
            srand(hwlib::now_us());
            seedActivated = true;
        }
        int cv[3] = {get_acX(), get_acY(), get_acZ()};
        int pv[3] = {pacX, pacY, pacZ};
        if( cv[0] > pv[0]+sens || cv[0] < pv[0]-sens ){
            if( cv[1] > pv[1]+sens || cv[1] < pv[1]-sens ){
                if( cv[2] > pv[2]+sens || cv[2] < pv[2]-sens ){
                    print_number( rand() % maxSides + 1 );
                    beep_dice();
                    hwlib::wait_ms(1000); // blocks repeated shakes when shaking
                }
            }
        }
        
        pacX = get_acX();
        pacY = get_acY();
        pacZ = get_acZ();
    }
};


/*! \brief gyro_dice_m8:gyro_dice, magic 8 ball
 */
class gyro_dice_m8: public gyro_dice
{
private:
    char answers[22][50] = {
        {"it is certain."},
        {"It is decidedly so."},
        {"Without a doubt."},
        {"Yes - definitely."},
        {"You may rely on it."},
        {"As I see it, yes."},
        {"Most likely."},
        {"Outlook: good."},
        {"Yes."},
        {"Signs point to yes."},
        {"Reply hazy, try again."},
        {"Ask again later."},
        {"Better not tell you now."},
        {"Cannot predit now."},
        {"Concentrate and ask again."},
        {"Don't count on it."},
        {"My reply is no."},
        {"My sources say no."},
        {"Outlook: not so good."},
        {"Very doubtful."},
        {"404, try again later."},
        {"42."}
    };
    bool seedActivated = false;
    int pacX, pacY, pacZ;
    int sens = 100;
public:
    /*! \brief gyro_dice_m8
     * gyrodice d0 constructor, requests a gyroscope, display and a beeper. 
     * On initialization starts the internal arduino clock.
     */
    gyro_dice_m8( gyro_mpu6050 gyro, hwlib::window_ostream display, hwlib::target::pin_out beeper ) 
    : gyro_dice( gyro, display, beeper ){
        hwlib::now_us();
        pacX = get_acX(),
        pacY = get_acY(),
        pacZ = get_acZ();
    }
    
    void check_side(){
        if( seedActivated == false ){
            srand(hwlib::now_us());
            seedActivated = true;
        }
        /*! \brief check_side
         * Checks wether a seed has already been generated, if not then generates a seed based on arduino clock.
         * Afterwards checks if the device is being shaken. if it is, prints a one of 22 random answers from the answers array.
         */
        int cv[3] = {get_acX(), get_acY(), get_acZ()};
        int pv[3] = {pacX, pacY, pacZ};
        if( cv[0] > pv[0]+sens || cv[0] < pv[0]-sens ){
            if( cv[1] > pv[1]+sens || cv[1] < pv[1]-sens ){
                if( cv[2] > pv[2]+sens || cv[2] < pv[2]-sens ){
                    print_text(answers[(rand() % 22)]);
                    beep_dice();
                    hwlib::wait_ms(1000); // blocks repeated shakes when shaking
                }
            }
        }
        
        pacX = get_acX();
        pacY = get_acY();
        pacZ = get_acZ();
    }
};

#endif // GYRO_DICE_HPP
