
#define DEBUG_MODE 1

#include <gb/gb.h>
#include <stdio.h>
#include <stdbool.h>
#include "../sprites/char.c"
#include "../sprites/background.c"

#include "../src/globals.h"

#include "../src/math.c"
#include "../src/input.c"
#include "../src/debug.c"

#include "./tuple_types.c"
#include "./sprites.c"
#include "./player.c"
#include "./maps.c"
#include "./physics.c"

//#include "./bank.c"
//#include "./bank2.c"
//#include "./bank3.c"

#include "../sprites/splash_data.c"
#include "../sprites/splash_map.c"

void performantdelay(UINT8 numloops){
    UINT8 i;
    for(i = 0; i < numloops; i++){
        wait_vbl_done();
    }     
}


void main()
{
    // Splash screen
    set_bkg_data(0, 114, splash_data);
    set_bkg_tiles(0, 0, 20, 18, splash_map);

    SHOW_BKG;
    DISPLAY_ON;

    waitpad(J_START);
    
    // Load Actual game stuff
    set_bkg_data(0, 12, BackgroundTiles);

    MPlayer player = {{{70, 24}, 0, 3, 0, {0, 0}, {false, {0, 0}}}, false};

    set_sprite_data(player.sprite.sprite_number, player.sprite.max_animations, MainChar);
    set_sprite_tile(player.sprite.sprite_number, player.sprite.animation_index);
    move_sprite(player.sprite.sprite_number, player.sprite.position.x, player.sprite.position.y);
    SHOW_SPRITES;

    reset_map(1);
    set_map(&player);
    //SWITCH_ROM_MBC1(0);

    // Game Loop
    while(1){
        UPDATE_JOYPAD_STATE;
        //Update loop

        // Player
        player_movement(&player);
        jump(&player);

        // Collisions
        death_check(&player);
        teleporter_check(&player);
        collision_check(&(player.sprite));
        
        // Gravity
        gravity(&(player.sprite));
        
        // Draw
        update_position(&(player.sprite));
        draw(&(player.sprite));
        
        win_condition(&player);
        
        // V-Sync
        //wait_vbl_done();
        performantdelay(5);
    }
}
