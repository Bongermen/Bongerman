#ifndef ITEM_LOGIC_H
#define ITEM_LOGIC_H
#define MIN_MOVE_SPEED 1
#define MAX_MOVE_SPEED 31
#define MAP_SIZE 13
#define MIN_BOMB_RANGE 1
#define MIN_BOMB_NUMBER 1
#define RANDOM_VALUE_LIMIT_N 10

#define PERC_EMPTY_ITEMS_N 50
#define PERC_POSITIVE_ITEMS_N 70



#include <stdbool.h>
#include "../objects/item.h"
#include "../objects/player.h"
#include "../objects/game_state.h"
#include "../util/util.h"

/* Returns true iff at least one item has been applied */
bool applyItems();

void applyItem(Player *player, item_type item);
void changeSpeed(Player *player, int8_t changeVal);
void changeRange(Player *player, int8_t changeVal);
void changeBombNum(Player *player, int8_t changeVal);
item_type generateRandomItem();
#endif
