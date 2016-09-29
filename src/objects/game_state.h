#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdbool.h>

#include "terrain.h"
#include "explosion.h"
#include "bomb.h"
#include "player.h"
#include "item.h"

#define MAP_SIZE 13
#define TILE_SIZE 32
#define PLAYER_NUM 4
#define CONTROLLER_SIGNALS_NUM 10

extern Terrain terrain_grid[MAP_SIZE][MAP_SIZE];
extern Bomb bomb_grid[MAP_SIZE][MAP_SIZE];
extern item_type items_grid[MAP_SIZE][MAP_SIZE];
extern Explosion explosion_grid[MAP_SIZE][MAP_SIZE];
extern bool changed_tiles[MAP_SIZE][MAP_SIZE];
extern Player players[PLAYER_NUM];
extern bool controller_signals[CONTROLLER_SIGNALS_NUM];

#endif
