#include "psx/types.h"

/* Return 0. Nothing references it (QUIRKS.md). */
s32 small_return_zero(void) {
    return 0;
}
