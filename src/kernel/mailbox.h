#ifndef MAILBOX_H
#define MAILBOX_H

#include <stdint.h>

#define NUM_OF_CHANNELS 9

#define MAILBOX_TAG_CHANNEL 8

#define MAILBOX_EMPTY 0x40000000
#define MAILBOX_FULL 0x80000000

#define MAILBOX_DATA_MASK 0xfffffff0
#define MAILBOX_CHANNEL_MASK 0xf

#define MAILBOX_TAG_SUCCESS 0x80000000

#define TAG_GET_TEMP 0x30006

#define BYTE_SIZE 8
#define WORD_SIZE 4

#define TAG_FB_ALLOC 0x40001
#define TAG_FB_PHYSICAL_XY 0x48003
#define TAG_FB_VIRTUAL_XY 0x48004
#define TAG_FB_BIT_DEPTH 0x48005
#define TAG_END 0

typedef struct {
    uint32_t identifier;
    uint32_t value_buffer_size;
    uint32_t indicator;
    uint32_t x;
    uint32_t y;
} tagFormat;

typedef struct {
   uint32_t buffer_size; 
   uint32_t buffer_code;

   uint32_t identifier;
   uint32_t value_buffer_size;
   uint32_t indicator;
   uint32_t x;
   uint32_t y;

   uint32_t end_tag;
} propertyMessage;

uint32_t mailboxRead(uint32_t channel);
void mailboxWrite(uint32_t input, uint32_t channel);

#endif
