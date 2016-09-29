#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <stdint.h>

typedef enum {
    EXPLOSION_EMPTY,
    EXPLOSION_TYPE_NORMAL,
    EXPLOSION_TYPE_PERMANENT,
} explosion_type;

typedef struct {
    explosion_type type;
    int32_t timer;
} Explosion;

#endif
