#include "fft/event_bunit.h"

/* Return the selected unit's maximum HP, MP, or CT value. */
s32 bunit_unit_get_max_stat(s32 unit_index) {
    s32 value;

    if (g_bunit_unit_stat_mode == MENU_UNIT_READOUT_HP) {
        value = g_bunit_unit_data[unit_index]->max_hp;
    } else if (g_bunit_unit_stat_mode == MENU_UNIT_READOUT_MP) {
        value = g_bunit_unit_data[unit_index]->max_mp;
    } else if (g_bunit_unit_stat_mode == MENU_UNIT_READOUT_CT) {
        value = UNIT_TURN_CT_THRESHOLD;
    }
    g_bunit_unit_max_stat_value = value;
    return g_bunit_unit_max_stat_value;
}
