#include "fft/main.h"

/* Clear a unit's per-battle state: Auto-Battle (Manual, no target), CT, turn flag, ability outcome,
 * charged ability CT (0xff), mount and KO count. */
void main_unit_reset_battle_state(battle_stats_t* unit) {
    unit->auto_battle_setting = BATTLE_AUTO_BATTLE_MANUAL;
    unit->auto_battle_target = 0;
    unit->ct = 0;
    unit->has_turn = 0;
    unit->ability_outcome = 0;
    unit->charged_ability_ct = 0xff;
    unit->mount_info = 0;
    unit->ko_count = 0;
}
