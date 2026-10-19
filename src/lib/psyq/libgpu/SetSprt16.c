/* LIBGPU 80023d6c-80023d80: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a 16x16 sprite (SPRT_16); the caller fills the other fields. */
void SetSprt16(SPRT_16* primitive) {
    setSprt16(primitive);
}
