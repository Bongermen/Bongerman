#include "round.h"

player_id playRound(void) {
    setupRound(); 

    int frames = 0;
    int players_alive = PLAYER_NUM;
    int final_countdown_count = 0;
    while(players_alive > 1) {

        updateControls();

        if (frames >= FRAMES_IN_A_ROUND) {
            if ((frames % FINAL_COUNTDOWN_RATE) == 0) {
                finalCountdown(final_countdown_count);
                final_countdown_count++;
            }
        } else {
            updateTimebar((double) ((double) frames / (double) FRAMES_IN_A_ROUND));
        }

        applyPlayerInput();
        applyAIMovement();
        if (applyItems()) {
            snd_play_pickup();
        }

        countdownExplosions();
        if (plantBombs()) {
            snd_play_plant();
        }
        if (countdownBombs()) {
            snd_play_explosion();
        }

        render();
        players_alive = countPlayers();
        frames++;
        update_sound();
    }

    return getWinner();
}

player_id getWinner(void) {
    for (int i = 0; i < PLAYER_NUM; i++) {
        if (players[i].alive) {
            return players[i].id;
        }
    }
    return PLAYER_NONE;
}

int8_t countPlayers(void) {

    int players_alive = 0;
    for (int i = 0; i < PLAYER_NUM; i++) {
        if (players[i].alive) {
            players_alive++;
        }
    }
    return players_alive;
}

void debug2(void) {

    direction next[4];
    for (int i = 0; i < PLAYER_NUM; i++) {
        if (players[i].alive) {
            next[i] = get_ai_dir(&(players[i]));
        }
    }

    for (int i = 0; i < PLAYER_NUM; i++) {
        move(&(players[i]), next[i]);
    }

    countdownExplosions();
    plantBombs();
    countdownBombs();
    render();
}

void applyPlayerInput(void) {
    if (controller_signals[CONTROLLER_ONE_IND_TRIGGER]) {
        players[PLAYER_ONE].plant_bomb = true;
    }
  
    if (controller_signals[CONTROLLER_ONE_IND_RIGHT]) {
        move(&players[PLAYER_ONE], DIRECTION_RIGHT);
    } else if (controller_signals[CONTROLLER_ONE_IND_UP]) {
        move(&players[PLAYER_ONE], DIRECTION_UP);
    } else if (controller_signals[CONTROLLER_ONE_IND_LEFT]) {
        move(&players[PLAYER_ONE], DIRECTION_LEFT);
    } else if (controller_signals[CONTROLLER_ONE_IND_DOWN]) {
        move(&players[PLAYER_ONE], DIRECTION_DOWN);
    } else {
        move(&players[PLAYER_ONE], DIRECTION_IDLE);
    }
  
    if (controller_signals[CONTROLLER_TWO_IND_TRIGGER]) {
        players[PLAYER_TWO].plant_bomb = true;
    }
  
    if (controller_signals[CONTROLLER_TWO_IND_RIGHT]) {
        move(&players[PLAYER_TWO], DIRECTION_RIGHT);
    } else if (controller_signals[CONTROLLER_TWO_IND_UP]) {
        move(&players[PLAYER_TWO], DIRECTION_UP);
    } else if (controller_signals[CONTROLLER_TWO_IND_LEFT]) {
        move(&players[PLAYER_TWO], DIRECTION_LEFT);
    } else if (controller_signals[CONTROLLER_TWO_IND_DOWN]) {
        move(&players[PLAYER_TWO], DIRECTION_DOWN);
    } else {
        move(&players[PLAYER_TWO], DIRECTION_IDLE);
    }
}

/* DEBUG - players all walk in the same direction which changes periodically  */
void debug1(void) {

    int count = 0;
    if (count % 100 == 0) {
    for (int i = 0; i < PLAYER_NUM; i++) {
        if (players[i].alive) {
            uint16_t random = getRandomLimit(100);
            switch (random) {
                case 0:
                case 1:
                case 2:
                    move(&(players[i]), DIRECTION_RIGHT);
                    break;
                case 3:
                case 4:
                case 5:
                    move(&(players[i]), DIRECTION_DOWN);
                    break;
                case 6:
                case 7:
                case 8:
                    move(&(players[i]), DIRECTION_LEFT);
                    break;
                case 9:
                case 10:
                case 11:
                    move(&(players[i]), DIRECTION_UP);
                    break;
                case 12:
                    players[i].plant_bomb = true;
                    break;
                default:
                    move(&(players[i]), DIRECTION_IDLE);
                    break;
                }
            }
        }
        count = 0;
    }

    applyItems();

    countdownExplosions();
    plantBombs();
    countdownBombs();
    count++;


    render();
}

void applyAIMovement(void) {
    direction next[2] = {0, 0};
    for (int i = 2; i < PLAYER_NUM; i++) {
        if (players[i].alive) {
            next[i - 2] = get_ai_dir(&(players[i]));
        }
    }

    for (int i = 2; i < PLAYER_NUM; i++) {
        move(&(players[i]), next[i - 2]);
    }
}

/* DEBUG - input testing */
void debug3(void) {
   
    updateControls();
    applyPlayerInput();
    applyAIMovement();
    applyItems();

    countdownExplosions();
    plantBombs();
    countdownBombs();
    render();
}

void setupRound(void) {

    setupGrids();
    setupPlayers();
    drawInitialState();
    drawPlayers();
    drawSidebar();
    drawPlayerIcons();
    drawPlayerScores();
    drawFullTimebar();
    renderFrame();
}

/* Sets up players with default values */
void setupPlayers(void) {
    for (int i = 0; i < PLAYER_NUM; i++) {
        players[i].alive = true;

        players[i].bomb.owner = players[i].id;
        players[i].bomb.timer = DEFAULT_BOMB_TIMER;
        players[i].bomb.range = DEFAULT_BOMB_RANGE;
        players[i].bomb.type  = DEFAULT_BOMB_TYPE;
        players[i].bomb.current_frame  = 0;
        players[i].bomb.explosion.type = EXPLOSION_TYPE_NORMAL;
        players[i].bomb.explosion.timer = 40;

        players[i].moving          = false;
        players[i].move_speed      = DEFAULT_MOVE_SPEED;
        players[i].move_direction  = DEFAULT_DIRECTION;

        players[i].max_bomb_number     = DEFAULT_MAX_BOMBS;
        players[i].current_bomb_number = 0;

        players[i].current_frame = A_MOVE_DOWN_FRAMES * i;
    }

    /* Set up starting positions */
    int id = 0;
    for (int i = 0; i < MAP_SIZE; i+= MAP_SIZE - 1) {
        for (int j = 0; j < MAP_SIZE; j+= MAP_SIZE - 1) {
            players[id].tile_position.x = j;
            players[id].tile_position.y = i;

            players[id].screen_position.x = j * TILE_SIZE;
            players[id].screen_position.y = i * TILE_SIZE;

            id++;
        } 
    }
}

/* Initialises arrays containing game state info */
void setupGrids(void) {

    /* Set up default values */
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            terrain_grid[i][j] = TERRAIN_WALL_BREAKABLE;

            bomb_grid[i][j].type = BOMB_EMPTY;
            bomb_grid[i][j].timer = 0;
            bomb_grid[i][j].range = 0;

            explosion_grid[i][j].type = EXPLOSION_EMPTY;
            explosion_grid[i][j].timer = 0;

            changed_tiles[i][j] = false;

            items_grid[i][j] = ITEM_EMPTY;
        }
    }

    /* Clear random walls */
    uint16_t number_of_tiles = 20 + getRandomLimit(60);
    for (int i = 0; i < number_of_tiles; i++) {
       uint16_t x = getRandomLimit(MAP_SIZE - 1);
       uint16_t y = getRandomLimit(MAP_SIZE - 1);
       terrain_grid[x][y] = TERRAIN_GROUND;
    }

    /* Set up unbreakable walls */
    for (int i = 1; i < MAP_SIZE; i += 2) {
        for (int j = 1; j < MAP_SIZE; j+=2) {
            terrain_grid[i][j] = TERRAIN_WALL_UNBREAKABLE;
        }
    }

    /* Clear out starting areas (corners) */
    for (int i = 0; i < MAP_SIZE; i += MAP_SIZE - 1) {
        for (int j = 0; j < MAP_SIZE; j += MAP_SIZE - 1) {
            terrain_grid[i][j] = TERRAIN_GROUND;
            int tempX = (i == 0) ? 1 : MAP_SIZE - 2;
            int tempY = (j == 0) ? 1 : MAP_SIZE - 2;
            terrain_grid[i][tempY] = TERRAIN_GROUND;
            terrain_grid[tempX][j] = TERRAIN_GROUND;
        }
    }

}

void finalCountdown(int count) {
    int x = 0;
    int y = 0;
    int y_inc = Y_INC_INIT;
    int x_inc = X_INC_INIT;
    int x_min = 0;
    int y_min = 0;
    int x_max = MAP_SIZE-1;
    int y_max = MAP_SIZE-1;

    //finalCountDown logic
    
    for(int i = 0; i<=count; i++) {
        if(count == i) {
            explosion_grid[x][y].type = EXPLOSION_TYPE_PERMANENT;
            changed_tiles[x][y] = true;
            return;
        }
        if(y_inc != 0) {
            y += y_inc;
            if(y > y_max) {
                y--;
                y_inc = 0;
                x_inc = 1;
                x_min ++;
            } else if(y < y_min) {
                y++;
                y_inc = 0;
                x_inc = -1;
                x_max --;
            } else {

            }
        } else if(x_inc != 0) {
            x += x_inc;
            if(x > x_max) {
                x--;
                x_inc = 0;
                y_inc = -1;
                y_max --;
            } else if(x < x_min) {
                x++;
                x_inc = 0;
                y_inc = 1;
                y_min ++;
            } else {

            }
        }
    }
}
