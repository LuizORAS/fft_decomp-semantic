/* LIBGPU 80023df8-80023e0c: initializes only length and command. */
#include "psx/libgpu.h"

/* Set the packet length and command code of a Gouraud-shaded line (LINE_G2); the caller fills the other
 * fields. */
void SetLineG2(LINE_G2* primitive) {
    setLineG2(primitive);
}
