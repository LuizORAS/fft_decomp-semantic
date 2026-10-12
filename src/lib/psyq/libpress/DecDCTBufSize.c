#include "psx/libpress.h"

s32 DecDCTBufSize(u32* bs) {
    /* The bitstream length is the low 16 bits of its first 32-bit header value. */
    return (u16)*bs;
}
