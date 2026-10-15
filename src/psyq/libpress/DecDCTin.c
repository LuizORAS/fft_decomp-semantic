#include "psx/libpress.h"

/* Start decoding the run-length codes at `buf`, which begin with the MDEC command word: `mode` bit 0 clears
 * command bit 27 for 24-bit output (without it the bit is set: 15-bit) and bit 1 sets bit 25 (the mask bit of
 * 15-bit pixels); then send the command and its data (MDEC_in). open_movie_update_stream_frame passes mode
 * 3. */
void DecDCTin(s32* buf, s32 mode) {
    if (mode & 1) {
        *buf = *buf & 0xF7FFFFFF;
    } else {
        *buf = *buf | 0x08000000;
    }
    if (mode & 2) {
        *buf = *buf | 0x02000000;
    } else {
        *buf = *buf & 0xFDFFFFFF;
    }
    MDEC_in((u32*)buf, (u16)*buf);
}
