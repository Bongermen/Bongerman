#include "main.h"

extern void dummy(int);

int kernel_main(void) {

    setup_sound();
    runGame();
    while (1) {
        dummy(1);
    }
}
