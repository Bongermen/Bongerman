#include "enemy_logic.h"

// Returns the distance between two points squared
int getDistanceSquared(int x1, int y1, int x2, int y2){
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

// Returns true if a block adjacent to pos is breakable
bool breakableAdjacent(Position pos) {
    for (int i = -1; i < 2; i++) {
        if (pos.x + i >= 0 && pos.x + i < MAP_SIZE) {
            if (terrain_grid[pos.x + i][pos.y] == TERRAIN_WALL_BREAKABLE) {
                return true;
            }

        }
    }
    for (int j = -1; j < 2; j++) {
        if (pos.y + j >= 0 && pos.y + j < MAP_SIZE) {
            if (terrain_grid[pos.x][pos.y + j] == TERRAIN_WALL_BREAKABLE) {
                return true;
            }
        }
    }
    return false;
}

// Returns true if pos1 and pos2 are perpendicular
bool perpendicular(Position pos1, Position pos2) {
    return (pos1.x == pos2.x) || (pos1.y == pos2.y);
}

// Returns true if a bomb at position bomb_pos with range range will hit position pos
bool inBombRange(Position pos, Position bomb_pos, int range) {
    return perpendicular(pos, bomb_pos) && (range * range) >= getDistanceSquared(pos.x, pos.y, bomb_pos.x, bomb_pos.y);
}

// Returns true if the position is in range of any bomb's explosion
bool inExplosionRange(Position pos) {
    if (explosion_grid[pos.x][pos.y].type != EXPLOSION_EMPTY) {
        return true;
    }
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (bomb_grid[i][j].type == BOMB_TYPE_NORMAL) {
                if (inBombRange(pos, (Position){i, j}, bomb_grid[i][j].range)) {
                    return true;
                }
            }
        }
    }
    return false;
}

// Returns true if the position is not in range of any bomb's explosion, is normal terrain, and is on the map (does not avoid explosions just bombs)
bool isMovable(Position pos) {
    return isCoordinateInRange(pos.x, pos.y) && (!inExplosionRange(pos)) && (terrain_grid[pos.x][pos.y] == TERRAIN_GROUND);
}

// Returns the position of the closest powerup to the player at player
Position getClosestPowerup(Player *player) {
    int min_distance_squared = MAP_SIZE * MAP_SIZE;
    int distance_squared;
    int x;
    int y;
    if (player->tile_position.x < MAP_SIZE / 2) {
        x = MAP_SIZE - 1;
    } else {
        x = 0;
    }
    if (player->tile_position.y < MAP_SIZE / 2) {
        y = MAP_SIZE - 1;
    } else {
        y = 0;
    }
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (items_grid[i][j] == ITEM_SPEED_UP || items_grid[i][j] == ITEM_RANGE_UP || items_grid[i][j] == ITEM_BOMBS_UP) {
                distance_squared = getDistanceSquared(player->tile_position.x, player->tile_position.y, i, j);
                if (distance_squared < min_distance_squared) {
                    min_distance_squared = distance_squared;
                    x = i;
                    y = j;
                }
            }
        }
    }
    return (Position){x,y};
}

// Returns the position of the closest other player to the player at player
Position getClosestPlayer(Player *player) {
    int min_distance_squared = MAP_SIZE * MAP_SIZE;
    int distance_squared;
    int x;
    int y;
    if (player->tile_position.x < MAP_SIZE / 2) {
        x = MAP_SIZE - 1;
    } else {
        x = 0;
    }
    if (player->tile_position.y < MAP_SIZE / 2) {
        y = MAP_SIZE - 1;
    } else {
        y = 0;
    }
    for (int i = 0; i < PLAYER_NUM; i++) {
        if (players[i].id != player->id && players[i].alive) {
            distance_squared = getDistanceSquared(player->tile_position.x, player->tile_position.y, players[i].tile_position.x, players[i].tile_position.y);
            if (distance_squared < min_distance_squared) {
                min_distance_squared = distance_squared;
                x = players[i].tile_position.x;
                y = players[i].tile_position.y;
            }
        }
    }
    return (Position){x,y};
}

// Returns the position of the closest bomb which can damage the player at player
Position getClosestBombInRange(Player *player) {
    int min_distance_squared = MAP_SIZE * MAP_SIZE;
    int distance_squared;
    int x;
    int y;
    if (player->tile_position.x < MAP_SIZE / 2) {
        x = MAP_SIZE - 1;
    } else {
        x = 0;
    }
    if (player->tile_position.y < MAP_SIZE / 2) {
        y = MAP_SIZE - 1;
    } else {
        y = 0;
    }
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (bomb_grid[i][j].type != BOMB_EMPTY && inBombRange(player->tile_position, (Position){i, j}, bomb_grid[i][j].range)) {
               distance_squared = getDistanceSquared(player->tile_position.x, player->tile_position.y, i, j);
               if (distance_squared < min_distance_squared) {
                min_distance_squared = distance_squared;
                x = i;
                y = j;
               }
            }
        }
    }
    return (Position){x, y};
}

// Checks if the space above/below/to the left of/to the right of pos is movable

bool checkUp(Position pos) {
    return isCoordinateInRange(pos.x, pos.y - 1) && (terrain_grid[pos.x][pos.y - 1] == TERRAIN_GROUND) && (bomb_grid[pos.x][pos.y - 1].type == BOMB_EMPTY) && (explosion_grid[pos.x][pos.y - 1].type == EXPLOSION_EMPTY);
}

bool checkDown(Position pos) {
    return isCoordinateInRange(pos.x, pos.y + 1) && (terrain_grid[pos.x][pos.y + 1] == TERRAIN_GROUND) && (bomb_grid[pos.x][pos.y + 1].type == BOMB_EMPTY) && (explosion_grid[pos.x][pos.y + 1].type == EXPLOSION_EMPTY);
}

bool checkLeft(Position pos) {
    return isCoordinateInRange(pos.x - 1, pos.y) && (terrain_grid[pos.x - 1][pos.y] == TERRAIN_GROUND) && (bomb_grid[pos.x - 1][pos.y].type == BOMB_EMPTY) && (explosion_grid[pos.x - 1][pos.y].type == EXPLOSION_EMPTY);
}

bool checkRight(Position pos) {
    return isCoordinateInRange(pos.x + 1, pos.y) && (terrain_grid[pos.x + 1][pos.y] == TERRAIN_GROUND) && (bomb_grid[pos.x + 1][pos.y].type == BOMB_EMPTY) && (explosion_grid[pos.x + 1][pos.y].type == EXPLOSION_EMPTY);
}

bool movableDir(direction dir, Position currPos) {
    switch (dir) {
            case DIRECTION_IDLE: {
                return true;
            }
            case DIRECTION_UP:{
                if (isMovable((Position){currPos.x,currPos.y - 1})) {return true;}
                break;
            }
            case DIRECTION_DOWN:{
                if (isMovable((Position){currPos.x,currPos.y + 1})) {return true;}
                break;
            }
            case DIRECTION_LEFT:{
                if (isMovable((Position){currPos.x - 1,currPos.y})) {return true;}
                break;
            }
            case DIRECTION_RIGHT:{
                if (isMovable((Position){currPos.x + 1,currPos.y})) {return true;}
                break;  
            }
    }
    return false;
}

// Returns the direction the ai player should move in to approach a powerup or player
direction getApproachMove(Player *player, Position pos) {
    Position currPos;
    currPos.x = player->tile_position.x;
    currPos.y = player->tile_position.y;
    if (pos.x - player->tile_position.x > 0) {
        if (movableDir(DIRECTION_RIGHT, currPos)) {
            return DIRECTION_RIGHT;
        }
    } else if (pos.x - player->tile_position.x < 0) {
        if (movableDir(DIRECTION_LEFT, currPos)) {
            return DIRECTION_LEFT;
        }
    }
    if (pos.y - player->tile_position.y < 0) {
        if (movableDir(DIRECTION_UP, currPos)) {
            return DIRECTION_UP;
        }
    } else if (pos.y - player->tile_position.y > 0) {
        if (movableDir(DIRECTION_DOWN, currPos)){
            return DIRECTION_DOWN;
        }
    }
    if (!(movableDir(DIRECTION_UP, currPos) || movableDir(DIRECTION_DOWN, currPos) || movableDir(DIRECTION_LEFT, currPos) || movableDir(DIRECTION_DOWN, currPos))) {
        return DIRECTION_IDLE;
    } else {
        int ranDir = getRandomLimit(3);
        while (!movableDir(ranDir, currPos)) {
            ranDir = getRandomLimit(3);
        }
        return ranDir;
    }
}

// Returns the direction to move away from the nearest bomb which can damage the player at player
direction avoidBombMove(Player *player) {
    Position pos = getClosestBombInRange(player);
    Position currPos;
    currPos.x = player->tile_position.x;
    currPos.y = player->tile_position.y;
    if (pos.x == currPos.x && pos.y == currPos.y) {
        if (checkRight(currPos)) {
            return DIRECTION_RIGHT;
        }
        if (checkLeft(currPos)) {
            return DIRECTION_LEFT;
        }
        if (checkUp(currPos)) {
            return DIRECTION_UP;
        }
        if (checkDown(currPos)) {
            return DIRECTION_DOWN;
        }
    } else if (pos.y == currPos.y) {
        if (checkUp(currPos)) {
            return DIRECTION_UP;
        }
        if (checkDown(currPos)) {
            return DIRECTION_DOWN;
        }
        if (pos.x - currPos.x > 0) {
            if (checkLeft(currPos)) {
                return DIRECTION_LEFT;
            }
        }
        if (pos.x - currPos.x < 0) {
            if (checkRight(currPos)) {
                return DIRECTION_RIGHT;
            }
        }
    } else if (pos.x == currPos.x) {
        if (checkRight(currPos)) {
            return DIRECTION_RIGHT;
        }
        if (checkLeft(currPos)) {
            return DIRECTION_LEFT;
        }
        if (pos.y - currPos.y > 0) {
            if (checkUp(currPos)) {
                return DIRECTION_UP;
            }
        }
        if (pos.y - currPos.y < 0) {
            if (checkDown(currPos)) {
                return DIRECTION_DOWN;
            }
        }
    }

    return DIRECTION_IDLE;
}

// Causes the player at player to drop a bomb if it is adjacent to a breakable tile and the player hasn't dropped a bomb (prevents suicide)
void dropBombIfBreakable(Player *player) {
    if (breakableAdjacent(player->tile_position) && player->current_bomb_number == 0) {
        player->plant_bomb = true;
    }
}

// Checks the state of the player and game and returns a plan for the ai
ai_plan getPlan(Player *player) {
    // If the ai player is moving from one tile to another its direction won't change (i.e. only changes movement when on a tile)
    if (!isAligned(player->screen_position.x, player->screen_position.y)) {
        return IDLE;
    }

    // If the ai player is in a bomb's range it will try to avoid the bomb
    else if (inExplosionRange(player->tile_position)) {
        return AVOID_BOMB;
    }

    // If the ai player is closer to a player/powerup it will approach the player/powerup (respectively)
    Position closestPowerup = getClosestPowerup(player);
    Position closestPlayer = getClosestPlayer(player);
    if (getDistanceSquared(player->tile_position.x, player->tile_position.y, closestPowerup.x, closestPowerup.y)
        < getDistanceSquared(player->tile_position.x, player->tile_position.y, closestPlayer.x, closestPlayer.y)) {
        return GET_POWERUP;
    } else {
        return APPROACH_PLAYER;
    }
}

// Given a player at player, returns the direction the player should move in following getPlan
direction get_ai_dir(Player *player) {
    direction dir = DIRECTION_IDLE;
    switch (getPlan(player)) {
        case IDLE: {
            dir = DIRECTION_IDLE;
            break;
        }
        case GET_POWERUP: {
            dir = getApproachMove(player, getClosestPowerup(player));
            dropBombIfBreakable(player);
            break;
        }
        case APPROACH_PLAYER: {
            Position pos = getClosestPlayer(player);
            dir = getApproachMove(player, pos);
            dropBombIfBreakable(player);
            if (inBombRange(pos, player->tile_position, player->bomb.range)){
                player->plant_bomb = true;
            }
            break;
        }
        case AVOID_BOMB: {
            dir = avoidBombMove(player);
            break;
        }
    }
    return dir;
}
/* Testing 
void setup_players(void){
    for (int i = 0; i < PLAYER_NUM; i++) {
        players[i].id = i;
        players[i].number_of_wins = 0;
    }
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

void setup_grids(void){
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
    for (int i = 1; i < MAP_SIZE; i += 2) {
        for (int j = 1; j < MAP_SIZE; j+=2) {
            terrain_grid[i][j] = TERRAIN_WALL_UNBREAKABLE;
        }
    }
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

bool check_player(int x, int y){
    for (int p = 0; p < PLAYER_NUM; p++) {
        if (players[p].tile_position.x == x && players[p].tile_position.y == y) {
            return true;
        }
    }
    return false;
}

void printPlan(ai_plan plan) {
    switch(plan) {
       case APPROACH_PLAYER: printf("approach player"); return;
       case GET_POWERUP: printf("get powerup"); return;
       case AVOID_BOMB: printf("avoid bomb"); return;
       case IDLE: printf("idle"); return;
    }
}

void printDir(direction dir) {
    switch (dir) {
       case DIRECTION_IDLE: printf("nowhere"); return;
       case DIRECTION_DOWN: printf("down"); return;
       case DIRECTION_UP: printf("up"); return;
       case DIRECTION_LEFT: printf("left"); return;
       case DIRECTION_RIGHT: printf("right"); return;
    }
}

int main(void) {
   setup_players();
   setup_grids();
   terrain_grid[12][10] = TERRAIN_GROUND;
   terrain_grid[0][10] = TERRAIN_GROUND;
   players[2].tile_position.y = 11;
   players[3].tile_position.y = 11;
   bomb_grid[0][10].type = BOMB_TYPE_NORMAL;
   bomb_grid[12][10].type = BOMB_TYPE_NORMAL;

   for (int i = 0; i < MAP_SIZE; i++) {
       printf("%i:\t", i);
       for (int j = 0; j < MAP_SIZE; j++) {
           if (check_player(j, i)) {
               if (bomb_grid[j][i].type == BOMB_TYPE_NORMAL) {
                printf("PB\t");
               } else {
                printf("P\t");
            }
           } else if (terrain_grid[j][i] == TERRAIN_WALL_UNBREAKABLE){
               printf("-\t");
           } else if (terrain_grid[j][i] == TERRAIN_WALL_BREAKABLE) {
               printf("X\t");
           } else if (bomb_grid[j][i].type == BOMB_TYPE_NORMAL) {
               printf("B\t");
           } else if (items_grid[j][i] == ITEM_BOMBS_UP) {
               printf("I\t");
           } else {
               printf("\t");
           }
       }
       printf("\n");
   }
   for (int i = 0; i < MAP_SIZE; i++) {
       printf("\t%i", i);
   }
   printf("\n\n\n");
   Position near_powerup;
   Position near_player;
   for (int p = 0; p < PLAYER_NUM; p++) {
       near_powerup = getClosestPowerup(&players[p]);
       near_player = getClosestPlayer(&players[p]);
       printf("Player %i (%i,%i) has plan ", p, players[p].tile_position.x, players[p].tile_position.y);
       printPlan(getPlan(&players[p]));
       printf(" and should move ");
       printDir(get_ai_dir(&players[p]));
       printf("\n");
   }
   return 0;
}  Testing End */
