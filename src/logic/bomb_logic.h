#ifndef BOMB_LOGIC_H
#define BOMB_LOGIC_H

#include <stdint.h>
#include <stdbool.h>

#include "../objects/bomb.h"
#include "../objects/explosion.h"
#include "../objects/terrain.h"
#include "../objects/player.h"
#include "../util/util.h"
#include "../objects/game_state.h"
#include "move_logic.h"
#include "item_logic.h"

/* The number of cycles before a bomb is redrawn */
#define BOMB_ANIMATION_CYCLES 5

/* 
 * Provides functions for managing bomb, destruction and explosion logic.
 * Call functions in the given order:
 *     1. countdownExplosions(...)
 *     2. plantBombs(...)
 *     3. countdownBombs(...) 
 */

/* Decrements timers on existing explosions and clears them when the timer reaches 0.
 * Kills any players standing inside a remaining explosion.
 * Updates changed tiles grid for tiles where an explosion has been cleared. */
void countdownExplosions(void);

/* Attempts to plants bombs for all players with plant_bomb flag set.
 * Updates changed tiles grid if a bomb needs to be rendered at that tile. 
 * Returns true iff at least one bomb has been planted. */
bool plantBombs(void);

/* Reduces timers on bombs and explodes them when the timer reaches 0.
 * Updates changed tiles grid for bomb animation and anything affected by
 * an explosion. 
 * Returns true iff at least one bomb explodes. */
bool countdownBombs(void);

/* Helper function for plantBombs -- INTERNAL USE ONLY
 * Plants a bomb in the Player tile position when possible.
 * Updates changed tiles grid if a bomb needs to be rendered at that tile. */
void plantBomb(Player *player);

/* Helper function for countdownBombs -- INTERNAL USE ONLY
 * Explodes the given bomb (if it's non-empty), updating the explosion 
 * grid, triggering  other bombs, killing players, breaking breakable terrain 
 * and destroying items.
 * Updates changed tiles grid for all required rendering. */
void explodeBomb(Bomb *bomb);

//TODO: consider overlap behaviour for explosions with different durations
/* Helper function for explodeBomb. -- INTERNAL USE ONLY
 * Applies destruction logic to the cell at the given position.
 * Sets the tile in changed tiles if needed.
 * Ignores out of range positions. */
void explodeTile(int8_t x, int8_t y, Explosion *explosion);

/* Helper function for explodeBomb and countdownExplosions. -- INTERNAL USE ONLY
 * Kills players if they are inside an explosion.
 * Sets the tile in changed tiles if needed. */
void killPlayersInExplosion(void);

#endif
