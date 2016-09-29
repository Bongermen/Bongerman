#ifndef ENEMY_LOGIC_H
#define ENEMY_LOGIC_H

#include <stdbool.h>

#include "move_logic.h"
#include "../objects/game_state.h"
#include "../objects/player.h"
#include "../objects/bomb.h"
#include "../objects/position.h"
#include "../objects/terrain.h"
#include "../util/util.h"

/* Testing 
#include <stdio.h>
#include "../objects/renderer.h"
#include "../graphics/graphics.h"

#define MAP_SIZE 13
#define TILE_SIZE 32
#define PLAYER_NUM 4
#define CONTROLLER_SIGNALS_NUM 10
 Testing End */
typedef enum {
    IDLE,
    AVOID_BOMB,
    GET_POWERUP,
    APPROACH_PLAYER
} ai_plan;

int getDistanceSquared(int x1, int y1, int x2, int y2);
bool breakableAdjacent(Position pos);
bool perpendicular(Position pos1, Position pos2);
bool inBombRange(Position pos, Position bomb_pos, int range);
bool inExplosionRange(Position pos);
bool isMovable(Position pos);
Position getClosestPowerup(Player *player);
Position getClosestPlayer(Player *player);
Position getClosestBombInRange(Player *player);
bool checkUp(Position pos);
bool checkDown(Position pos);
bool checkLeft(Position pos);
bool checkRight(Position pos);
direction getApproachMove(Player *player, Position pos);
direction avoidBombMove(Player *player);
void dropBombIfBreakable(Player *player);
ai_plan getPlan(Player *player);
direction get_ai_dir(Player *player);

#endif
