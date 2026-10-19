#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Set the screen's 8 selector values to 0, the first choice (g_jobstts_menu_selection_values). */
void jobstts_menu_clear_selection_values(void) {
    s32 index;

    for (index = 7; index >= 0; index--) {
        g_jobstts_menu_selection_values[index] = 0;
    }
}
