#include <gb/gb.h>
#include <stdio.h>
#include <stdbool.h> 
#include "../sprites/char.c"
#include "../src/input.c"

void gravity(){
    
}

UINT8 total_sprites = 0;

typedef struct MSprite
{
    UINT8 x;
    UINT8 y;
    UINT8 animation_index;
    UINT8 max_animations;
    UINT8 sprite_number;
    UINT8 speed;
} MSprite;   

typedef struct MPlayer
{
    MSprite sprite;
    bool    moved;
    UINT8   movement_delay;
    UINT16   movement_time;
} MPlayer;

void move_player(MPlayer* player, UINT8 move, bool is_x){
    player->moved = true;
    if(is_x == true){
        player->sprite.x += move;
        scroll_sprite(player->sprite.sprite_number, move, 0);        
    }
    else {
        player->sprite.y += move;
        scroll_sprite(player->sprite.sprite_number, 0, move);
    }
}
void player_movement(MPlayer* player)
{
    if(!(player->moved)){
        if(JOYPAD_DOWN_PAD_L){
            move_player(player, -player->sprite.speed, true);
        }
        else if(JOYPAD_DOWN_PAD_R){
            move_player(player, player->sprite.speed, true);
        }
        if(JOYPAD_DOWN_PAD_U){
            move_player(player, -player->sprite.speed, false);
        }
        else if(JOYPAD_DOWN_PAD_D){
            move_player(player, player->sprite.speed, false);
        }
    }
    if(player->movement_time > player->movement_delay){
        player->movement_time = 0;
        player->moved = false;
    }

    ++(player->movement_time);
    
}

void main()
{
    UPDATE_JOYPAD_STATE;
    
    MPlayer player = {{88, 78, 0, 3, 0, 10}, false, 60, 0};
    UINT8 current_sprite_index = 0;
    
    set_sprite_data(player.sprite.sprite_number, player.sprite.max_animations, MainChar);
    set_sprite_tile(player.sprite.sprite_number, player.sprite.animation_index);
    move_sprite(player.sprite.sprite_number, player.sprite.x, player.sprite.y);
    SHOW_SPRITES;
    UINT8 movement = 0;

    while(1){
        player_movement(&player);
    }
}
