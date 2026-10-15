#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Set the screen's 8 selector values to 0, the first choice (g_jobstts_menu_selection_values). */
void jobstts_menu_clear_selection_values(void) {
    s32 index = 7;
    u16* value = &g_jobstts_menu_selection_values[7];

    do {
        *value = 0;
        index--;
        value--;
    } while (index >= 0);
}
