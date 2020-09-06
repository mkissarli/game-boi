//#include "./tuple_types.h"
//#include "./player.c"

#include "../sprites/you_win_data.c"
#include "../sprites/you_win_map.c"

//#include "../sprites/background.c"
//#include "../maps/map1.c"
//#include "../maps/map2.c"
//#include "../maps/map3.c"
//#include "../maps/map4.c"
//#include "../maps/map4b.c"
//#include "../maps/map5.c"
//#include "../maps/map6.c"
//#include "../maps/map7.c"
//#include "../maps/map8.c"
//#include "../maps/map9.c"
//#include "../maps/map10.c"

UINT8 current_level = 0;
UINT8 current_world = 1;

UINT16 get_world_to_map(UINT16 x, UINT16 y){
    return ((x - 8 ) >> 3) + ((y - 16) >> 3 )* 20;
}

//extern BYTE win_condition(MPlayer* player);
unsigned char use_maps[TOTAL_MAP_NUM][TOTAL_TILES];
extern unsigned char maps[TOTAL_MAP_NUM][TOTAL_TILES];
extern unsigned char maps2[TOTAL_MAP_NUM][TOTAL_TILES];
//extern void death_check(MPlayer* player);
//extern void set_map(MPlayer* player);

/*
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
    {Map8},
    {Map9},
    {Map10},
};

*/

void reset_map(UINT8 cur){
    if(cur == 1){
        for(int j = 0; j < TOTAL_MAP_NUM; ++j){
            for(int i = 0; i < TOTAL_TILES; ++i){
                use_maps[j][i] = maps[j][i];
            }
        }
    }
    else{
        for(int j = 0; j < TOTAL_MAP_NUM; ++j){
            for(int i = 0; i < TOTAL_TILES; ++i){
                use_maps[j][i] = maps2[j][i];
            }
        }
    }
}

void set_map (MPlayer* player){
    SWITCH_ROM_MBC5(current_world); 
    DEBUG_LOG_MESSAGE("world %d \n",current_world);
    set_bkg_tiles(0,0,20,18, GLOBAL_MAP);
    
    SWITCH_ROM_MBC5(0);
    // Start from bottom as more likely the start is near the bottom
    for(UINT16 i = 20 * 18 - 1; i >= 0; --i){
        SWITCH_ROM_MBC5(current_world);
        if(GLOBAL_MAP[i] == START_TILE){
            SWITCH_ROM_MBC5(0);
            player->sprite.position.x = i % 20 * 8 + 8;
            player->sprite.position.y = i / 20 * 8 + 16;
            break;
        }
    }

    update_position(&(player->sprite));
}

BYTE win_condition (MPlayer* player){
    UINT16 pos = get_world_to_map((player->sprite.position.x), player->sprite.position.y);
    SWITCH_ROM_MBC5(current_world);
    if(GLOBAL_MAP[pos] == FLAG_TILE){
        SWITCH_ROM_MBC5(0);
        if(current_level < TOTAL_MAP_NUM - 1){
            //DEBUG_LOG_MESSAGE("You did a win!!");
            // Some form of congrats
            delay(100);
            ++current_level;
            set_map(player);
            return 1;
        }
        else {
            current_level = 0;
            ++current_world;

            reset_map(current_world);
            
            //DEBUG_LOG_MESSAGE("You won the game!");
            if(current_world > TOTAL_WORLD_NUM){
                move_sprite(player->sprite.sprite_number, 180, 0 );
                set_bkg_data(0, 114, you_win_data);
                set_bkg_tiles(0, 0, 20, 18, you_win_map);
                
                current_level = 0;
                current_world = 1;
                reset_map(1);
                JOYPAD_WAIT_ANY;
                delay(100);
                
                set_bkg_data(0, 12, BackgroundTiles);
                set_map(player);
                
            }
        }
    }
    return 0;
}

