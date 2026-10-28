/* LIBGPU 80023d58-80023d6c: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of an 8x8 sprite (SPRT_8); the caller fills the other fields. */
void SetSprt8(SPRT_8* primitive) {
    setSprt8(primitive);
}
