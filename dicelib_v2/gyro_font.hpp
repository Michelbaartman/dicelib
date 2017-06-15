#ifndef GYRO_FONT_HPP
#define GYRO_FONT_HPP
#define display_width 128
#define display_height 64

uint16_t gyro_font[20] = {  0b1111011011110000, //0
                            0b0101100101110000, //1
                            0b1110011101110000, //2
                            0b1110010111110000, //3
                            0b1011110010010000, //4
                            0b1111000111110000, //5
                            0b1111001111110000, //6
                            0b1110010101000000, //7
                            0b1111011111110000, //8
                            0b1111110011110000, //9
                            0b1111000111110000, //S
                            0b1001001111010000, //H
                            0b0101011111010000, //A
                            0b1011101101010000, //K
                            0b1111001101110000, //E
                            0b0100100000100000, //!
                            };

void cubePrint(auto display, int size, int loc_x, int loc_y){
    for( int y = loc_y; y < loc_y + size; y++){
        for( int x = loc_x; x < loc_x + size; x++){
            display.write( hwlib::location( x, y ), hwlib::black );
        }
    }
}

void print_font(auto display, int number, int loc_x, int loc_y, int scale){
    int loc_x_or = loc_x+(scale*3);
    loc_x = loc_x_or;
    uint16_t numberByte = gyro_font[number];
    numberByte >>= 4;
    for(int i = 0; i < 4; i++){
        for(int n = 0; n < 3; n++){
            uint16_t bit = numberByte & 1;
            if(bit == 1){
                cubePrint(display, scale, loc_x, loc_y);
            }
            loc_x -= scale*2;
            numberByte >>= 1 ;
        }
        loc_y -= scale*2;
        loc_x = loc_x_or;
    }
}

void print_shake(auto display){
    display.clear();
    int scale = 2;
    int scaleOffset = scale * 4;
    int start_loc_y = display_height/2 + scaleOffset;
    int start_loc_x = 0 + display_width*0.1;
    for(int i = 10; i < 16; i++){
        print_font(display, i, start_loc_x, start_loc_y, scale);
        start_loc_x += 20;
    }

    //cubePrint(display, 20, 5, 5);
}
#endif // GYRO_FONT_HPP
