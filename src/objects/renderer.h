#ifndef RENDERER_H
#define RENDERER_H

#include "game_state.h"
#include "player.h"
#include "bomb.h"
#include "explosion.h"
#include "../util/util.h"
#include "../graphics/graphics.h"
#include "../kernel/assets.h"

#define OFFSET_X 32
#define OFFSET_Y 32

#define BOMB_EXPLODING_THRESHOLD 50
#define COLOR_SIDEBAR_BG fromRGB(247, 140, 0)
#define COLOR_SIDEBAR_FONT COLOR_WHITE
#define COLOR_SIDEBAR_FONT_BG COLOR_BLACK
#define COLOR_BORDER COLOR_RED
#define COLOR_TIMER fromRGB(8, 158, 0)
#define COLOR_TIMER_BG fromRGB(0, 0, 0)

#define SIDEBAR_OFFSET ((MAP_SIZE + 2) * TILE_SIZE + 1)
#define SIDEBAR_TEXT_OFFSET (SIDEBAR_OFFSET + 30 + TILE_SIZE)
#define SIDEBAR_PLAYERS_OFF_Y 120
#define SIDEBAR_PLAYERS_GAP_Y 60
#define SIDEBAR_TIMER_WIDTH 97
#define SIDEBAR_TIMER_HEIGHT 15

#define RESULTS_OFFSET 160
#define RESULTS_TEXT_X (RESULTS_OFFSET + 8)
#define RESULTS_TEXT_Y (RESULTS_OFFSET + 90)
#define COLOR_RESULTS_BG COLOR_SIDEBAR_BG

#define RESULTS_READY_X (RESULTS_OFFSET - 30)
#define RESULTS_READY_TEXT_X (RESULTS_OFFSET - 45)

/* Renders the game using the global game state grids */
void render(void);

/* Draws initial state of the map, call only once at the start of the game for
 * performance reasons */
void drawInitialState(void);

/* Draws player status on the side of the screen */
void drawSidebar(void);
void drawPlayerIcons(void);
void drawPlayerScores(void);


/* Timebar drawing utilities */
void drawFullTimebar(void);
void updateTimebar(double percentage);
void drawTimebar(int, uint32_t); // -- INTERNAL USE ONLY

/* Draws result screen with the given winner */
void drawResultScreen(player_id winner);

void drawReadyScreen(void);
void drawPlayerReady(player_id id);

/* Helper function for render - INTERNAL USE ONLY */
void drawPlayers(void);

/* Helper function for render - INTERNAL USE ONLY */
void redrawTile(uint32_t x, uint32_t y);

#endif
