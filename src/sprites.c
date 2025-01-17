#include "./sprites.h"

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
    //DEBUG_LOG_MESSAGE("%d :: %d", sprite->position.x, sprite->position.y);
    move_sprite(sprite->sprite_number, sprite->position.x, sprite->position.y);
}


void update_position(MSprite* sprite){
    // Not colliding left and moving left and not colliding right and moving right
    if(!(sprite->col.direction.x == -1 && sprite->speed.x < 0) &&
       !(sprite->col.direction.x ==  1 && sprite->speed.x > 0)){
        //DEBUG_LOG_MESSAGE("%d -> %d -> %d",)
        sprite->position.x = FROUNDU(FADD(UTOF(sprite->position.x), sprite->speed.x));
    }
    else{
        sprite->speed.x = 0;
    }
    // Not colliding up and going up and not colliding down and moving down.
    if(!(sprite->col.direction.y == -1 && sprite->speed.y < 0) &&
       !(sprite->col.direction.y ==  1 && sprite->speed.y > 0)){
        sprite->position.y = FROUNDU(FADD(UTOF(sprite->position.y), sprite->speed.y));
    }
    else {
        sprite->speed.y = 0;
    }
}

void update_position_split(UINT16 x, UINT16 y){
}
