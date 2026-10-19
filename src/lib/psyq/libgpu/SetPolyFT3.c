/* LIBGPU 80023ccc-80023ce0: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a textured triangle (POLY_FT3); the caller fills the other
 * fields. */
void SetPolyFT3(POLY_FT3* primitive) {
    setPolyFT3(primitive);
}
