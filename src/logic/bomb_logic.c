#include "bomb_logic.h"
#include "../objects/renderer.h"

void countdownExplosions(void) {

    for (int x = 0; x < MAP_SIZE; x++) {
        for (int y = 0; y < MAP_SIZE; y++) {

            Explosion *explosion = &explosion_grid[x][y];

            if (explosion->type == EXPLOSION_TYPE_NORMAL) {
                explosion->timer--;

                if (explosion->timer == 0) {
                    explosion_grid[x][y].type = EXPLOSION_EMPTY; 

                    /* Set the terrain on this tile to be ground - destroy breakable wall */
                    terrain_grid[x][y] = TERRAIN_GROUND;


                    /* Update the changed tiles so explosion is no longer displayed */
                    changed_tiles[x][y] = true;
                }
            }
        }
    }

    killPlayersInExplosion();

}

bool plantBombs(void) {

    bool bomb_planted = false;
    for (int i = 0; i < PLAYER_NUM; i++) {
        if (players[i].plant_bomb) {
            players[i].plant_bomb = false;
            plantBomb(&(players[i]));
            bomb_planted = true;
        }
    }

    return bomb_planted;
}


bool countdownBombs(void) {
    bool bomb_exploded = false;

    for (int x = 0; x < MAP_SIZE; x++) {
        for (int y = 0; y < MAP_SIZE; y++) {

            Bomb *bomb = &bomb_grid[x][y];

            if (bomb->type != BOMB_EMPTY) {
                bomb->timer--;

                if ((bomb->timer) == 0) {
                    explodeBomb(bomb);
                    bomb_exploded = true;
                } else if ((bomb->timer) % BOMB_ANIMATION_CYCLES == 0) {
                    /* Update the changed tiles so bomb is rendered
                     * to indicate imminient explostion */
                    bomb->current_frame = !(bomb->current_frame);
                    changed_tiles[x][y] = true;
                }
            }
        }
    }

    killPlayersInExplosion();
    return bomb_exploded;
}

void plantBomb(Player *player) {

    /* Get the coordinates for convenience */
    int8_t x = player->tile_position.x;
    int8_t y = player->tile_position.y;

    /* Check player is alive, has enough bombs and is not standing on a bomb */
    if ((!(player->alive)) || 
        (player->current_bomb_number >= player->max_bomb_number) ||
        (bomb_grid[x][y].type != BOMB_EMPTY)) { return; }

    /* Copy the player's bomb template into the bomb grid and set its position */
    Bomb bomb = player->bomb;
    bomb.position = player->tile_position;
    bomb_grid[x][y] = bomb;

    /* Update the changed tiles so bomb is rendered */
    changed_tiles[x][y] = true;

    player->current_bomb_number++;
}

void explodeBomb(Bomb *bomb) {

    /* Check bomb is not empty */
    if (bomb->type == BOMB_EMPTY) { return; }

    /* Get the coordinates, range and explosion for convenience */
    int8_t x = bomb->position.x;
    int8_t y = bomb->position.y;
    int8_t range = bomb->range;
    Explosion *explosion = &(bomb->explosion);
    
    /* Decrement corresponding player bomb count */
    players[bomb->owner].current_bomb_number--;

    /* Remove bomb and explode tile */
    bomb_grid[x][y].type = BOMB_EMPTY;
    explodeTile(x, y, explosion);

    /* Explode tiles on the horizontal and vertical that are in range but
     * don't explode past an unbreakable wall */

    bool left_blocked  = false;
    bool right_blocked = false;
    bool up_blocked    = false;
    bool down_blocked  = false;

    for (int i = 1; i <= range; i++) {

        if (!(up_blocked)) {
            switch (terrain_grid[x][y + i]) {
                case TERRAIN_WALL_UNBREAKABLE:
                    up_blocked = true;
                    break;
                case TERRAIN_WALL_BREAKABLE:
                    explodeTile(x, y + i, explosion);
                    up_blocked = true;
                    break;
                case TERRAIN_GROUND:
                    explodeTile(x, y + i, explosion);
                    break;
            }
        }

        if (!(down_blocked)) {
            switch (terrain_grid[x][y - i]) {
                case TERRAIN_WALL_UNBREAKABLE:
                    down_blocked = true;
                    break;
                case TERRAIN_WALL_BREAKABLE:
                    explodeTile(x, y - i, explosion);
                    down_blocked = true;
                    break;
                case TERRAIN_GROUND:
                    explodeTile(x, y - i, explosion);
                    break;
            }
        }

        if (!(right_blocked)) {
            switch (terrain_grid[x + i][y]) {
                case TERRAIN_WALL_UNBREAKABLE:
                    right_blocked = true;
                    break;
                case TERRAIN_WALL_BREAKABLE:
                    explodeTile(x + i, y, explosion);
                    right_blocked = true;
                    break;
                case TERRAIN_GROUND:
                    explodeTile(x + i, y, explosion);
                    break;
            }
        }

        if (!(left_blocked)) {
            switch (terrain_grid[x - i][y]) {
                case TERRAIN_WALL_UNBREAKABLE:
                    left_blocked = true;
                    break;
                case TERRAIN_WALL_BREAKABLE:
                    explodeTile(x - i, y, explosion);
                    left_blocked = true;
                    break;
                case TERRAIN_GROUND:
                    explodeTile(x - i, y, explosion);
                    break;
            }
        }
    }
}

void explodeTile(int8_t x, int8_t y, Explosion *explosion) {

    /* Check position is valid, the explosion type is not empty and there is not
     * an unbreakable wall */
    if ((!isCoordinateInRange(x, y)) ||
        (explosion->type == EXPLOSION_EMPTY) ||
        (terrain_grid[x][y] == TERRAIN_WALL_UNBREAKABLE)) { return; }

    /* Set the explosion, overwriting any existing explosions */
    if (explosion_grid[x][y].type != EXPLOSION_TYPE_PERMANENT) {
        explosion_grid[x][y] = *explosion;
    }

    /* Set any bombs on this tile to explode on the next round */
    bomb_grid[x][y].timer = 1;

    /* Destroy any item on this tile, unless covered by a breakable wall */
    if (terrain_grid[x][y] != TERRAIN_WALL_BREAKABLE) {
        items_grid[x][y] = ITEM_EMPTY;
    } else {
        items_grid[x][y] = generateRandomItem();
    }

    /* Update the changed tiles so changes are rendered */
    changed_tiles[x][y] = true;

}

void killPlayersInExplosion(void) {
    
    Position pos_1 = {0, 0};
    Position pos_2 = {0, 0};

    for (int i = 0; i < PLAYER_NUM; i++) {
        if (explosion_grid[players[i].tile_position.x][players[i].tile_position.y].type != EXPLOSION_EMPTY) {
            players[i].alive = false;

            getOccupiedTiles(&(players[i]), &pos_1, &pos_2);

            /* Render both tiles the player occupied */
            changed_tiles[pos_1.x][pos_1.y] = true;
            changed_tiles[pos_2.x][pos_2.y] = true;

            /* Render changes in sidebar */
            drawPlayerIcons();
            drawPlayerScores();
        }
    }
}
