#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

/* ChangeStats event instruction: add a signed delta to one stat of each resolved unit.
 *
 * Parameters: unit id halfword, stat selector (event_change_stats_e), then a
 * halfword delta. HP clamps to max HP and the other stats to 0-100. */
void world_script_change_stats(const u8* parameters) {
    u16 unit_id;
    s32 misc_index;
    s32 unit_index;
    s32 battle_id;
    s32 value;
    s16 delta;
    s32 battle_unit_index;
    battle_stats_t* unit;

    delta = world_script_load_halfword(parameters + 3);
    unit_id = world_script_load_halfword(parameters);
    if (world_unit_try_get_misc_data_by_unit_id(&unit_id, &misc_index) != 0) {
        unit_index = 0;
        do {
            if (world_script_filter_unit_id_by_mode(&unit_id, (u16*)&unit_index, &misc_index) != 0) {
                battle_unit_index = battle_unit_get_battle_index_by_misc_id(unit_id);
                if (battle_unit_index != -1) {
                    unit = battle_unit_find_stats_by_entd_unit_id(
                        battle_unit_get_stats_from_battle_id(battle_unit_index)->unit_id, &battle_id);
                    if (battle_id >= 0) {
                        if (parameters[2] == EVENT_CHANGE_STATS_HP) {
                            value = unit->hp + delta;
                            world_script_clamp_s32_to_range(&value, 0, unit->max_hp);
                            unit->hp = value;
                        }
                        if (parameters[2] == EVENT_CHANGE_STATS_CT) {
                            value = unit->ct + delta;
                            world_script_clamp_s32_to_range(&value, 0, 100);
                            unit->ct = value;
                        }
                        if (parameters[2] == EVENT_CHANGE_STATS_BRAVE) {
                            value = unit->brave + delta;
                            world_script_clamp_s32_to_range(&value, 0, 100);
                            unit->brave = value;
                        }
                        if (parameters[2] == EVENT_CHANGE_STATS_FAITH) {
                            value = unit->faith + delta;
                            world_script_clamp_s32_to_range(&value, 0, 100);
                            unit->faith = value;
                        }
                    }
                }
                if (misc_index == 0) {
                    break;
                }
            }
            unit_index++;
        } while (unit_index < BATTLE_UNIT_SLOT_COUNT);
    }
}
