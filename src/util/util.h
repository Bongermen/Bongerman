#ifndef UTIL_H
#define UTIL_H
#define BUFF_SIZE 50

#include <stdint.h>
#include "../kernel/mailbox.h"
#include "../objects/position.h"
#include "../objects/game_state.h"

bool isCoordinateInRange(int8_t x, int8_t y);
uint32_t fromLittleEndian(uint8_t* address);
char* itoa(int32_t val, int32_t base);
char* u_itoa(uint32_t val, int32_t base);
void initRandomSeed(void);
uint16_t getRandom(void);
uint16_t getRandomLimit(uint16_t limit);

#endif
