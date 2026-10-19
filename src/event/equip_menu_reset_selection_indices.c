#include "fft/event_equip.h"
#include "psx/types.h"

/* Fill the 4 s16 selection-index slots ending at
 * g_equip_menu_result_rows with -1 (writing backwards). */
void equip_menu_reset_selection_indices(void) {
    s16 sentinel = -1;
    s32 remaining;

    for (remaining = 3; remaining >= 0; remaining--) {
        g_equip_menu_result_rows[remaining] = sentinel;
    }
}
