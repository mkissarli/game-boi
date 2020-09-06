//#include "./tuple_types.c"
//#include "./player.c"

#include "../sprites/you_win_data.c"
#include "../sprites/you_win_map.c"

#include "../sprites/background.c"
#include "../maps/map1.c"
#include "../maps/map2.c"
#include "../maps/map3.c"
#include "../maps/map4.c"
#include "../maps/map4b.c"
#include "../maps/map5.c"
#include "../maps/map6.c"
#include "../maps/map7.c"
#include "../maps/map8.c"

#define START_TILE    0x02
#define FLAG_TILE     0x01
#define EMPTY_TILE    0x00

#define SPIKE_U 0x05
#define SPIKE_D 0x07
#define SPIKE_L 0x06
#define SPIKE_R 0x08

#define TELEPORTER_TILE 0x09

#define MAP_WIDTH     20
#define MAP_HEIGHT    18
#define TOTAL_TILES   MAP_WIDTH * MAP_HEIGHT
#define TOTAL_MAP_NUM 8

UINT16 get_world_to_map(MVector* pos){
    return ((pos->x - 8 ) >> 3) + ((pos->y - 16) >> 3 )* 20;
}

static UINT8 current_level = 0;

static unsigned char maps[TOTAL_MAP_NUM][TOTAL_TILES] =
{
    {Map1},
    {Map2},
    {Map3},
    {Map5},
    {Map4b},
    {Map6},
    {Map7},
    {Map8}
};

#define GLOBAL_MAP maps[current_level]

void set_map (MPlayer* player){
    set_bkg_tiles(0,0,20,18,GLOBAL_MAP);
    // Start from bottom as more likely the start is near the bottom
    for(UINT16 i = 20 * 18 - 1; i >= 0; --i){
        if(GLOBAL_MAP[i] == START_TILE){
            player->sprite.position.x = i % 20 * 8 + 8;
            player->sprite.position.y = i / 20 * 8 + 16;
            break;
        }
    }

    update_position(&(player->sprite));
}

BYTE win_condition (MPlayer* player){
    UINT16 pos = get_world_to_map(&(player->sprite.position));
    if(GLOBAL_MAP[pos] == FLAG_TILE){
        if(current_level < TOTAL_MAP_NUM - 1){
            DEBUG_LOG_MESSAGE("You did a win!!");
            // Some form of congrats
            delay(100);
            ++current_level;
            set_map(player);
            return 1;
        }
        else {
            DEBUG_LOG_MESSAGE("You won the game!");
            move_sprite(player->sprite.sprite_number, 180, 0 );
            set_bkg_data(0, 114, you_win_data);
            set_bkg_tiles(0, 0, 20, 18, you_win_map);

            current_level = 0;
            JOYPAD_WAIT_ANY;
            set_map(player);
        }
    }
    return 0;
}
