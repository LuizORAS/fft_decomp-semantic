#include "psx/libpress.h"

s32 DecDCTBufSize(u32* bs) {
    /* The bitstream length is the low halfword of its first header word. */
    return (u16)*bs;
}
