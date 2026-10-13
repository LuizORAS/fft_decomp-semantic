#include "fft/menu.h"

/* Request SMALL without waiting for a pending companion load to finish. */
void menu_request_open_small(void) {
    g_menu_companion_executable_request_index = COMPANION_OVERLAY_SMALL;
}
