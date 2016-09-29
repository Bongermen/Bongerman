#include "util.h"
#include <stdbool.h>

static uint32_t next = 1;

/* Returns true iff the given coordinate is inside the map grids. */
bool isCoordinateInRange(int8_t x, int8_t y) {
    return ((x >= 0) && (x < MAP_SIZE)
        &&  (y >= 0) && (y < MAP_SIZE));
}

/* Initialise the pseudo-random number generator with the current temperature 
    * of the device */
/* Call this function ONCE, and BEFORE pulling any random numbers from the RNG */
void initRandomSeed(void) {

    volatile uint32_t temperature[8] __attribute__((aligned(16)));
    temperature[0] = 8 * WORD_SIZE;
    temperature[1] = 0;

    temperature[2] = TAG_GET_TEMP;
    temperature[3] = 2 * WORD_SIZE;
    temperature[4] = WORD_SIZE;
    temperature[5] = 0;
    temperature[6] = 0;

    temperature[7] = TAG_END;

    mailboxWrite((uintptr_t)&temperature, MAILBOX_TAG_CHANNEL);
    mailboxRead(MAILBOX_TAG_CHANNEL);

    next = temperature[6];
}

/* Gets random unsigned integer using the linear feedback shift register 
 * Range: [1,  2^16 - 1) */
uint16_t getRandom(void) {
    uint16_t bit = 0;
    bit  = ((next >> 0) ^ (next >> 2) ^ (next >> 3) ^ (next >> 5)) & 1;
    return next = (next >> 1) | (bit << 15);
}

/* Returns a random number between 0 and limit inclusive. */
uint16_t getRandomLimit(uint16_t limit) {

    uint16_t divisor = 32767/(limit+1);
    uint16_t retval;

    do { 
        retval = getRandom() / divisor;
    } while (retval > limit);

    return retval;
}

uint32_t fromLittleEndian(uint8_t* address) {
    uint32_t result = 0;
    for (int32_t i = 3; i > -1; i--) {
        result |= address[i];
        if (i > 0) {
            result <<= 8;
        }
    }
    return result;
}

char* itoa(int32_t val, int32_t base){

    static char buf[BUFF_SIZE] = {0};
    bool isNeg = val<0;
    int32_t i = BUFF_SIZE-2;

    if(val==0) {
    	buf[i--] = '0';
    }

    if(isNeg) {
    	val = -val;
    }

    for(; val && i ; --i, val /= base) {
        buf[i] = "0123456789abcdef"[val % base];
    }
    if(isNeg) {
    	buf[i--] = '-';
    }

    return &buf[i+1];

}


char* u_itoa(uint32_t val, int32_t base){

    static char buf[BUFF_SIZE] = {0};
    int32_t i = BUFF_SIZE-2;

    if(val==0) {
    	buf[i--] = '0';
    }

    for(; val && i ; --i, val /= base) {
        buf[i] = "0123456789abcdef"[val % base];
    }
    return &buf[i+1];

}
