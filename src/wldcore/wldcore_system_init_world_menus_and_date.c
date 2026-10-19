#include "fft/script.h"
#include "fft/wldcore.h"
#include "psx/types.h"

/* Prepare WORLD's menus for the visit (world_menu_init_with_world_text_and_faces: their text, common
 * graphics and the WLDFACE portraits) and the roster's name strings, then advance the calendar by
 * the days an event left in EVENT_SCRIPT_VAR_DATE_ADVANCE (wldcore_calendar_advance_days) and clear
 * it. */
void wldcore_system_init_world_menus_and_date(void) {
    s32 value;

    world_menu_init_with_world_text_and_faces();
    world_text_generate_formation_unit_name_string();
    value = script_get_variable(EVENT_SCRIPT_VAR_DATE_ADVANCE);
    if (value != 0) {
        wldcore_calendar_advance_days(value);
        script_set_variable(EVENT_SCRIPT_VAR_DATE_ADVANCE, 0);
    }
}
