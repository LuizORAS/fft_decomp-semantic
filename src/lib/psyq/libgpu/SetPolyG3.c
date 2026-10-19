/* LIBGPU 80023ce0-80023cf4: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a Gouraud-shaded triangle (POLY_G3); the caller fills the other
 * fields. */
void SetPolyG3(POLY_G3* primitive) {
    setPolyG3(primitive);
}
