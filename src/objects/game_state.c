#include "game_state.h"

Terrain terrain_grid[MAP_SIZE][MAP_SIZE];
Bomb bomb_grid[MAP_SIZE][MAP_SIZE];
item_type items_grid[MAP_SIZE][MAP_SIZE];
Explosion explosion_grid[MAP_SIZE][MAP_SIZE];
bool changed_tiles[MAP_SIZE][MAP_SIZE];
Player players[PLAYER_NUM];
bool changed_tiles[MAP_SIZE][MAP_SIZE];
bool controller_signals[CONTROLLER_SIGNALS_NUM];
