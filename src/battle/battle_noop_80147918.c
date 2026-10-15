#include "psx/types.h"

/* Allocate and release a 48-byte stack frame, then return. Nothing on the disc calls it or holds its address. The
 * unused array reproduces the frame without claiming original locals. */
void battle_noop_80147918(void) {
    s32 unused[12];
}
