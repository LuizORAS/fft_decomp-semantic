#include "fft/event_require.h"
#include "psx/types.h"

void require_panel_copy_battle_stats_to_gauges(battle_stats_t* unit, world_unit_status_billboard_t* output) {
    u16 maximum_hp;
    u16 maximum_mp;
    u8 ct;

    output->level = unit->level;
    output->team_kind = 0;
    if (unit->initial_team_flags & BATTLE_TEAM_MASK) {
        output->team_kind = 1;
    }
    if (!(unit->initial_team_flags & BATTLE_TEAM_OR_PLAYER_CONTROL_MASK)) {
        output->team_kind = 2;
    }
    if (unit->auto_battle_setting != 0) {
        output->team_kind = 3;
    }
    output->experience = unit->experience;
    maximum_hp = unit->max_hp;
    output->max_hp = maximum_hp;
    if (maximum_hp == 0) {
        output->max_hp = maximum_hp + 1;
    }
    output->hp = unit->hp;
    output->hp_delta = 0;
    maximum_mp = unit->max_mp;
    output->max_mp = maximum_mp;
    if (maximum_mp == 0) {
        output->max_mp = maximum_mp + 1;
    }
    output->mp = unit->mp;
    output->mp_delta = 0;
    output->max_ct = 100;
    ct = unit->ct;
    output->battle_id = 0;
    output->list_index = 0;
    output->unit_count = 0;
    output->ct = ct;
}
