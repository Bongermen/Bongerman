#include "mailbox.h"

static volatile uint32_t* MAILBOX_READ   = (uint32_t*) 0x2000B880;
static volatile uint32_t* MAILBOX_STATUS = (uint32_t*) 0x2000B898;
static volatile uint32_t* MAILBOX_WRITE  = (uint32_t*) 0x2000B8A0;

/* Writes to given mailbox channel 
 * Input must have its bottom 4 bits empty (because that's where the channel data
 * will go) */
void mailboxWrite(uint32_t input, uint32_t channel) {

    /* Check for validity of arguments */
    if ((input & MAILBOX_CHANNEL_MASK) == 0 && channel <= NUM_OF_CHANNELS) {
        uint32_t status = *(MAILBOX_STATUS);

        /* Wait for mailbox to be ready */
        while (status & MAILBOX_FULL) {
            status = *(MAILBOX_STATUS);
        }

        *(MAILBOX_WRITE) = input | channel;
    }
}

/* Reads from a given mailbox channel - read from FB_CHANNEL for framebuffer data
 * Returns 0 if successful, non-zero if not */
uint32_t mailboxRead(uint32_t channel) {
    uint32_t result = 0;
    /* wait for mailbox to be ready */
    while (1) {
        if ((*(MAILBOX_STATUS) & MAILBOX_EMPTY) == 0) {
            result = *(MAILBOX_READ);
            /* Top 28 bits are actual data, bottom 4 bits are channel number */

            uint32_t result_channel = result & MAILBOX_CHANNEL_MASK;
            /* Return only results for correct channel */
            if (result_channel == channel) {
                return (result & MAILBOX_DATA_MASK); /* ignore channel data */
            }
        }
    }
} 

