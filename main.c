#include <gb/gb.h>
#include <stdio.h>
#include "./sprites/char.c"

void main(){
    UINT8 current_sprite_index = 0;
    UINT8 main_char = 0;
    set_sprite_data(main_char, 3, MainChar);
    set_sprite_tile(main_char, 0);
    move_sprite(main_char, 88, 78);
    SHOW_SPRITES;


    while(1){
        switch(joypad()){
        case J_LEFT:
            scroll_sprite(main_char, 10, 0);
        case J_RIGHT:
            scroll_sprite(main_char, -10, 0);
        case J_UP:
            scroll_sprite(main_char, 0, 10);
        case J_DOWN:
            scroll_sprite(main_char, 0, -10);
        }
        
        current_sprite_index = current_sprite_index + 1;
        if(current_sprite_index == 3){
            current_sprite_index = 0;
        }

        set_sprite_tile(main_char, current_sprite_index);
        delay(1000);
        
    }
}
