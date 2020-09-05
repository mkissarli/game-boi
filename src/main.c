
#define DEBUG_MODE 1

#include <gb/gb.h>
#include <stdio.h>
#include <stdbool.h>
#include "../sprites/char.c"
#include "../src/input.c"
#include "../src/debug.c"

#include "./tuple_types.c"
#include "./sprites.c"
#include "./player.c"
#include "./maps.c"
#include "./physics.c"

void performantdelay(UINT8 numloops){
    UINT8 i;
    for(i = 0; i < numloops; i++){
        wait_vbl_done();
    }     
}

void main()
{
    set_bkg_data(0, 5, BackgroundTiles);
    
    SHOW_BKG;
    
    MPlayer player = {{{70, 24}, 0, 3, 0, {0, 0}, {false, {0, 0}}}, false};

    set_sprite_data(player.sprite.sprite_number, player.sprite.max_animations, MainChar);
    set_sprite_tile(player.sprite.sprite_number, player.sprite.animation_index);
    move_sprite(player.sprite.sprite_number, player.sprite.position.x, player.sprite.position.y);
    SHOW_SPRITES;

    set_map(&player);
    
    while(1){
        UPDATE_JOYPAD_STATE;
        //Update loop

        // Player
        player_movement(&player);
        jump(&player);

        // Collisions
        death_check(&player);
        collision_check(&(player.sprite));
        
        // Gravity
        //++gravity_time;
        //if(gravity_time > 3){
        gravity(&(player.sprite));
        //++gravity_time;
        //}
        
        // Draw
        update_position(&(player.sprite));
        draw(&(player.sprite));


        win_condition(&player);
        
        // V-Sync
        //wait_vbl_done();
        performantdelay(5);
    }
}
