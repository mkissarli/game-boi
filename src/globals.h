#define FPS 60

#define START_TILE    0x02
#define FLAG_TILE     0x01
#define EMPTY_TILE    0x00

#define SPIKE_U 0x05
#define SPIKE_D 0x07
#define SPIKE_L 0x06
#define SPIKE_R 0x08

#define TELEPORTER_TILE 0x09
#define TELEPORTER2_TILE 0x0A
#define TELEPORTER3_TILE 0x0B

#define MAP_WIDTH     20
#define MAP_HEIGHT    18
#define TOTAL_TILES   MAP_WIDTH * MAP_HEIGHT
#define TOTAL_MAP_NUM 4
#define TOTAL_WORLD_NUM 2


#define GLOBAL_MAP use_maps[current_level]
//maps[current_level]
