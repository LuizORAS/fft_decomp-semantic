/* LIBGPU 80023d30-80023d44: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a Gouraud-shaded quadrilateral (POLY_G4); the caller fills the
 * other fields. About 13 functions use it. */
void SetPolyG4(POLY_G4* primitive) {
    setPolyG4(primitive);
}
