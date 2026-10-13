#include "fft/battle_script.h"
#include "fft/battle_unit.h"
#include "fft/script.h"
#include "psx/types.h"

/* Battle twin of world_script_change_stats. ChangeStats event instruction: add a signed delta to one stat of each
 * resolved unit.
 *
 * Parameters: 16-bit unit id, stat selector (event_change_stats_e), then a
 * 16-bit delta. HP clamps to max HP and the other stats to 0-100. */
void script_change_stats(const event_change_stats_parameters_t* parameters) {
    u16 unit_id;
    s32 filter_mode;
    s32 unit_index;
    s32 battle_id;
    s32 value;
    s16 delta;
    s32 battle_unit_index;
    battle_stats_t* unit;

    delta = script_read_s16(parameters->delta_le);
    unit_id = script_read_s16(parameters->unit_id_le);
    if (script_prepare_unit_filter(&unit_id, &filter_mode) != 0) {
        unit_index = 0;
        do {
            if (script_filter_unit_id_by_mode(&unit_id, (u16*)&unit_index, &filter_mode) != 0) {
                battle_unit_index = battle_unit_get_battle_id_by_runtime_id(unit_id);
                if (battle_unit_index != -1) {
                    unit = battle_unit_find_stats_by_entd_unit_id(
                        battle_unit_get_stats_from_battle_id(battle_unit_index)->unit_id, &battle_id);
                    if (battle_id >= 0) {
                        if (parameters->stat == EVENT_CHANGE_STATS_HP) {
                            value = unit->hp + delta;
                            script_clamp_s32_to_range(&value, 0, unit->max_hp);
                            unit->hp = value;
                        }
                        if (parameters->stat == EVENT_CHANGE_STATS_CT) {
                            value = unit->ct + delta;
                            script_clamp_s32_to_range(&value, 0, UNIT_TURN_CT_THRESHOLD);
                            unit->ct = value;
                        }
                        if (parameters->stat == EVENT_CHANGE_STATS_BRAVE) {
                            value = unit->brave + delta;
                            script_clamp_s32_to_range(&value, 0, UNIT_BRAVE_FAITH_MAX);
                            unit->brave = value;
                        }
                        if (parameters->stat == EVENT_CHANGE_STATS_FAITH) {
                            value = unit->faith + delta;
                            script_clamp_s32_to_range(&value, 0, UNIT_BRAVE_FAITH_MAX);
                            unit->faith = value;
                        }
                    }
                }
                if (filter_mode == 0) {
                    break;
                }
            }
            unit_index++;
        } while (unit_index < EVENT_UNIT_SLOT_COUNT);
    }
}
