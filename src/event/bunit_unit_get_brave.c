#include "fft/event_bunit.h"

/* Row callback 6 of the unit list grid (installed by bunit_menu_init_unit_list): listed unit `unit_id`'s Brave. */
s32 bunit_unit_get_brave(s32 unit_id) {
    return g_bunit_unit_data[unit_id]->brave;
}
