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
