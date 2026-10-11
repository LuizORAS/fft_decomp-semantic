#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

/* Face a unit in the given direction, and its mount or rider too once the battle has started
 * (EVENT_SCRIPT_VAR_BATTLE_NOT_STARTED clear). */
void world_unit_set_facing(s32 misc_id, s32 facing) {
    s32 battle_id;
    battle_stats_t* stats;

    if (world_script_get_variable(EVENT_SCRIPT_VAR_BATTLE_NOT_STARTED) == 0) {
        battle_id = battle_unit_get_battle_index_by_misc_id(misc_id);
        if (battle_id >= 0) {
            stats = battle_unit_get_stats_from_battle_id(battle_id);
            if (stats->mount_info != 0) {
                battle_unit_set_animation_from_facing_nibble(
                    battle_unit_get_misc_id_by_battle_id(stats->mount_info & BATTLE_MOUNT_INFO_PARTNER_ID_MASK),
                    facing & 0xFFFF);
            }
        }
    }
    battle_unit_set_animation_from_facing_nibble(misc_id, facing & 0xFFFF);
}
