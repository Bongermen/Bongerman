#include "input.h"

int controller_pins[CONTROLLER_SIGNALS_NUM];

void initialisePins(void) {
    controller_pins[CONTROLLER_ONE_IND_TRIGGER] = CONTROLLER_ONE_PINS_TRIGGER;
    controller_pins[CONTROLLER_ONE_IND_UP]      = CONTROLLER_ONE_PINS_UP;
    controller_pins[CONTROLLER_ONE_IND_DOWN]    = CONTROLLER_ONE_PINS_DOWN;
    controller_pins[CONTROLLER_ONE_IND_LEFT]    = CONTROLLER_ONE_PINS_LEFT;
    controller_pins[CONTROLLER_ONE_IND_RIGHT]   = CONTROLLER_ONE_PINS_RIGHT;
    controller_pins[CONTROLLER_TWO_IND_TRIGGER] = CONTROLLER_TWO_PINS_TRIGGER;
    controller_pins[CONTROLLER_TWO_IND_UP]      = CONTROLLER_TWO_PINS_UP;
    controller_pins[CONTROLLER_TWO_IND_DOWN]    = CONTROLLER_TWO_PINS_DOWN;
    controller_pins[CONTROLLER_TWO_IND_LEFT]    = CONTROLLER_TWO_PINS_LEFT;
    controller_pins[CONTROLLER_TWO_IND_RIGHT]   = CONTROLLER_TWO_PINS_RIGHT;
}

void updateControls(void) {
    uint32_t *pin_values = (uint32_t *) GPIO_PINS_VALUES_ADDRESS;

    uint32_t mask = 1;
    for (int i = 0; i < CONTROLLER_SIGNALS_NUM; i++) {
        controller_signals[i] = !(((*pin_values) >> (controller_pins[i])) & mask);
    }
}
