#ifndef ROUND_H
#define ROUND_H

#include "game_state.h"
#include "renderer.h"
#include "../graphics/graphics.h"
#include "../logic/bomb_logic.h"
#include "../logic/move_logic.h"
#include "../input/input.h"
#include "../logic/enemy_logic.h"
#include "../logic/item_logic.h"
#include <stdbool.h>

#define FRAMES_IN_A_ROUND 2800
#define FINAL_COUNTDOWN_RATE 5
#define Y_INC_INIT 1
#define X_INC_INIT 0

/* Sound functions, defined in sound.s */
extern void update_sound(void);
extern void snd_play_pickup(void);
extern void snd_stop_pickup(void);
extern void snd_play_timer_ticking(void);
extern void snd_play_explosion(void);
extern void snd_stop_explosion(void);
extern void snd_play_plant(void);
extern void snd_stop_plant(void);
extern void snd_play_death(void);
extern void snd_play_crowd_cheer(void);
extern void snd_stop_crowd_cheer(void);

player_id playRound(void);

void setupRound(void);
void setupPlayers(void);
void setupGrids(void);

int8_t countPlayers(void);
player_id getWinner(void);

/* Moves players using the controller signals array */
void applyPlayerInput(void);
void applyAIMovement(void);

void finalCountdown(int);

void debug1(void);
void debug2(void);
void debug3(void);

#endif
