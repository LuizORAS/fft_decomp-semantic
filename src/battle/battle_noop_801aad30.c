#include "fft/battle.h"
#include "psx/types.h"

/* Store both arguments to their incoming stack home slots and return. Nothing on the disc calls it or holds its
 * address. Taking their addresses reproduces those stores; it does not establish what the original body contained. */
void battle_noop_801aad30(s32 ignored_1, s32 ignored_2) {
    s32* unused;
    unused = &ignored_1;
    unused = &ignored_2;
}
