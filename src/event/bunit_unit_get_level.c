#include "fft/event_bunit.h"

/* Row callback 4 of the unit list grid (installed by bunit_menu_init_unit_list): listed unit `unit_id`'s level. */
s32 bunit_unit_get_level(s32 unit_id) {
    return g_bunit_unit_data[unit_id]->level;
}
