#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../kernel/main.h"
#include "../kernel/mailbox.h"
#include "../util/util.h"
#include "../kernel/assets.h"

#define CACHE_OFFSET 0x40000000

#define FB_CHANNEL 1

#define PHYSICAL_WIDTH 640
#define PHYSICAL_HEIGHT 480
#define VIRTUAL_WIDTH PHYSICAL_WIDTH
#define VIRTUAL_HEIGHT PHYSICAL_HEIGHT
#define BIT_DEPTH 32

#define RED_MASK 0xf8
#define GREEN_MASK 0xfc
#define BLUE_MASK 0xf8

#define COLOR_RED 0xffff0000
#define COLOR_GREEN 0xff00ff00
#define COLOR_BLUE 0xff0000ff
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0xff000000

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

typedef struct {
    uint32_t physical_width;
    uint32_t physical_height;
    uint32_t virtual_width;
    uint32_t virtual_height;
    uint32_t gpu_pitch;
    uint32_t bit_depth;
    uint32_t offset_x;
    uint32_t offset_y;
    uint32_t gpu_pointer;
    uint32_t gpu_size;

} frameBuffer;

void renderFrame(void) __attribute__((optimize("prefetch-loop-arrays")));
void initFrameBuffer(void);
uint32_t fromRGB(uint8_t red, uint8_t green, uint8_t blue);
uint32_t fromRGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

/* Utility functions */
inline uint32_t clampX(uint32_t x) { return x >= VIRTUAL_WIDTH ? VIRTUAL_WIDTH - 1 : x; }
inline uint32_t clampY(uint32_t y) { return y >= VIRTUAL_HEIGHT ? VIRTUAL_HEIGHT - 1 : y; }

void drawFilledCircle(int32_t x, int32_t y, uint32_t radius, uint32_t colour);
void drawSprite(int32_t x, int32_t y, uint8_t* sprite);
void drawAnimFrame(int32_t x, int32_t y, uint8_t* sprite, uint32_t frame);
void drawCharacter(char c, int32_t x, int32_t y, uint32_t colour, uint8_t* font);
void drawString(char* string, int32_t x, int32_t y, uint32_t colour);
void drawPixel(int32_t x, int32_t y, uint32_t colour);
void drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t colour);
void drawRect(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t colour, bool filled);
void clearScreen(uint32_t colour);

#endif 
