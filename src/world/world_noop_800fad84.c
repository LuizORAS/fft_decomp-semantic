#include "psx/types.h"

/* Create and release a 0x30-byte frame, then return. Nothing on the disc calls it or holds its address. */
void world_noop_800fad84(void) {
    u8 unused[48];

    (void)unused;
}
