#include "fft/battle.h"

/* Apply one command record that follows the resource rows of a map's GNS file and
 * step g_battle_map_gns_record_cursor past it.
 *
 * The indoor, outdoor, snow, 0x8a and weather-modifier commands always run. Any
 * other command runs only when the requested map state compares with the record's
 * map state as its comparison asks. The requested state is the low 12 bits of the
 * script variable the record names (the layout) packed with the script's weather
 * and time of day. Only the low byte of the record's variable id and of its map
 * state is read.
 *
 * Rows are applied in file order, so the last row of a kind wins; type 0x87 has
 * no dispatcher case.
 *
 * Only phase 2 does anything, and it is the only phase the map loader passes.
 * Always returns 0. */
s32 battle_process_map_gns_record(s32 phase, battle_map_resource_t* record) {
    char unused_zeroes[8] = { 0 };
    s32 unused_sizes[4] = { 0x1000, 0x1000, 0x1000, 0 };
    s32 requested_layout;
    s32 requested_layout_and_weather;
    battle_map_resource_t* selected;

    if (phase == 2) {
        /* Everything below works on this copy; the parameter itself dies
         * here, which keeps it out of a callee-saved register. */
        selected = record;
        if (selected->command.resource_type == GNS_RESOURCE_SET_INDOOR) {
            battle_map_dispatch_gns_resource(GNS_RESOURCE_SET_INDOOR, selected);
        } else if (selected->command.resource_type == GNS_RESOURCE_SET_OUTDOOR) {
            battle_map_dispatch_gns_resource(GNS_RESOURCE_SET_OUTDOOR, selected);
        } else if (selected->command.resource_type == GNS_RESOURCE_CLEAR_SNOW) {
            battle_map_dispatch_gns_resource(GNS_RESOURCE_CLEAR_SNOW, selected);
        } else if (selected->command.resource_type == GNS_RESOURCE_SET_SNOW) {
            battle_map_dispatch_gns_resource(GNS_RESOURCE_SET_SNOW, selected);
        } else if (selected->command.resource_type == GNS_RESOURCE_UNHANDLED_8A) {
            battle_map_dispatch_gns_resource(GNS_RESOURCE_UNHANDLED_8A, selected);
        } else if (selected->command.resource_type == GNS_RESOURCE_SET_WEATHER_MODIFIER) {
            battle_map_dispatch_gns_resource(GNS_RESOURCE_SET_WEATHER_MODIFIER, selected);
        } else {
            /*
             * Requested map state, packed like the file row's map_state: layout in
             * bits 0-11, weather in 12-14, time of day in 15.  The target reads
             * the record's variable id and map state as single bytes (lbu), not
             * as the 16-bit values the struct declares, so those two stay byte reads.
             */
            requested_layout = battle_script_get_variable(*(u8*)&selected->command.script_variable_id) & 0xfff;
            requested_layout_and_weather
                = requested_layout | ((battle_script_get_variable(EVENT_SCRIPT_VAR_WEATHER) & 7) << 12);
            if (battle_map_is_gns_record_matching_state(selected->command.map_state_comparison,
                    *(u8*)&selected->command.map_state,
                    requested_layout_and_weather
                        | ((battle_script_get_variable(EVENT_SCRIPT_VAR_TIME_OF_DAY) & 1) << 15))) {
                battle_map_dispatch_gns_resource(selected->command.resource_type, selected);
            }
        }
        g_battle_map_gns_record_cursor += selected->command.record_byte_length;
    }
    return 0;
}
