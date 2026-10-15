#include "psx/libpress.h"

/* Return the first halfword of the BS frame at `bs`: the size of its expanded run-length codes, in words
 * (psx-spx). No code refers to it. */
s32 DecDCTBufSize(const u16* bs) {
    return *bs;
}
