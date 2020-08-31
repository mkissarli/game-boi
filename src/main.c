#include <gb/gb.h>
#include <stdio.h>
#include "../sprites/char.c"

//UINT8[] sprites = UINT8[];

void gravity(){
    
}

UINT8 total_sprites = 0;

typedef struct MSprite {
    UINT8 x;
    UINT8 y;
    UINT8 animation_index;
    UINT8 max_animations;
    UINT8 sprite_number;
} MSprite;   

/*void next_sprite(MSprite* s){
    ++(*s.animation_index);
    if(*s.animation_index == *s.max_animations){
        s.animation_index = 0;
    }
    set_sprite_tile(*s.sprite_number, *s.animation_index);        
}
/*
void next_sprite(UINT8 sprite_index, UINT8* current_sprite_index){
    ++(*current_sprite_index);
    if(*current_sprite_index == 3){
        *current_sprite_index = 0;
    }
    
    set_sprite_tile(sprite_index, *current_sprite_index);
}
*/
void main()
{
    MSprite main_char = {88, 78, 0, 3, 0};
    UINT8 current_sprite_index = 0;
//    UINT8 main_char = 1;
    set_sprite_data(main_char.sprite_number, main_char.max_animations, MainChar);
    set_sprite_tile(main_char.sprite_number, main_char.animation_index);
    move_sprite(main_char.sprite_number, main_char.x, main_char.y);
    SHOW_SPRITES;
    UINT8 movement = 0;

    while(1){
        switch(joypad()){
        case J_LEFT:
            scroll_sprite(main_char.sprite_number, -10, 0);
            //next_sprite(main_char, &current_sprite_index);
            movement = 1;
            delay(100);
            break;
        case J_RIGHT:
            scroll_sprite(main_char.sprite_number, 10, 0);
            //next_sprite(main_char, &current_sprite_index);
            movement = 1;
            delay(100);
            break;
        case J_UP:
            scroll_sprite(main_char.sprite_number, 0, -10);
            //next_sprite(main_char, &current_sprite_index);
            movement = 1;
            delay(100);
            break;
        case J_DOWN:
            scroll_sprite(main_char.sprite_number, 0, 10);
            //next_sprite(main_char, &current_sprite_index);
            movement = 1;
            delay(100);
            break;
        }
    }
}
