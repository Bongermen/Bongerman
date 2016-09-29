#ifndef BOMB_H
#define BOMB_H

#include <stdint.h>
#include <stdbool.h>

#include "position.h"
#include "explosion.h"

#define DEFAULT_BOMB_TIMER 100
#define DEFAULT_BOMB_RANGE 1
#define DEFAULT_BOMB_TYPE BOMB_TYPE_NORMAL

typedef enum {
    BOMB_EMPTY,
    BOMB_TYPE_NORMAL
} bomb_type;

typedef struct {
    int owner;       // todo make a player_id
    int32_t timer;   // 0 when out of time
    int8_t range;    // number of grid units for the explosion
    bomb_type type;
    Position position;
    Explosion explosion;

    bool current_frame;
} Bomb;

#endif
