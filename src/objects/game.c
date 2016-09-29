#include "game.h"

void runGame(void) {

    initialisePins();
    initRandomSeed();
    initFrameBuffer();
    initialisePlayers();
    setupRound();

    while (readyScreen()) {

        player_id winner = playRound();

        for(int j = 0; j < PLAYER_NUM; j++) {
            if(winner == players[j].id) {
                players[j].number_of_wins++;
            }
        }
        drawPlayerScores();
        snd_stop_pickup();
        snd_stop_explosion();
        snd_stop_plant();
        snd_play_crowd_cheer();
        for (int i = 0; i < 100; i++) {
            drawResultScreen(winner);
            renderFrame();
            update_sound();
        }
        snd_stop_crowd_cheer();
        update_sound();
    }
}

void initialisePlayers(void) {
    for (int i = 0; i < PLAYER_NUM; i++) {
        players[i].id = i;
        players[i].number_of_wins = 0;
    }
}

bool readyScreen(void) {
    
    drawReadyScreen();
    renderFrame();
    bool player1_ready = false;
    bool player2_ready = false;

    while(!(player1_ready && player2_ready)) {
        updateControls();
        if(!player1_ready && controller_signals[CONTROLLER_ONE_IND_TRIGGER] == 1) {
            drawPlayerReady(PLAYER_ONE);
            player1_ready = true;
            snd_play_pickup();
        }
        if(!player2_ready && controller_signals[CONTROLLER_TWO_IND_TRIGGER] == 1) {
            drawPlayerReady(PLAYER_TWO);
            player2_ready = true;
            snd_play_pickup();
        }
        update_sound();
    }

    while(player1_ready || player2_ready) {
        updateControls();
        if(player1_ready && controller_signals[CONTROLLER_ONE_IND_TRIGGER] == 0) {
            drawPlayerReady(PLAYER_ONE);
            player1_ready = false;
        }
        if(player2_ready && controller_signals[CONTROLLER_TWO_IND_TRIGGER] == 0) {
            drawPlayerReady(PLAYER_TWO);
            player2_ready = false;
        }
        update_sound();
    }
    return true;

}
