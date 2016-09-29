#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include "../objects/game_state.h"

/* Architecture addresses and initialisation values */
#define GPIO_PINS_VALUES_ADDRESS 0x20200034

/* Controller Signal indices in controller signals array */
#define CONTROLLER_ONE_IND_TRIGGER 0
#define CONTROLLER_ONE_IND_UP      1
#define CONTROLLER_ONE_IND_DOWN    2
#define CONTROLLER_ONE_IND_LEFT    3
#define CONTROLLER_ONE_IND_RIGHT   4
#define CONTROLLER_TWO_IND_TRIGGER 5
#define CONTROLLER_TWO_IND_UP      6
#define CONTROLLER_TWO_IND_DOWN    7
#define CONTROLLER_TWO_IND_LEFT    8
#define CONTROLLER_TWO_IND_RIGHT   9

/* Pin out values  */
#define CONTROLLER_ONE_PINS_TRIGGER 2
#define CONTROLLER_ONE_PINS_UP      3
#define CONTROLLER_ONE_PINS_DOWN    4
#define CONTROLLER_ONE_PINS_LEFT    7
#define CONTROLLER_ONE_PINS_RIGHT   8
#define CONTROLLER_TWO_PINS_TRIGGER 9
#define CONTROLLER_TWO_PINS_UP      10
#define CONTROLLER_TWO_PINS_DOWN    11
#define CONTROLLER_TWO_PINS_LEFT    14
#define CONTROLLER_TWO_PINS_RIGHT   15

extern int controller_pins[CONTROLLER_SIGNALS_NUM];

/* Set pins 0 - 9 for input */
void initialisePins(void);

/* Updates global controller signals array by getting GPIO values*/
void updateControls(void);

#endif
