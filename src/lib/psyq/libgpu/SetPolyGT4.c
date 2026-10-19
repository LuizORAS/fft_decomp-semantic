/* LIBGPU 80023d44-80023d58: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a Gouraud-shaded textured quadrilateral (POLY_GT4); the caller
 * fills the other fields. About 190 functions use it. */
void SetPolyGT4(POLY_GT4* primitive) {
    setPolyGT4(primitive);
}
