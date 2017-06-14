#ifndef GYRO_FONT_HPP
#define GYRO_FONT_HPP

uint16_t gyro_font[7] = {0b1111011011110000,
                        0b0101100101110000,
                        0b1110011101110000,
                        0b1110010111110000,
                        0b1011110010010000,
                        0b1111000111110000,
                        0b1111001111110000};

void cubePrint(auto display, int size, int loc_x, int loc_y){
   for( uint16_t x = loc_x; x < (loc_x + size); x++ ){
       for( uint16_t y = loc_y; y < (loc_y + size); y++){
           display.write( hwlib::location( x, y ), hwlib::black );
       }
   }
}

void fontPrint(auto display, int number, int loc_x, int loc_y, int scale){
    //display.clear();
    int loc_x_or = loc_x;
    uint16_t numberByte = gyro_font[number];
    numberByte >>= 4 ;
    hwlib::cout << "drawing number: " << number << "|byte: " << int(gyro_font[number]);
    hwlib::cout << "|confirm: ";
    for(int i = 0; i < 4; i++){
        for(int n = 0; n < 3; n++){
            uint16_t bit = numberByte & 1;
            if(bit == 1){
                hwlib::cout << 1 ;
                cubePrint(display, scale, loc_x, loc_y);
            } else {
                hwlib::cout << 0 ;
            }
            loc_x += scale*2;
            numberByte >>= 1 ;
        }
        loc_y += scale*2;
        loc_x = loc_x_or;
    }
    hwlib::cout << "\n";

}
#endif // GYRO_FONT_HPP
