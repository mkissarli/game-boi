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

void player_movement(MPlayer* player)
{
    FIXED movement_speed = 5;
    if(JOYPAD_DOWN_PAD_L){
        //if(player->sprite.speed.x != -movement_speed.x){
        //    player->sprite.speed.x -= 1;
        //}
        player->sprite.speed.x = ITOF(-movement_speed);  //FSUB(player->sprite.speed.x, FDIV(movement_speed.x,FPS));
        //DEBUG_LOG_MESSAGE("speed: %d.%d", FINT(player->sprite.speed.x), FFRAC(player->sprite.speed.x,10));  
        next_animation(&(player->sprite));
    }
    else if(JOYPAD_DOWN_PAD_R){
        //if(player->sprite.speed.x != movement_speed.x){
            //player->sprite.speed.x += 1;
            //}
        player->sprite.speed.x = ITOF(movement_speed);//movement_speed.x;// FADD(player->sprite.speed.x, FDIV(movement_speed.x,FPS));
        DEBUG_LOG_MESSAGE("speed: %d -> %d.%d",
                          FROUNDI(player->sprite.speed.x),
                          FINT(player->sprite.speed.x),
                          FFRAC(player->sprite.speed.x,10));  
        next_animation(&(player->sprite));
    }
    // This could be made more parabolic
    else if(JOYPAD_UP_PAD_L && JOYPAD_UP_PAD_R){
        player->sprite.speed.x = 0;
        /*
        if(player->sprite.speed.x < 0){
            player->sprite.speed.x += 1;
        }
        else if(player->sprite.speed.x > 0){
            player->sprite.speed.x -= 1;
        }
        */
    }
    
    if(JOYPAD_DOWN_PAD_D){
        player->sprite.speed.y = ITOF(movement_speed);//FADD(player->sprite.speed.y, FDIV(movement_speed.y, FPS));
    }
    else if(JOYPAD_UP_PAD_D){
        player->sprite.speed.y =0; //-= movement_speed.y;
    }
}

void jump(MPlayer* player){
    if(JOYPAD_DOWN_A){
        //No jumping off the top.
        if(player->sprite.col.direction.y == -1){
            return;
        }
        if((player->sprite.col.direction.y == 1 ||
            player->sprite.col.direction.x != 0)){
            player->sprite.speed.y = ITOF(-15);
        }
    
        if(player->sprite.col.direction.x == -1){
            player->sprite.speed.x = ITOF(5);
        }
        else if (player->sprite.col.direction.x == 1){
            player->sprite.speed.x = ITOF(-5);
        }
    }
}
