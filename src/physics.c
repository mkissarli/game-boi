#define GRAVITY ITOF(54)
UINT8 gravity_time = 0;
void gravity(MSprite* sprite){
    //Collision Check
    if(sprite->col.direction.y != 1){
        sprite->speed.y =  FADD(sprite->speed.y, FDIV(GRAVITY, ITOF(FPS)));
        DEBUG_LOG_MESSAGE("ACC: %d.%d", FINT(FDIV(GRAVITY, ITOF(FPS))), FFRAC(FDIV(GRAVITY, ITOF(FPS)), 10))
        DEBUG_LOG_MESSAGE("sprite y speed: %d.%d", FINT(sprite->speed.y), FFRAC(sprite->speed.y, 10));
    }
}

#define COL_DEATH_CHECK(x) \
    (GLOBAL_MAP[pos + x] == SPIKE_U ||         \
     GLOBAL_MAP[pos + x] == SPIKE_D ||         \
     GLOBAL_MAP[pos + x] == SPIKE_L ||         \
     GLOBAL_MAP[pos + x] == SPIKE_R)           \

#define COL_CHECK(x)\
    (GLOBAL_MAP[pos + x] != EMPTY_TILE &&      \
     GLOBAL_MAP[pos + x] != FLAG_TILE  &&      \
     GLOBAL_MAP[pos + x] != START_TILE &&      \
     !COL_DEATH_CHECK(x))


void collision_check(MSprite* sprite){
    UINT16 pos = get_world_to_map(&(sprite->position));
    
    sprite->col.has_collided = false;
    sprite->col.direction.x = 0;
    sprite->col.direction.y = 0;
    // Left
    if(pos - 1 % 20 == 0 ||
       COL_CHECK(-1)){
        sprite->col.direction.x = -1;
        sprite->col.has_collided = true;
    }
    // Right
    if(pos + 1 % 20 == 19 ||
       COL_CHECK(1)){
        sprite->col.direction.x = 1;
        sprite->col.has_collided = true;
    }
    // Up
    if(pos - 20 < 0 ||
       COL_CHECK(-20)){
        sprite->col.direction.y = -1;
        sprite->col.has_collided = true;
    }
    // Down
    if(pos + 20 > 20 * 18 ||
       COL_CHECK(20)){
        sprite->col.direction.y = 1;
        sprite->col.has_collided = true;
    }
}

void death_check(MPlayer* player){
    UINT16 pos = get_world_to_map(&(player->sprite.position));

    if(COL_DEATH_CHECK(0)){
        DEBUG_LOG_MESSAGE("YOU HAVE DIED.");
        update_position(&(player->sprite));

        current_level = 0;
        JOYPAD_WAIT_ANY;
        set_map(player);
    }
}
