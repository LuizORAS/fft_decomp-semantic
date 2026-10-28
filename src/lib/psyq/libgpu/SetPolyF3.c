/* LIBGPU 80023cb8-80023ccc: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a flat-shaded triangle (POLY_F3); the caller fills the other
 * fields. */
void SetPolyF3(POLY_F3* primitive) {
    setPolyF3(primitive);
}
