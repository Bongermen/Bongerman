#ifndef GAME_H
#define GAME_H

#include "round.h"
#include "renderer.h"
#include "../graphics/graphics.h"
#include "../util/util.h"
#include "../input/input.h"

/* Sound functions, defined in sound.s */
extern void update_sound(void);
extern void snd_play_player_ready(void);
extern void snd_play_game_name(void);
extern void snd_play_end_game(void);

void runGame(void);
void initialisePlayers(void);
bool readyScreen(void);

#endif
