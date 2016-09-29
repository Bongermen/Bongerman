#ifndef MOVE_LOGIC_H
#define MOVE_LOGIC_H

#include "../objects/game_state.h"
#include "../objects/direction.h"

void move(Player *player, direction dir);
bool checkWalkable(int32_t x, int32_t y);

void getOccupiedTiles(Player* player, Position* tile1, Position* tile2);
void screenToTile(Position *pos);
bool isAligned(int32_t x, int32_t y);
void setPlayerTilePos(Player* player);
void setPlayerPosition(Player* player, int32_t x, int32_t y);
bool snapToGrid(Player* player, direction dir);
void clamp(int32_t* i);
bool walkingOutOfBounds(Player* player, direction dir);

void getPixelOffset(Position* result, direction dir);
void getNextStep(Position* pos, direction dir);
void getOffsets(Position* next_pos, Position* pixel_offset, Player* player, direction dir);

#endif
