
#define DEBUG_MODE 1

#include <gb/gb.h>
#include <stdio.h>
#include <stdbool.h>
#include "../sprites/char.c"
#include "../src/input.c"
#include "../src/debug.c"

#include "../sprites/background.c"
#include "../maps/map1.c"

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

typedef struct MTile
{
    unsigned char value;
    MVector position;
} MTile;

typedef struct MCollision
{
    bool has_collided;
    MVel direction; // Numpad 5 == no collision
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

static UINT8 GRAVITY = 1;
UINT8 gravity_time = 0;
void gravity(MSprite* sprite){
    //Collision Check
    if(sprite->col.direction.y != 1){
        sprite->speed.y += GRAVITY;
    }
}


void update_position(MSprite* sprite){
    if(!(sprite->col.direction.x == -1 && sprite->speed.x < 0) &&
       !(sprite->col.direction.x ==  1 && sprite->speed.x > 0)){
        sprite->position.x += sprite->speed.x;
    }
    if(!(sprite->col.direction.y == -1 && sprite->speed.y < 0) &&
       !(sprite->col.direction.y ==  1 && sprite->speed.y > 0)){
        sprite->position.y += sprite->speed.y;
    }
}

void player_movement(MPlayer* player)
{
    MVector movement_speed = {4, 4};
    UPDATE_JOYPAD_STATE;
    if(JOYPAD_DOWN_PAD_L){
        //if(player->sprite.speed.x != -movement_speed.x){
        //    player->sprite.speed.x -= 1;
        //}
        player->sprite.speed.x = -movement_speed.x;
        next_animation(&(player->sprite));
    }
    else if(JOYPAD_DOWN_PAD_R){
        //if(player->sprite.speed.x != movement_speed.x){
            //player->sprite.speed.x += 1;
            //}
        player->sprite.speed.x = movement_speed.x;
        next_animation(&(player->sprite));
    }
    // This could be made more parabolic
    else if(JOYPAD_UP_PAD_L && JOYPAD_UP_PAD_R){
        //player->sprite.speed.x = 0;
        if(player->sprite.speed.x < 0){
            player->sprite.speed.x += 1;
        }
        else if(player->sprite.speed.x > 0){
            player->sprite.speed.x -= 1;
        }
        
    }
    
    if(JOYPAD_DOWN_PAD_D){
        player->sprite.speed.y = movement_speed.y;
    }
    else if(JOYPAD_UP_PAD_D){
        player->sprite.speed.y =0; //-= movement_speed.y;
    }
}

void jump(MPlayer* player){
    //if(player->sprite.col.has_collided == true){
    //    player->jumped = false;
    //}
    
    //DEBUG_LOG_MESSAGE("Collision: %d %d", player->sprite.col.direction.x, player->sprite.col.direction.y);
    if(JOYPAD_DOWN_A){
        if((player->sprite.col.direction.y == 1 ||
            player->sprite.col.direction.x != 0)){
            player->sprite.speed.y = -15;
        }
    
        if(player->sprite.col.direction.x == -1){
            DEBUG_LOG_MESSAGE("Left Side Collision!");
            player->sprite.speed.x = 5;
        }
        else if (player->sprite.col.direction.x == 1){
            player->sprite.speed.x = -5;
        }
    }
}

UINT16 get_world_to_map(MVector* pos){
    return (pos->x - 8 )/8 + ((pos->y - 16) /8 )* 20;
}

#define START_TILE 0x02
#define FLAG_TILE  0x01
#define EMPTY_TILE 0x00

void collision_check_2(MSprite* sprite, unsigned char map[]){
    UINT16 pos = get_world_to_map(&(sprite->position));
    
    sprite->col.has_collided = false;
    sprite->col.direction.x = 0;
    sprite->col.direction.y = 0;
    // Left
    if(pos - 1 % 20 == 0 ||
       (map[pos - 1] != EMPTY_TILE &&
        map[pos - 1] != FLAG_TILE  &&
        map[pos - 1] != START_TILE)){
        sprite->col.direction.x = -1;
        sprite->col.has_collided = true;
    }
    // Right
    if(pos + 1 % 20 == 19 ||
       (map[pos + 1] != EMPTY_TILE &&
        map[pos + 1] != FLAG_TILE  &&
        map[pos + 1] != START_TILE)){
        sprite->col.direction.x = 1;
        sprite->col.has_collided = true;
    }
    // Up
    if(pos - 20 < 0 ||
       (map[pos - 20] != EMPTY_TILE &&
        map[pos - 20] != FLAG_TILE  &&
        map[pos - 20] != START_TILE)){
        sprite->col.direction.y = -1;
        sprite->col.has_collided = true;
    }
    // Down
    if(pos + 20 > 20 * 18 || 
       (map[pos + 20] != EMPTY_TILE &&
        map[pos + 20] != FLAG_TILE  &&
        map[pos + 20] != START_TILE)){
        sprite->col.direction.y = 1;
        sprite->col.has_collided = true;
    }
}

void set_map(unsigned char map[], MPlayer* player){
    set_bkg_tiles(0,0,20,18,map);
    // Start from bottom as more likely the start is near the bottom
    for(UINT16 i = 20 * 18 - 1; i >= 0; --i){
        if(map[i] == START_TILE){
            player->sprite.position.x = i % 20 * 8 + 8;
            player->sprite.position.y = i / 20 * 8 + 16;
            break;
        }
    }

    update_position(&(player->sprite));
}

BYTE win_condition(unsigned char map[], MPlayer* player){
    if(map[get_world_to_map(&(player->sprite.position))] == FLAG_TILE){
        DEBUG_LOG_MESSAGE("You did a win!!");
        return 1;
    }
    return 0;
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

    set_map(Map1, &player);
    
    while(1){
        UPDATE_JOYPAD_STATE;
        //Update loop

        // Player
        player_movement(&player);
        jump(&player);

        // Collisions
        collision_check_2(&(player.sprite), Map1);
        
        // Gravity
        //++gravity_time;
        //if(gravity_time > 3){
        gravity(&(player.sprite));
        //++gravity_time;
        //}
        
        // Draw
        update_position(&(player.sprite));
        draw(&(player.sprite));


        win_condition(Map1, &player);
        
        // V-Sync
        wait_vbl_done();
        //performantdelay(5);
    }
}
