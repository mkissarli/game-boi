#include <gb/gb.h>
#include <stdio.h>
#include <stdbool.h> 
#include "../sprites/char.c"
#include "../src/input.c"


UINT8 total_sprites = -1;
UINT8 create_sprite_num(){
    ++total_sprites;
    return total_sprites;
}

typedef struct MSprite
{
    UINT8 x;
    UINT8 y;
    UINT8 animation_index;
    UINT8 max_animations;
    UINT8 sprite_number;
    UINT8 speed;
} MSprite;

MSprite* sprites[40];

void draw(MSprite* sprite){
    move_sprite(sprite->sprite_number, sprite->x, sprite->y);
}

typedef struct MPlayer
{
    MSprite sprite;
    bool    moved;
    UINT8   movement_delay;
    UINT16  movement_time;
} MPlayer;

void next_animation(MSprite* sprite){
    ++(sprite->animation_index);
    if(sprite->animation_index == sprite->max_animations){
        sprite->animation_index = 0;
    }
    set_sprite_tile(sprite->sprite_number, sprite->animation_index);
}

static UINT8 GRAVITY = 9;
UINT8 gravity_time = 0;
void gravity(MSprite* sprite){
    //Collision Check
    sprite->y += 1;
}

void move_player(MPlayer* player, UINT8 move, bool is_x){
    player->moved = true;
    if(is_x == true){
        player->sprite.x += move;       
    }
    else {
        player->sprite.y += move;
    }
    next_animation(&(player->sprite));
    //draw(&(player->sprite));
}


void check_collision_with_wall(){
    
}

void create_level(){
    
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
    
    MPlayer player = {{88, 78, 0, 3, 0, 5}, false, 6, 0};
    sprites[0] = &(player.sprite);
    set_sprite_data(player.sprite.sprite_number, player.sprite.max_animations, MainChar);
    set_sprite_tile(player.sprite.sprite_number, player.sprite.animation_index);
    move_sprite(player.sprite.sprite_number, player.sprite.x, player.sprite.y);
    SHOW_SPRITES;
    UINT8 movement = 0;

    while(1){
        //Update loop
        player_movement(&player);
        /*++gravity_time;
        for(UINT8 i = 0; i < 40; ++i){
            if(gravity_time > 6) { gravity(sprites[i]); }
        }
        gravity_time = 0;
        */
        //Draw loop
        for(UINT8 i = 0; i < 40; ++i){
            draw(sprites[i]);
        //    move_sprite(0, sprites[i]->x, sprites[i]->y);
        }
        
        //draw(sprites[0]);
        wait_vbl_done();
    }
}
