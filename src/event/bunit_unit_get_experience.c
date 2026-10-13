#include "fft/event_bunit.h"

/* Row callback 5 of the unit list grid (installed by bunit_menu_init_unit_list): listed unit `unit_id`'s experience. */
s32 bunit_unit_get_experience(s32 unit_id) {
    return g_bunit_unit_data[unit_id]->experience;
}
