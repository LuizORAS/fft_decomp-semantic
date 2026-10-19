/* LIBGPU 80023d08-80023d1c: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a flat-shaded quadrilateral (POLY_F4); the caller fills the
 * other fields. About 15 functions use it. */
void SetPolyF4(POLY_F4* primitive) {
    setPolyF4(primitive);
}
