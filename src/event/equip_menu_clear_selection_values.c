#include "fft/event_equip.h"
#include "fft/menu.h"
#include "psx/types.h"

/* Set the screen's 4 selector values to 0, the first choice (g_menu_selection_values). */
void equip_menu_clear_selection_values(void) {
    s32 i;

    for (i = 3; i >= 0; i--) {
        g_menu_selection_values[i] = 0;
    }
}
