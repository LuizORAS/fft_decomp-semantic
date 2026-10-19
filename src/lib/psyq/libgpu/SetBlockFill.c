/* LIBGPU 80023e8c-80023ea0: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a VRAM fill command, in a TILE-sized packet; the caller fills
 * the other fields. psyq_gs_reset_screen_state primes LIBGS's clear tiles with it. */
void SetBlockFill(TILE* primitive) {
    setBlockFill(primitive);
}
