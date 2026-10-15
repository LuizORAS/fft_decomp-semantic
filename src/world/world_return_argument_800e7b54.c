#include "psx/types.h"

/* Return `value` unchanged, as world_return_argument_800e7b4c right before it does. Nothing on the disc
 * calls it or holds its address. */
s32 world_return_argument_800e7b54(s32 value) {
    return value;
}
