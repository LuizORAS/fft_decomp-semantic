/* LIBGPU 80023ea0-80023ebc. */
#include "psx/libgpu.h"

/* Prepare a VRAM copy packet: its length, a cache clear and the copy command; the caller adds the source and
 * destination. The battle map loader and the overlays' screens copy VRAM areas with it. */
void SetDrawMove(DR_MOVE* primitive) {
    setDrawMove(primitive);
}
