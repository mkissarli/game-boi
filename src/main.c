#include <gb/gb.h>
#include <stdio.h>
#include <stdbool.h> 
#include "../sprites/char.c"
#include "../src/input.c"
#include "../sprites/backgroundtiles.c"
#include "../sprites/test_map.c"

INT8 total_sprites = -1;
UINT8 create_sprite_num(){
    ++total_sprites;
    return total_sprites;
}

void performantdelay(UINT8 numloops){
    UINT8 i;
    for(i = 0; i < numloops; i++){
        wait_vbl_done();
    }     
}

typedef struct MVector
{
    UINT8 x;
    UINT8 y;
} MVector;

typedef struct MVel
{
    INT8 x;
    INT8 y;
} MVel;

typedef struct MCollision
{
    bool has_collided;
} MCollision;

typedef struct MSprite
{
    MVector position;
    UINT8   animation_index;
    UINT8   max_animations;
    UINT8   sprite_number;
    MVel    speed;
    MCollision col;
} MSprite;

MSprite* sprites[40] = {0};

void draw(MSprite* sprite){
    move_sprite(sprite->sprite_number, sprite->position.x, sprite->position.y);
}

typedef struct MPlayer
{
    MSprite sprite;
    bool    moved;
    UINT8   movement_delay;
    UINT16  movement_time;
    bool    jumped;
    UINT8   jump_delay;
    UINT16  jump_time;
} MPlayer;

void next_animation(MSprite* sprite){
    ++(sprite->animation_index);
    if(sprite->animation_index == sprite->max_animations){
        sprite->animation_index = 0;
    }
    set_sprite_tile(sprite->sprite_number, sprite->animation_index);
}

static UINT8 GRAVITY = 2;
UINT8 gravity_time = 0;
void gravity(MSprite* sprite){
    //Collision Check
    sprite->speed.y += GRAVITY;

    if(sprite->position.y + sprite->speed.y > 144){
        sprite->speed.y = 0;
        sprite->position.y = 144;
    }
}


void update_position(MSprite* sprite){
    sprite->position.x += sprite->speed.x;
    sprite->position.y += sprite->speed.y;

/*    if(sprite->position.x > 160){
      sprite->position.x = 160;
      }
      else{
      sprite->position.x = 0;
      }

      if(sprite->position.y > 144){
      sprite->position.y = 144;
      }
      else{
      sprite->position.y = 0;
      }*/
}

void player_movement(MPlayer* player)
{
    MVector movement_speed = {4, 4};
    UPDATE_JOYPAD_STATE;
    // if(!(player->moved)){
    if(JOYPAD_DOWN_PAD_L){
        player->sprite.speed.x = -movement_speed.x;
        next_animation(&(player->sprite));
    }
    else if(JOYPAD_DOWN_PAD_R){
        player->sprite.speed.x = movement_speed.x;
        next_animation(&(player->sprite));
    }
    // This could be made more parabolic
    else if(JOYPAD_UP_PAD_L && JOYPAD_UP_PAD_R){
        player->sprite.speed.x = 0;
        /*if(player->sprite.speed.x < 0){
            player->sprite.speed.x += 1;
        }
        else if(player->sprite.speed.x > 0){
            player->sprite.speed.x -= 1;
        }
        */
    }
    
    if(JOYPAD_DOWN_PAD_D){
        player->sprite.speed.y = movement_speed.y;
    }
    else if(JOYPAD_RELEASED_PAD_D){
        player->sprite.speed.y -= movement_speed.y;
    }
}

void jump(MPlayer* player){
    //collision check
    if(player->sprite.col.has_collided == true){
        player->jumped = false;
    }
    
    //if(!(joypad_prev_state & J_UP) && JOYPAD_DOWN_PAD_U && !(player->jumped)){
    if(JOYPAD_DOWN_PAD_U && !(player->jumped)){
        player->sprite.speed.y = -5;
        player->jumped = true;
    }
}

void collision_check(MSprite* sprite, unsigned char map[]){
    UINT16 tile_num = 20 * 18;
    sprite->col.has_collided = false;
    for(int i = 0; i < tile_num; ++i){
        if(map[i] == 0x00){ continue; }
        else if (i % 20 == sprite->position.x / 8 &&
                 //sprite->position.x <= i ||
                 i/20 == sprite->position.y / 8) {
                 //sprite->position.y <= ((i/20) * 8)){
            sprite->col.has_collided = true;
            break;
        }
    }
    
}

void main()
{
    set_bkg_data(0, 3, backgroundtiles);
    set_bkg_tiles(0,0,20,18, TestMap);

    SHOW_BKG;
    
    MPlayer player = {{{88, 78}, 0, 3, 0, {0, 0}, {false}}, false, 6, 0, false, 60, 0};
    sprites[0] = &(player.sprite);
    set_sprite_data(player.sprite.sprite_number, player.sprite.max_animations, MainChar);
    set_sprite_tile(player.sprite.sprite_number, player.sprite.animation_index);
    move_sprite(player.sprite.sprite_number, player.sprite.position.x, player.sprite.position.y);
    SHOW_SPRITES;
    UINT8 movement = 0;

    while(1){
        UPDATE_JOYPAD_STATE;
        
        //Update loop
        player_movement(&player);
        jump(&player);
        ++gravity_time;
        for(UINT8 i = 0; i < 40; ++i){
            if(sprites[i] == 0){break;}
            if(gravity_time > 6) { gravity(sprites[i]); }
            update_position(sprites[i]);
        }
        if(gravity_time > 6){
            gravity_time = 0;
        }
        
        //Draw loop
        for(UINT8 i = 0; i < 40; ++i){
            if(sprites[i] == 0){break;}
            draw(sprites[i]);
        }

        // Collision Check?
        //if(player.sprite.position.y < 144){
        //    player.sprite.col.has_collided = false;
        // }
        //else {
        //    player.sprite.col.has_collided = true;
        //}

        collision_check(sprites[0], TestMap);
        wait_vbl_done();
    }
}
