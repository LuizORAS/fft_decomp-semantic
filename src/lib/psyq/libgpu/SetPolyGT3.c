/* LIBGPU 80023cf4-80023d08: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a Gouraud-shaded textured triangle (POLY_GT3); the caller fills
 * the other fields. */
void SetPolyGT3(POLY_GT3* primitive) {
    setPolyGT3(primitive);
}
