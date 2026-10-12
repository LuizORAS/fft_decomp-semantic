#include "fft/battle_effect.h"
#include "psx/types.h"

/* Returns the battle_effect_element_index_e of an element mask: 0 for no
 * element, otherwise the position of the first table mask it meets (9 when it
 * meets none). */
s32 battle_effect_get_ninja_ball(s32 elements) {
    s32 i;

    if (elements != 0) {
        for (i = 0; i < 9; i++) {
            if ((elements & g_battle_effect_ninja_ball_masks[i]) != 0) {
                break;
            }
        }
        elements = i;
    }
    return elements;
}
