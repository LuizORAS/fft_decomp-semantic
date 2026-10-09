#include "fft/event_bunit.h"

s32 bunit_unit_get_current_stat(s32 unit_index) {
    menu_unit_readout_e mode;
    bunit_unit_data_t* entry;
    s32 value;

    mode = g_bunit_unit_stat_mode;
    if (mode == MENU_UNIT_READOUT_HP) {
        entry = g_bunit_unit_data[unit_index];
        value = entry->current_hp;
    } else if (mode == MENU_UNIT_READOUT_MP) {
        entry = g_bunit_unit_data[unit_index];
        value = entry->current_mp;
    } else if (mode == MENU_UNIT_READOUT_CT) {
        entry = g_bunit_unit_data[unit_index];
        value = entry->current_ct;
    }
    g_bunit_unit_current_stat_value = value;
    return g_bunit_unit_current_stat_value;
}
