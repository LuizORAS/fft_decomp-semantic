#include "fft/event_bunit.h"

void bunit_menu_clear_selection_records(void) {
    memset(g_bunit_menu_selection_records, 0, sizeof(g_bunit_menu_selection_records));
}
