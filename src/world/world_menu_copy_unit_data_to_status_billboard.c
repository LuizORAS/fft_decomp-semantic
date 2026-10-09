#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

void world_menu_copy_unit_data_to_status_billboard(battle_stats_t* unit, battle_unit_status_record_t* output) {
    battle_stats_t* linked_unit;
    s32 resolved_id;
    s32 count;
    s32 i;
    s32 unit_id;
    u16 value;
    u16 mp;
    u8 ct;

    linked_unit = unit;
    output->level = linked_unit->level;
    output->team_kind = 0;
    if (linked_unit->initial_team_flags & BATTLE_TEAM_MASK)
        output->team_kind = 1;
    if (!(linked_unit->initial_team_flags & BATTLE_TEAM_OR_PLAYER_CONTROL_MASK))
        output->team_kind = 2;
    if (linked_unit->auto_battle_setting != BATTLE_AI_AUTOBATTLE_MANUAL)
        output->team_kind = 3;
    output->experience = linked_unit->experience;
    value = linked_unit->max_hp;
    output->gauges[BATTLE_UNIT_STATUS_GAUGE_HP].max = value;
    if (value == 0) {
        value++;
        output->gauges[BATTLE_UNIT_STATUS_GAUGE_HP].max = value;
    }
    output->gauges[BATTLE_UNIT_STATUS_GAUGE_HP].value = linked_unit->hp;
    output->gauges[BATTLE_UNIT_STATUS_GAUGE_HP].bonus = 0;
    value = linked_unit->max_mp;
    output->gauges[BATTLE_UNIT_STATUS_GAUGE_MP].max = value;
    if (value == 0) {
        value++;
        output->gauges[BATTLE_UNIT_STATUS_GAUGE_MP].max = value;
    }
    mp = linked_unit->mp;
    output->gauges[BATTLE_UNIT_STATUS_GAUGE_MP].bonus = 0;
    output->gauges[BATTLE_UNIT_STATUS_GAUGE_CT].max = 100;
    output->gauges[BATTLE_UNIT_STATUS_GAUGE_MP].value = mp;
    ct = linked_unit->ct;
    output->list_index = 0;
    output->unit_count = 0;
    output->gauges[BATTLE_UNIT_STATUS_GAUGE_CT].value = ct;
    if (output->list_index >= 0)
        output->list_index++;
    output->unit_count = BATTLE_UNIT_SLOT_COUNT;
    count = 0;
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        if (battle_unit_has_misc_id(i) != 0) {
            unit_id = battle_unit_get_battle_index_by_misc_id(i);
            linked_unit = battle_unit_find_stats_by_entd_unit_id(
                battle_unit_get_stats_from_battle_id(unit_id)->unit_id, &resolved_id);
            if (resolved_id >= 0 && unit_id == resolved_id && linked_unit->unit_id != 0) {
                count++;
            }
        }
    }
    output->unit_count = count;
}
