#include "psx/libpress.h"

/* Start decoding the run-length codes at `buf`, which begin with the MDEC command word: `mode` bit 0 clears
 * command bit 27 for 24-bit output (without it the bit is set: 15-bit) and bit 1 sets bit 25 (the mask bit of
 * 15-bit pixels); then send the command and its data (MDEC_in). open_movie_update_stream_frame passes mode
 * 3. */
void DecDCTin(u32* buf, s32 mode) {
    if (mode & PSYQ_PRESS_MODE_RGB24) {
        *buf &= ~PSYQ_MDEC_COMMAND_RGB16;
    } else {
        *buf |= PSYQ_MDEC_COMMAND_RGB16;
    }
    if (mode & PSYQ_PRESS_MODE_SET_BIT15) {
        *buf |= PSYQ_MDEC_COMMAND_SET_BIT15;
    } else {
        *buf &= ~PSYQ_MDEC_COMMAND_SET_BIT15;
    }
    MDEC_in(buf, (u16)*buf);
}
