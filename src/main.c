#include <gb/gb.h>
#include <stdio.h>
#include <stdbool.h>
//#include <string.h>
#include "../sprites/char.c"
#include "../src/input.c"
#include "../sprites/backgroundtiles.c"
#include "../sprites/test_map.c"

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

typedef struct MTile
{
    unsigned char value;
    MVector position;
} MTile;

typedef struct MCollision
{
    bool has_collided;
    UINT8 direction; //0 left, 1 right, 2 down, 3 up,
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

void draw(MSprite* sprite){
    move_sprite(sprite->sprite_number, sprite->position.x, sprite->position.y);
}

typedef struct MPlayer
{
    MSprite sprite;
    bool    jumped;
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

    if(sprite->col.has_collided){
        sprite->speed.y = 0;
        //sprite->position.y = 144;
    }
}


void update_position(MSprite* sprite){
    sprite->position.x += sprite->speed.x;
    sprite->position.y += sprite->speed.y;
        
}

void player_movement(MPlayer* player)
{
    MVector movement_speed = {4, 4};
    UPDATE_JOYPAD_STATE;
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
    if(JOYPAD_DOWN_PAD_U && !(player->jumped)){
        player->sprite.speed.y = -5;
        player->jumped = true;
    }
}


void collision_check(MSprite* sprite, unsigned char map[], UINT16 count){
    UINT16 tile_num = 20 * 18;
    sprite->col.has_collided = false;
    //sprite->col.direction = 4;
    for(int i = 0; i < count; ++i){
        if(i % 20 * 8 + 8      <= sprite->position.x + sprite->speed.x + 8 &&
           i % 20 * 8 + 8  + 8 >= sprite->position.x + sprite->speed.x     &&
           i / 20 * 8 + 16     <= sprite->position.y + sprite->speed.y + 8 &&
           i / 20 * 8 + 16 + 8 >= sprite->position.y + sprite->speed.y     &&
           map[i] != 0x00){
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
    
    MPlayer player = {{{70, 30}, 0, 3, 0, {0, 0}, {false, 4}}, false};

    set_sprite_data(player.sprite.sprite_number, player.sprite.max_animations, MainChar);
    set_sprite_tile(player.sprite.sprite_number, player.sprite.animation_index);
    move_sprite(player.sprite.sprite_number, player.sprite.position.x, player.sprite.position.y);
    SHOW_SPRITES;

    UINT16 count = 0;
    UINT16 x_count = 0;
    UINT16 y_count = 0;
    UINT8 map_width = 20;
    UINT8 map_height = 18;
    UINT16 tile_num = map_width * map_height;
 
    /*MTile map[20*18] = {{{0}}};
    
    for(int i = 0; i < tile_num; ++i){
        
        ++x_count;
        if(x_count == map_width){
            x_count = 0;
            ++y_count;
        }
        //if(TestMap[i] == 0x00){
        //        continue;
        //}
        ++count;
            
        MTile t = {TestMap[i], {x_count * 8 + 8, y_count * 8 + 16}};
        map[count] = t;
        }*/
    

    while(1){
        UPDATE_JOYPAD_STATE;
        
        //Update loop
        player_movement(&player);
        jump(&player);
        ++gravity_time;
        if(gravity_time > 6){
            //gravity(&(player.sprite));
            ++gravity_time;
        }

        if(!player.sprite.col.has_collided){update_position(&(player.sprite));}

        draw(&(player.sprite));

        collision_check(&(player.sprite), TestMap, 20*18);
        wait_vbl_done();
    }
}
