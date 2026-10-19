/* LIBGPU 80023d1c-80023d30: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a textured quadrilateral (POLY_FT4); the caller fills the other
 * fields. About 20 functions use it. */
void SetPolyFT4(POLY_FT4* primitive) {
    setPolyFT4(primitive);
}
