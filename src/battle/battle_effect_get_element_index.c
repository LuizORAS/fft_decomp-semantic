#include "fft/battle.h"
#include "psx/types.h"

/* Returns the battle_effect_element_index_e of an element mask: 0 for no
 * element, otherwise the position of the first table mask it meets (9 when it
 * meets none). */
s32 battle_effect_get_element_index(s32 elements) {
    s32 i;

    if (elements != 0) {
        for (i = 0; i < 9; i++) {
            if ((elements & g_battle_effect_element_masks[i]) != 0) {
                break;
            }
        }
        elements = i;
    }
    return elements;
}
