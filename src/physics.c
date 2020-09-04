static UINT8 GRAVITY = 1;
UINT8 gravity_time = 0;
void gravity(MSprite* sprite){
    //Collision Check
    if(sprite->col.direction.y != 1){
        sprite->speed.y += GRAVITY;
    }
}

void collision_check(MSprite* sprite){
    UINT16 pos = get_world_to_map(&(sprite->position));
    
    sprite->col.has_collided = false;
    sprite->col.direction.x = 0;
    sprite->col.direction.y = 0;
    // Left
    if(pos - 1 % 20 == 0 ||
       (GLOBAL_MAP[pos - 1] != EMPTY_TILE &&
        GLOBAL_MAP[pos - 1] != FLAG_TILE  &&
        GLOBAL_MAP[pos - 1] != START_TILE)){
        sprite->col.direction.x = -1;
        sprite->col.has_collided = true;
    }
    // Right
    if(pos + 1 % 20 == 19 ||
       (GLOBAL_MAP[pos + 1] != EMPTY_TILE &&
        GLOBAL_MAP[pos + 1] != FLAG_TILE  &&
        GLOBAL_MAP[pos + 1] != START_TILE)){
        sprite->col.direction.x = 1;
        sprite->col.has_collided = true;
    }
    // Up
    if(pos - 20 < 0 ||
       (GLOBAL_MAP[pos - 20] != EMPTY_TILE &&
        GLOBAL_MAP[pos - 20] != FLAG_TILE  &&
        GLOBAL_MAP[pos - 20] != START_TILE)){
        sprite->col.direction.y = -1;
        sprite->col.has_collided = true;
    }
    // Down
    if(pos + 20 > 20 * 18 || 
       (GLOBAL_MAP[pos + 20] != EMPTY_TILE &&
        GLOBAL_MAP[pos + 20] != FLAG_TILE  &&
        GLOBAL_MAP[pos + 20] != START_TILE)){
        sprite->col.direction.y = 1;
        sprite->col.has_collided = true;
    }
}
