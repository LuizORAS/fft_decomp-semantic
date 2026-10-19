#include "fft/event_bunit.h"

/* Row callback 7 of the unit list grid (installed by bunit_menu_init_unit_list): listed unit `unit_id`'s Faith. */
s32 bunit_unit_get_faith(s32 unit_id) {
    return g_bunit_unit_pointers[unit_id]->faith;
}
