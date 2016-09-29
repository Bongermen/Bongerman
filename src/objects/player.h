#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

#include "position.h"
#include "bomb.h"
#include "direction.h"

#define DEFAULT_MOVE_SPEED 2
#define DEFAULT_DIRECTION DIRECTION_DOWN
#define DEFAULT_MAX_BOMBS 1

typedef enum {
    PLAYER_ONE   = 0,
    PLAYER_TWO   = 1,
    PLAYER_THREE = 2,
    PLAYER_FOUR  = 3,
    PLAYER_NONE
} player_id;

typedef struct {
    player_id id;
    bool alive;
    Position tile_position;
    Position screen_position;
    Bomb bomb;

    bool moving;
    int8_t move_speed;
    direction move_direction;

    bool plant_bomb;
    int8_t max_bomb_number;
    int8_t current_bomb_number;

    uint8_t current_frame;

    uint8_t number_of_wins;
} Player;

#endif
