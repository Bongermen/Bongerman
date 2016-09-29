#include "item_logic.h"

bool applyItems() {
        bool item_applied = false; 
	for(int32_t i = 0; i < PLAYER_NUM; i++) {
		int32_t pos_x = players[i].tile_position.x;
		int32_t pos_y = players[i].tile_position.y;
		item_type item = items_grid[pos_x][pos_y];
		
		if(item!=ITEM_EMPTY) {
			applyItem(&(players[i]), item); 
			items_grid[pos_x][pos_y] = ITEM_EMPTY;
			changed_tiles[pos_x][pos_y] = true;
                        item_applied = true;
		}
	}

        return item_applied;
}

//TODO: Edge cases for speed up
void applyItem(Player *player, item_type item) {
	switch(item) {
		case ITEM_SPEED_UP:
			changeSpeed(player, 1);
			break;

		case ITEM_SPEED_DOWN:
			changeSpeed(player, -1);
			break;

		case ITEM_RANGE_UP:
			changeRange(player, 1);
			break;

		case ITEM_RANGE_DOWN:
			changeRange(player, -1);
			break;

		case ITEM_BOMBS_UP:
			changeBombNum(player, 1);
			break;

		case ITEM_BOMBS_DOWN:
			changeBombNum(player, -1);
			break;

		default:
			break;
	}
}

void changeSpeed(Player *player, int8_t changeVal) {
	int8_t changedVal = (player->move_speed + changeVal);
	if(changedVal >= MIN_MOVE_SPEED && changedVal <= MAX_MOVE_SPEED) {
		player->move_speed = changedVal;
	}
}

void changeRange(Player *player, int8_t changeVal) {
	int8_t changedVal = (player->bomb.range + changeVal);
	if(changedVal >= MIN_BOMB_RANGE && changedVal <= (MAP_SIZE-1)) {
		player->bomb.range = changedVal;
	}
}

void changeBombNum(Player *player, int8_t changeVal) {
	int8_t changedVal = (player->max_bomb_number + changeVal);
	if(changedVal >= MIN_BOMB_NUMBER) {
		player->max_bomb_number = changedVal;
	}
}

item_type generateRandomItem() {
	uint16_t random_n = getRandomLimit(RANDOM_VALUE_LIMIT_N);
	if(random_n > PERC_EMPTY_ITEMS_N*RANDOM_VALUE_LIMIT_N/100) {
		random_n = getRandomLimit(RANDOM_VALUE_LIMIT_N);
		uint16_t random2 = getRandomLimit(2);
		if(random_n <= PERC_POSITIVE_ITEMS_N*RANDOM_VALUE_LIMIT_N/100) {
			return (random2*2);
		} else {
			return (random2*2+1);
		}
	} else {
		return ITEM_EMPTY;
	}
}
