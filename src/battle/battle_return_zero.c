#include "psx/types.h"

/* Always 0. The resident executable calls it through
 * main_overlay_call_battle_return_zero while script variable 0xff is set. */
s32 battle_return_zero(void) {
    return 0;
}
