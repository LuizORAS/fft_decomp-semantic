#include "psx/libpress.h"

/* Receive `size` 32-bit values of decoded pixels into `buf` by DMA (MDEC_out). OPEN's movie player takes each
 * slice of the picture this way. */
void DecDCTout(u32* buf, s32 size) {
    MDEC_out(buf, size);
}
