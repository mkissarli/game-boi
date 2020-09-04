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
