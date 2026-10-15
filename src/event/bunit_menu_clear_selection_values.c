#include "fft/event_bunit.h"
#include "psx/types.h"

/* Set the screen's 16 selector values to 0, the first choice (g_bunit_menu_selection_values). */
void bunit_menu_clear_selection_values(void) {
    s32 index;

    for (index = 15; index >= 0; index--) {
        g_bunit_menu_selection_values[index] = 0;
    }
}
