#include <gb/gb.h>

#include "./globals.h"

//#include "./tuple_types.h"
//#include "./player.h"

//#include "../sprites/you_win_data.c"
//#include "../sprites/you_win_map.c"

//#include "../sprites/background.c"
#include "../maps/map1.c"
#include "../maps/map2.c"
#include "../maps/map3.c"
#include "../maps/map4.c"
//#include "../maps/map4b.c"
#include "../maps/map5.c"
//#include "../maps/map6.c"
//#include "../maps/map7.c"
//#include "../maps/map8.c"
//#include "../maps/map9.c"
//#include "../maps/map10.c"

// These are our extern(ally) defined functions we use in bank1
//extern typedef struct MPlayer;
//extern typedef MVector;
//extern UINT16 get_world_to_map(UINT16, UINT16);
//extern UINT8 current_level;
//extern void update_player();
//extern void update_position_split(UINT16, UINT16);

unsigned char maps[TOTAL_MAP_NUM][TOTAL_TILES] =
{
    {Map1},
    {Map2},
    {Map3},
    {Map5},
    //{Map4b},
    //{Map6},
    //{Map7},
    //{Map8},
    //{Map9},
    //{Map10},
};



