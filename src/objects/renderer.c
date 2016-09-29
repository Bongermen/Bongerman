#include "renderer.h"

void render(void) {

    /* Render the tiles that have changed */
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (changed_tiles[i][j]) {
                redrawTile(i, j);
                changed_tiles[i][j] = false;
            }
        }
    }

    drawPlayers();

    /* Call renderFrame at the end of each frame to draw results on screen */
    renderFrame();
}

void drawInitialState(void) {

    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            redrawTile(i, j);
        }

        /* Draw border */
        drawAnimFrame(0, i * TILE_SIZE + OFFSET_Y, a_terrain, 2);
        drawAnimFrame(MAP_SIZE * TILE_SIZE + OFFSET_X, i * TILE_SIZE + OFFSET_Y, a_terrain, 2);
        drawAnimFrame(i * TILE_SIZE + OFFSET_X, 0, a_terrain, 2);
        drawAnimFrame(i * TILE_SIZE + OFFSET_X, MAP_SIZE * TILE_SIZE + OFFSET_Y, a_terrain, 2);
    }

    /* Draw corners */
    drawAnimFrame(0, 0, a_terrain, 2);
    drawAnimFrame(MAP_SIZE * TILE_SIZE + OFFSET_X, 0, a_terrain, 2);
    drawAnimFrame(0, MAP_SIZE * TILE_SIZE + OFFSET_Y, a_terrain, 2);
    drawAnimFrame(MAP_SIZE * TILE_SIZE + OFFSET_X, MAP_SIZE * TILE_SIZE + OFFSET_Y, a_terrain, 2);
}

void drawSidebar(void) {
    /* Draw border */
    drawRect(SIDEBAR_OFFSET, 0, PHYSICAL_WIDTH - SIDEBAR_OFFSET - 1, PHYSICAL_HEIGHT - 1, COLOR_BORDER, false);
    drawRect(SIDEBAR_OFFSET + 1, 1, PHYSICAL_WIDTH - SIDEBAR_OFFSET - 3, PHYSICAL_HEIGHT - 3, COLOR_BLACK, false);
    drawRect(SIDEBAR_OFFSET + 2, 2, PHYSICAL_WIDTH - SIDEBAR_OFFSET - 5, PHYSICAL_HEIGHT - 5, COLOR_BORDER, false);

    /* Draw background */
    drawRect(SIDEBAR_OFFSET + 3, 3, PHYSICAL_WIDTH - SIDEBAR_OFFSET - 7, PHYSICAL_HEIGHT - 7, COLOR_SIDEBAR_BG, true);

    /* Draw text bg */
    for (int i = 0; i < PLAYER_NUM; i++) {
        drawRect(SIDEBAR_TEXT_OFFSET - 5, SIDEBAR_PLAYERS_OFF_Y + i * SIDEBAR_PLAYERS_GAP_Y - 2, 82, CHAR_HEIGHT * 2 + 3, COLOR_BORDER, false);
        drawRect(SIDEBAR_TEXT_OFFSET - 4, SIDEBAR_PLAYERS_OFF_Y + i * SIDEBAR_PLAYERS_GAP_Y - 1, 80, CHAR_HEIGHT * 2 + 1, COLOR_SIDEBAR_FONT_BG, true);

        /* Draw border around player icons */
        drawRect(SIDEBAR_OFFSET + 19, SIDEBAR_PLAYERS_OFF_Y + i * SIDEBAR_PLAYERS_GAP_Y - 1, TILE_SIZE + 1, TILE_SIZE + 1, COLOR_BORDER, false);
    }

    /* Draw credits */
    drawRect(SIDEBAR_OFFSET + 15 - 1, 364, 132, 92, COLOR_BORDER, false);
    drawRect(SIDEBAR_OFFSET + 15, 365, 130, 90, COLOR_BLACK, true);
    drawString("Credits:\n Daniel Dean\n Nik Gupta\n Rishabh Jain\n Han Thi Nguyen",
        SIDEBAR_OFFSET + 20, 370, COLOR_WHITE);
        
}

void drawFullTimebar(void) {
    drawTimebar(SIDEBAR_TIMER_WIDTH, COLOR_TIMER_BG);
}

void drawTimebar(int width, uint32_t color) {
    drawRect(SIDEBAR_OFFSET + 38, 48, SIDEBAR_TIMER_WIDTH + 4, SIDEBAR_TIMER_HEIGHT + 4, COLOR_BORDER, false);
    drawRect(SIDEBAR_OFFSET + 39, 49, SIDEBAR_TIMER_WIDTH + 2, SIDEBAR_TIMER_HEIGHT + 2, COLOR_BLACK, false);
    drawRect(SIDEBAR_OFFSET + 40, 50, SIDEBAR_TIMER_WIDTH, SIDEBAR_TIMER_HEIGHT, COLOR_TIMER_BG, true);
    drawSprite(SIDEBAR_OFFSET + 20, 50, a_clock);
    drawRect(SIDEBAR_OFFSET + 40, 50, width, SIDEBAR_TIMER_HEIGHT, color, true);
    
}

void updateTimebar(double percentage) {
    uint32_t color = fromRGB((255 * percentage), (255.0 * (1.0 - percentage)), 0);
    drawTimebar((1.0 - percentage) * (double) (SIDEBAR_TIMER_WIDTH), color);
}

void drawPlayerIcons(void) {
    for (int i = 0; i < PLAYER_NUM; i++) {
        /* Draw player icon if alive, draw dead icon if dead */
        int frame = players[i].alive ? i : 4;
        drawAnimFrame(SIDEBAR_OFFSET + 20, SIDEBAR_PLAYERS_OFF_Y + i * SIDEBAR_PLAYERS_GAP_Y, a_player_icons, frame);

        /* Draw player name */
        drawString("Player", SIDEBAR_TEXT_OFFSET, SIDEBAR_PLAYERS_OFF_Y + i * SIDEBAR_PLAYERS_GAP_Y, COLOR_SIDEBAR_FONT);
        drawString(itoa(i + 1, 10), SIDEBAR_TEXT_OFFSET + 54, SIDEBAR_PLAYERS_OFF_Y + i * SIDEBAR_PLAYERS_GAP_Y, COLOR_SIDEBAR_FONT);
    }
}

void drawPlayerScores(void) {
    for (int i = 0; i < PLAYER_NUM; i++) {
        /* Clear previous score */
        drawRect(SIDEBAR_TEXT_OFFSET + 54, SIDEBAR_PLAYERS_OFF_Y + i * SIDEBAR_PLAYERS_GAP_Y + CHAR_HEIGHT, 16, CHAR_HEIGHT, COLOR_SIDEBAR_FONT_BG, true);

        /* Draw score */
        drawString("Wins:", SIDEBAR_TEXT_OFFSET, SIDEBAR_PLAYERS_OFF_Y + i * SIDEBAR_PLAYERS_GAP_Y + CHAR_HEIGHT, COLOR_SIDEBAR_FONT);

        drawString(itoa(players[i].number_of_wins, 10), SIDEBAR_TEXT_OFFSET + 54, SIDEBAR_PLAYERS_OFF_Y + i * SIDEBAR_PLAYERS_GAP_Y + CHAR_HEIGHT, COLOR_SIDEBAR_FONT);
    }
}

void drawResultScreen(player_id winner) {

    /* Draw border */
    drawRect(RESULTS_OFFSET - 3 - 70, RESULTS_OFFSET - 3, PHYSICAL_WIDTH - 2*RESULTS_OFFSET + 5, PHYSICAL_HEIGHT - 2*RESULTS_OFFSET + 5, COLOR_BLACK, false);
    drawRect(RESULTS_OFFSET - 2 - 70, RESULTS_OFFSET - 2, PHYSICAL_WIDTH - 2*RESULTS_OFFSET + 3, PHYSICAL_HEIGHT - 2*RESULTS_OFFSET + 3, COLOR_BORDER, false);
    drawRect(RESULTS_OFFSET - 1 - 70, RESULTS_OFFSET - 1, PHYSICAL_WIDTH - 2*RESULTS_OFFSET + 1, PHYSICAL_HEIGHT - 2*RESULTS_OFFSET + 1, COLOR_BLACK, false);

    /* Draw background */
    drawRect(RESULTS_OFFSET - 70, RESULTS_OFFSET, PHYSICAL_WIDTH - 2*RESULTS_OFFSET - 1, PHYSICAL_HEIGHT - 2*RESULTS_OFFSET - 1, COLOR_RESULTS_BG, true);

    /* If no winners */
    if (winner == PLAYER_NONE) {
        drawString("Draw!", RESULTS_TEXT_X + 80, RESULTS_TEXT_Y - 30, COLOR_BLACK);
        return;
    }

    drawString("Winner", RESULTS_TEXT_X + 62, RESULTS_TEXT_Y - 50, COLOR_BLACK);
    drawRect(RESULTS_TEXT_X + 69, RESULTS_TEXT_Y - 30, TILE_SIZE + 1, TILE_SIZE + 1, COLOR_BORDER, false);
    drawSprite(RESULTS_TEXT_X + 60 - TILE_SIZE, RESULTS_TEXT_Y - 29, a_trophy);
    drawAnimFrame(RESULTS_TEXT_X + 70, RESULTS_TEXT_Y - 29, a_player_icons, winner);
    drawSprite(RESULTS_TEXT_X + 80 + TILE_SIZE, RESULTS_TEXT_Y - 29, a_trophy);
    drawString("Player", RESULTS_TEXT_X + 55, RESULTS_TEXT_Y + 10, COLOR_BLACK);
    drawString(itoa(winner + 1, 10), RESULTS_TEXT_X + 109, RESULTS_TEXT_Y + 10, COLOR_BLACK);
    drawString("Wins:", RESULTS_TEXT_X + 58, RESULTS_TEXT_Y + 26, COLOR_BLACK);
    drawString(itoa(players[winner].number_of_wins, 10), RESULTS_TEXT_X + 106, RESULTS_TEXT_Y + 26, COLOR_BLACK);
}

void drawReadyScreen(void) {

    drawSprite(120, 20, a_logo);

    drawRect(RESULTS_OFFSET - 3 - 70, RESULTS_OFFSET - 3, PHYSICAL_WIDTH - 2*RESULTS_OFFSET + 5, PHYSICAL_HEIGHT - 2*RESULTS_OFFSET + 5, COLOR_BLACK, false);
    drawRect(RESULTS_OFFSET - 2 - 70, RESULTS_OFFSET - 2, PHYSICAL_WIDTH - 2*RESULTS_OFFSET + 3, PHYSICAL_HEIGHT - 2*RESULTS_OFFSET + 3, COLOR_BORDER, false);
    drawRect(RESULTS_OFFSET - 1 - 70, RESULTS_OFFSET - 1, PHYSICAL_WIDTH - 2*RESULTS_OFFSET + 1, PHYSICAL_HEIGHT - 2*RESULTS_OFFSET + 1, COLOR_BLACK, false);

    /* Draw background */
    drawRect(RESULTS_OFFSET - 70, RESULTS_OFFSET, PHYSICAL_WIDTH - 2*RESULTS_OFFSET - 1, PHYSICAL_HEIGHT - 2*RESULTS_OFFSET - 1, COLOR_RESULTS_BG, true);

    drawString("Press TRIGGER to get ready", RESULTS_TEXT_X - 20, RESULTS_TEXT_Y - 50, COLOR_BLACK);

    for (int i = 0; i < PLAYER_NUM; i++) {
        drawRect(RESULTS_READY_X + i*70 - 1, RESULTS_TEXT_Y - 30, TILE_SIZE + 1, TILE_SIZE + 1, COLOR_BORDER, false);
        drawAnimFrame(RESULTS_READY_X + i*70, RESULTS_TEXT_Y - 29, a_player_icons, i);
        drawString("Player", RESULTS_READY_TEXT_X + i*70, RESULTS_TEXT_Y + 10, COLOR_BLACK);
        drawString(itoa(i + 1, 10), RESULTS_READY_TEXT_X + i*70 + 50, RESULTS_TEXT_Y + 10, COLOR_BLACK);
    }

    drawPlayerReady(PLAYER_THREE);
    drawPlayerReady(PLAYER_FOUR);
}

void drawPlayerReady(player_id id) {
    drawSprite(RESULTS_READY_X + id*70, RESULTS_TEXT_Y - 29, a_ready_icon);
    renderFrame();
}

void drawPlayers(void) {
    for (int i = 0; i < PLAYER_NUM; i++) {
        
        if (!(players[i].alive)) {
            continue;
        }

        uint8_t* player_sprite = NULL;
        uint8_t num_of_frames  = 0;

        switch (players[i].move_direction) {
            case DIRECTION_UP:
                player_sprite = a_bomberman_up;
                num_of_frames = A_MOVE_UP_FRAMES;
                break;
            case DIRECTION_DOWN:
                player_sprite = a_bomberman_down;
                num_of_frames = A_MOVE_DOWN_FRAMES;
                break;
            case DIRECTION_LEFT:
                player_sprite = a_bomberman_left;
                num_of_frames = A_MOVE_LEFT_FRAMES;
                break;
            case DIRECTION_RIGHT:
                player_sprite = a_bomberman_right;
                num_of_frames = A_MOVE_RIGHT_FRAMES;
                break;
            case DIRECTION_IDLE:
                break;
        }

        uint8_t anim_frame_offset = num_of_frames * i;

        if (players[i].moving) {
            players[i].current_frame = ((players[i].current_frame + 1) % num_of_frames) + anim_frame_offset;
        } else {
            players[i].current_frame = anim_frame_offset;
        }
        drawAnimFrame(players[i].screen_position.x  + OFFSET_X, players[i].screen_position.y + OFFSET_Y, player_sprite, players[i].current_frame);
    }
}

void redrawTile(uint32_t x, uint32_t y) {
    /* Draw terrain */
    if (x >= MAP_SIZE || y >= MAP_SIZE) { return; }

    /* Draw ground regardless */
    drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_terrain, (x+y)%2);

    switch (terrain_grid[x][y]) {
        case TERRAIN_GROUND:
            break;
        case TERRAIN_WALL_BREAKABLE:
            drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_terrain, 3);
            break;
        case TERRAIN_WALL_UNBREAKABLE:
            drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_terrain, 2);
            break;
    }

    /* Draw items */
    switch (items_grid[x][y]) {
        case ITEM_SPEED_UP:
            drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_item_icons, ITEM_SPEED_UP);
            break;
        case ITEM_SPEED_DOWN:
            drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_item_icons, ITEM_SPEED_DOWN);
            break;
        case ITEM_RANGE_UP:
            drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_item_icons, ITEM_RANGE_UP);
            break;
        case ITEM_RANGE_DOWN:
            drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_item_icons, ITEM_RANGE_DOWN);
            break;
        case ITEM_BOMBS_UP:
            drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_item_icons, ITEM_BOMBS_UP);
            break;
        case ITEM_BOMBS_DOWN:
            drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_item_icons, ITEM_BOMBS_DOWN);
            break;
        
        default:
            /* This shouldn't happen */
            break;
    }

    /* Draw bombs */
    int32_t timer = bomb_grid[x][y].timer;
    uint8_t* bomb_sprite = a_bomb_neutral;

    if (timer <= BOMB_EXPLODING_THRESHOLD) {
        bomb_sprite = a_bomb_exploding;
    }

    switch (bomb_grid[x][y].type) {
        case BOMB_EMPTY:
            break;
        case BOMB_TYPE_NORMAL:
            drawAnimFrame(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, bomb_sprite, bomb_grid[x][y].current_frame);
            break;
        
        default:
            /* This shouldn't happen */
            break;
    }

    /* Draw explosions */
    switch (explosion_grid[x][y].type) {
        case EXPLOSION_EMPTY:
            break;
        case EXPLOSION_TYPE_NORMAL:
            drawSprite(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_explosion);
            break;

        case EXPLOSION_TYPE_PERMANENT:
            drawSprite(x * TILE_SIZE + OFFSET_X, y * TILE_SIZE + OFFSET_Y, a_explosion);
            break;
        
        default:
            /* This shouldn't happen */
            break;
    }
}
