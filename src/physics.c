#define GRAVITY ITOF(54)
UINT8 gravity_time = 0;
void gravity(MSprite* sprite){
    //Collision Check
    if(sprite->col.direction.y != 1 &&
       FINT(sprite->speed.y) <= 8){
        sprite->speed.y =  FADD(sprite->speed.y, FDIV(GRAVITY, ITOF(FPS)));
        //DEBUG_LOG_MESSAGE("ACC: %d.%d", FINT(FDIV(GRAVITY, ITOF(FPS))), FFRAC(FDIV(GRAVITY, ITOF(FPS)), 10))
        //DEBUG_LOG_MESSAGE("sprite y speed: %d.%d", FINT(sprite->speed.y), FFRAC(sprite->speed.y, 10));
    }
}

#define COL_DEATH_CHECK(x) \
    (GLOBAL_MAP[pos + x] == SPIKE_U ||         \
     GLOBAL_MAP[pos + x] == SPIKE_D ||         \
     GLOBAL_MAP[pos + x] == SPIKE_L ||         \
     GLOBAL_MAP[pos + x] == SPIKE_R)           \

#define COL_CHECK(x)\
    (GLOBAL_MAP[pos + x] != EMPTY_TILE &&       \
     GLOBAL_MAP[pos + x] != FLAG_TILE  &&       \
     GLOBAL_MAP[pos + x] != START_TILE &&       \
     GLOBAL_MAP[pos + x] != TELEPORTER_TILE &&  \
     GLOBAL_MAP[pos + x] != TELEPORTER2_TILE && \
     GLOBAL_MAP[pos + x] != TELEPORTER3_TILE && \
     !COL_DEATH_CHECK(x))                       \


UINT16 round_up(UINT16 num, UINT16 multiple)
{
    if (multiple == 0){
        return num;
    }
    
    UINT16 remainder = num % multiple;
    if (remainder == 0){
        return num;
    }
    return num + multiple - remainder;
}

UINT16 round_down(UINT16 num, UINT16 multiple){
    if (multiple == 0){
        return num;
    }
    
    UINT16 remainder = num % multiple;
    if (remainder == 0){
        return num;
    }
    return num - remainder;
}

void collision_check(MSprite* sprite){
    UINT16 pos = get_world_to_map(sprite->position.x, sprite->position.y);
    
    sprite->col.has_collided = false;
    sprite->col.direction.x = 0;
    sprite->col.direction.y = 0;
    SWITCH_ROM_MBC5(current_world);
    // Left
    if(pos - 1 % 20 == 0 ||
       COL_CHECK(-1)){
        SWITCH_ROM_MBC5(0);
        sprite->col.direction.x = -1;
        sprite->col.has_collided = true;
        sprite->position.x = round_up(sprite->position.x, 8);//(((sprite->position.x - 8) >> 3) * 8) + 8;
    }
    // Right
    if(pos + 1 % 20 == 19 ||
       COL_CHECK(1)){
        SWITCH_ROM_MBC5(0);
        sprite->col.direction.x = 1;
        sprite->col.has_collided = true;
      
        sprite->position.x = round_down(sprite->position.x, 8);//(((sprite->position.x - 8) >> 3) * 8) + 8;
    }
    // Up
    if(pos - 20 < 0 ||
       COL_CHECK(-20)){
        SWITCH_ROM_MBC5(0);
        sprite->col.direction.y = -1;
        sprite->col.has_collided = true;        
        sprite->position.y = round_up(sprite->position.y, 8);//(((sprite->position.y - 16) >> 3) * 8) + 16;
    }
    // Down
    if(pos + 20 > 20 * 18 ||
       COL_CHECK(20)){
        SWITCH_ROM_MBC5(0);
        sprite->col.direction.y = 1;
        sprite->col.has_collided = true;
        sprite->position.y = round_down(sprite->position.y, 8);//sprite->position.y = (((sprite->position.x - 16) >> 3) * 8) + 16;
    }
    SWITCH_ROM_MBC1(0);
}

void death_check(MPlayer* player){
    //SWITCH_ROM_MBC1(1);
    UINT16 pos = get_world_to_map((player->sprite.position.x), player->sprite.position.y);
    SWITCH_ROM_MBC5(current_world);
    if(COL_DEATH_CHECK(0)){
        SWITCH_ROM_MBC5(0);
        update_position(&(player->sprite));

        //current_level = 0;
        JOYPAD_WAIT_ANY;
        set_map(player);
    }
    SWITCH_ROM_MBC5(0);
}

void teleporter_check(MPlayer* player){
    //SWITCH_ROM_MBC1(0);
    if(teleporter_active == true){
        teleporter_active = false;
        UINT16 pos = get_world_to_map((player->sprite.position.x), player->sprite.position.y);
        
        SWITCH_ROM_MBC5(current_world);
        if(GLOBAL_MAP[pos] == TELEPORTER_TILE  ||
           GLOBAL_MAP[pos] == TELEPORTER2_TILE ||
           GLOBAL_MAP[pos] == TELEPORTER3_TILE){
            DEBUG_LOG_MESSAGE("Teleporter active");
            for(int i = 0; i < TOTAL_TILES; ++i){
                if(i != pos && GLOBAL_MAP[i] == GLOBAL_MAP[pos]){
                    SWITCH_ROM_MBC5(0);
                    player->sprite.position.x = (i % 20) * 8 + 8;
                    player->sprite.position.y = (i / 20) * 8 + 16;
                    delay(100);
                    update_position(&(player->sprite));
                }
            }
        }
    }
    //SWITCH_ROM_MBC1(1);
}
