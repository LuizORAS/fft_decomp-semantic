#include "psx/libpress.h"

/* Set the MDEC output-depth and mask bits of the 32-bit command from mode,
 * then start the input DMA. */
void DecDCTin(u32* buf, s32 mode) {
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
    MDEC_in(buf, (u16)*buf);
}
