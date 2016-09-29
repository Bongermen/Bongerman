#include "graphics.h"

uint32_t* base_gpu_ptr = NULL;

static uint32_t gpu_ptr[PHYSICAL_WIDTH*PHYSICAL_HEIGHT] __attribute__((aligned(16)));

void renderFrame(void) {
    for (int i = 0; i < PHYSICAL_WIDTH * PHYSICAL_HEIGHT; i++) {
        int32_t pixel = gpu_ptr[i];

        if (pixel != 0) {
            base_gpu_ptr[i] = pixel;
            gpu_ptr[i] = 0;
        }
    }
}

/* Prints a string */
void drawString(char* string, int32_t x, int32_t y, uint32_t colour) { 

    int32_t count = 0;
    char c = string[count];
    uint32_t dx = 0;

    while (c != '\0') {
        if (c == '\n') {
            y += CHAR_HEIGHT;
            dx = 0;
        } else {
            drawCharacter(c, x + dx, y, colour, font);
            dx += CHAR_WIDTH;
        }
        count++;
        c = string[count]; 
    }
}

void drawSprite(int32_t x, int32_t y, uint8_t* sprite) {

    /* Get dimensions */
    uint32_t height = fromLittleEndian(sprite);
    sprite += WORD_SIZE;
    uint32_t width = fromLittleEndian(sprite);
    sprite += WORD_SIZE;

    for (uint32_t j = 0; j < height; j++) {
        for (uint32_t i = 0; i < width; i++) {

            uint8_t r = *(sprite++);
            uint8_t g = *(sprite++);
            uint8_t b = *(sprite++);
            uint8_t a = *(sprite++);

            /* Transparency */
            if (a > 0) {
                drawPixel(x + i, y + j, fromRGBA(r, g, b, a));
            }
        }
    }
}

/* Draws a specific frame from a sprite sheet */
void drawAnimFrame(int32_t x, int32_t y, uint8_t* sprite, uint32_t frame) {

    /* Get animation properties */
    uint32_t total_frames = *(sprite);
    sprite += WORD_SIZE;
    uint32_t frame_height = *(sprite);
    sprite += WORD_SIZE;
    uint32_t frame_width = *(sprite);
    sprite += WORD_SIZE;

    /* Get dimensions */
    uint32_t height = fromLittleEndian(sprite);
    sprite += WORD_SIZE;
    uint32_t width = fromLittleEndian(sprite);
    sprite += WORD_SIZE;

    height++; //temp to suppress warning

    frame = frame % total_frames;
    uint32_t frames_per_row = width / frame_width;
    uint32_t offset_x = (frame % frames_per_row) * frame_width * WORD_SIZE;
    uint32_t offset_y = (frame / frames_per_row) * frame_height * WORD_SIZE * width;

    sprite += offset_x;
    sprite += offset_y;
    
    for (uint32_t j = 0; j < frame_height; j++) {
        for (uint32_t i = 0; i < frame_width; i++) {

            uint8_t r = *(sprite++);
            uint8_t g = *(sprite++);
            uint8_t b = *(sprite++);
            uint8_t a = *(sprite++);

            /* Transparency */
            if (a > 0) {
                drawPixel(x + i, y + j, fromRGBA(r, g, b, a));
            }
        }

        sprite += WORD_SIZE * (width - frame_width);
    }
}

/* Converts from rgba values to a single 32-bit value that can be written
 * directly to the graphics pointer */
uint32_t fromRGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    uint32_t result = 0;
    result |= alpha;
    result <<= BYTE_SIZE;

    result |= red;
    result <<= BYTE_SIZE;

    result |= green;
    result <<= BYTE_SIZE;

    result |= blue;

    return result;
}

/* Converts from rgb values to a single 32-bit value that can be written
 * directly to the graphics pointer with a default 100% opacity setting */
uint32_t fromRGB(uint8_t red, uint8_t green, uint8_t blue) {
    return fromRGBA(red, green, blue, 0xff);
}

/* Draws filled circle using Bresenham's circle algorithm */
void drawFilledCircle(int32_t x, int32_t y, uint32_t radius, uint32_t colour) {
    int32_t x0 = radius;
    int32_t y0 = 0;
    int32_t radius_error = 1 - x0;

    while (x0 >= y0) {
        drawLine(-y0 + x, -x0 + y, y0 + x, -x0 + y, colour);
        drawLine(-x0 + x, -y0 + y, x0 + x, -y0 + y, colour);
        drawLine(-x0 + x,  y0 + y, x0 + x,  y0 + y, colour);
        drawLine(-y0 + x,  x0 + y, y0 + x,  x0 + y, colour);

        y0++;
        if (radius_error < 0) {
            radius_error += 2 * y0 + 1;
        } else {
            x0--;
            radius_error += 2 * (y0 - x0 + 1);
        }
    }
}

/* Draws filled/not filled rectangles */
void drawRect(int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t colour, bool filled) {
    if (width == 0 || height == 0) {
        return;
    }
    if (!filled) {
        drawLine(x, y, x + width, y, colour);
        drawLine(x, y, x, y + height, colour);
        drawLine(x, y + height, x + width, y + height, colour);
        drawLine(x + width, y, x + width, y + height, colour);
    } else {
        for (int i = x; i <= x + width; i++) {
            drawLine(i, y, i, y + height, colour);
        }
    }
}

void clearScreen(uint32_t colour) {
    drawRect(0, 0, PHYSICAL_WIDTH - 1, PHYSICAL_HEIGHT - 1, colour, true);
}

/* Draws a line using Bresenham's line algorithm */
void drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t colour) {
    int32_t dx = x1 - x0;
    dx = (dx >= 0) ? dx : -dx; /* Abs */
    int32_t dy = y1 - y0;
    dy = (dy >= 0) ? dy : -dy; /* Abs */

    int32_t sx = x0 < x1 ? 1 : -1;
    int32_t sy = y0 < y1 ? 1 : -1;

    int32_t err = dx - dy;
    int32_t e2;
    bool done = false;

    while (!done) {
        drawPixel(x0, y0, colour);
        if ((x0 == x1) && (y0 == y1)) {
            done = true;
        } else {
            e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y0 += sy;
            }
        }
    }
}

void drawPixel(int32_t x, int32_t y, uint32_t colour) {
    if (x < 0 || y < 0 || x >= PHYSICAL_WIDTH || y >= PHYSICAL_HEIGHT) {
        return;
    }

    gpu_ptr[x + y * PHYSICAL_WIDTH] = colour;
}

/* Helper function for drawString */
void drawCharacter(char c, int32_t x, int32_t y, uint32_t colour, uint8_t* font) {
    if (c > 127) { return; }

    uint32_t charIndex = c * CHAR_HEIGHT;

    for (uint32_t i = 0; i < CHAR_HEIGHT; i++) {
        for (uint32_t j = 0; j < CHAR_WIDTH; j++) {
            uint8_t bits = font[charIndex + i];

            bool status = (bits >> j) & 1;
            if (status) {
                drawPixel(x + j, y + i, colour);
            }
        }
    }
}

/* Returns graphics pointer if initialization of the frame buffer succeeded
 * Return 16bit pointers because of the bit depth */
void initFrameBuffer(void) {
    /* Align to 16 bytes to ensure bottom 4 bits are empty (for channel data) */
    volatile uint32_t fb[22] __attribute__((aligned(16)));
    int i = 0;

    fb[i++] = 22 * WORD_SIZE; //size
    fb[i++] = 0; //request

    /* alloc */
    fb[i++] = TAG_FB_ALLOC; //request
    fb[i++] = 2 * WORD_SIZE; //size
    fb[i++] = WORD_SIZE; //len
    fb[i++] = 0; //address
    fb[i++] = 0; //size

    /* physical xy */
    fb[i++] = TAG_FB_PHYSICAL_XY; //request
    fb[i++] = 2 * WORD_SIZE; //size
    fb[i++] = 2 * WORD_SIZE; //len
    fb[i++] = PHYSICAL_WIDTH; //width
    fb[i++] = PHYSICAL_HEIGHT; //height

    /* virtual xy */
    fb[i++] = TAG_FB_VIRTUAL_XY; //request
    fb[i++] = 2 * WORD_SIZE; //size
    fb[i++] = 2 * WORD_SIZE; //len
    fb[i++] = VIRTUAL_WIDTH; //width
    fb[i++] = VIRTUAL_HEIGHT; //height

    /* bit depth */
    fb[i++] = TAG_FB_BIT_DEPTH; //request
    fb[i++] = WORD_SIZE; //size
    fb[i++] = WORD_SIZE; //len
    fb[i++] = BIT_DEPTH; //depth

    fb[i] = TAG_END; //end

    mailboxWrite((uintptr_t)&fb, MAILBOX_TAG_CHANNEL);
    mailboxRead(MAILBOX_TAG_CHANNEL);

    if (fb[1] == MAILBOX_TAG_SUCCESS) {
        base_gpu_ptr = (uint32_t*) (intptr_t) fb[5];
    }

}

