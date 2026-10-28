#include "psx/libpress.h"

/* Return the first 16-bit value of the BS frame at `bs`: the size of its expanded run-length codes, in 32-bit values
 * (psx-spx). No code refers to it. */
s32 DecDCTBufSize(u32* bs) {
    /* The bitstream length is the low 16 bits of its first 32-bit header value. */
    return (u16)*bs;
}
