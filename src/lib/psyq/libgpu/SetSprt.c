/* LIBGPU 80023d80-80023d94: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a sprite of any size (SPRT); the caller fills the other fields. */
void SetSprt(SPRT* primitive) {
    setSprt(primitive);
}
