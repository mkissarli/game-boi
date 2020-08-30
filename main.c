#include <gb/gb.h>
#include <stdio.h>
#include "./sprites/char.c"


void next_sprite(UINT8 sprite_index, UINT8* current_sprite_index){
    ++(*current_sprite_index);
    if(*current_sprite_index == 3){
        *current_sprite_index = 0;
    }
    
    set_sprite_tile(sprite_index, *current_sprite_index);
}

void main()
{
    UINT8 current_sprite_index = 0;
    UINT8 main_char = 0;
    set_sprite_data(main_char, 3, MainChar);
    set_sprite_tile(main_char, 0);
    move_sprite(main_char, 88, 78);
    SHOW_SPRITES;
    UINT8 movement = 0;

    while(1){
        switch(joypad()){
        case J_LEFT:
            scroll_sprite(main_char, -10, 0);
            next_sprite(main_char, &current_sprite_index);
            movement = 1;
            delay(100);
            break;
        case J_RIGHT:
            scroll_sprite(main_char, 10, 0);
            next_sprite(main_char, &current_sprite_index);
            movement = 1;
            delay(100);
            break;
        case J_UP:
            scroll_sprite(main_char, 0, -10);
            next_sprite(main_char, &current_sprite_index);
            movement = 1;
            delay(100);
            break;
        case J_DOWN:
            scroll_sprite(main_char, 0, 10);
            next_sprite(main_char, &current_sprite_index);
            movement = 1;
            delay(100);
            break;
        }
    }
}
