#include "fft/battle_unit.h"
#include "fft/world.h"
#include "psx/types.h"

/* ChangeStats event instruction: add a signed delta to one stat of each resolved unit.
 *
 * Parameters: 16-bit unit id, stat selector (event_change_stats_e), then a
 * 16-bit delta. HP clamps to max HP and the other stats to 0-100. */
void world_script_change_stats(const event_change_stats_parameters_t* parameters) {
    u16 unit_id;
    s32 misc_index;
    s32 unit_index;
    s32 battle_id;
    s32 value;
    s16 delta;
    s32 battle_unit_index;
    battle_stats_t* unit;

    delta = world_script_load_halfword(parameters->delta_le);
    unit_id = world_script_load_halfword(parameters->unit_id_le);
    if (world_unit_try_get_misc_data_by_unit_id(&unit_id, &misc_index) != 0) {
        for (unit_index = 0; unit_index < BATTLE_UNIT_SLOT_COUNT; unit_index++) {
            if (world_script_filter_unit_id_by_mode(&unit_id, &unit_index, &misc_index) != 0) {
                battle_unit_index = battle_unit_get_battle_index_by_misc_id(unit_id);
                if (battle_unit_index != -1) {
                    unit = battle_unit_find_stats_by_entd_unit_id(
                        battle_unit_get_stats_from_battle_id(battle_unit_index)->unit_id, &battle_id);
                    if (battle_id >= 0) {
                        if (parameters->stat == EVENT_CHANGE_STATS_HP) {
                            value = unit->hp + delta;
                            world_script_clamp_s32_to_range(&value, 0, unit->max_hp);
                            unit->hp = value;
                        }
                        if (parameters->stat == EVENT_CHANGE_STATS_CT) {
                            value = unit->ct + delta;
                            world_script_clamp_s32_to_range(&value, 0, UNIT_TURN_CT_THRESHOLD);
                            unit->ct = value;
                        }
                        if (parameters->stat == EVENT_CHANGE_STATS_BRAVE) {
                            value = unit->brave + delta;
                            world_script_clamp_s32_to_range(&value, 0, UNIT_BRAVE_FAITH_MAX);
                            unit->brave = value;
                        }
                        if (parameters->stat == EVENT_CHANGE_STATS_FAITH) {
                            value = unit->faith + delta;
                            world_script_clamp_s32_to_range(&value, 0, UNIT_BRAVE_FAITH_MAX);
                            unit->faith = value;
                        }
                    }
                }
                if (misc_index == 0) {
                    break;
                }
            }
        }
    }
}
