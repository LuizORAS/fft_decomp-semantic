#include "fft/battle_unit.h"
#include "fft/script.h"
#include "fft/world.h"
#include "psx/types.h"

/* Face a unit, and its mount when one is linked, in the given direction.
   Script variable 0x1FD suppresses the mount update. */
void world_unit_set_facing(s32 runtime_id, s32 facing) {
    s32 battle_id;
    battle_stats_t* stats;

    if (script_get_variable(EVENT_SCRIPT_VAR_BATTLE_NOT_STARTED) == 0) {
        battle_id = battle_unit_get_battle_id_by_runtime_id(runtime_id);
        if (battle_id >= 0) {
            stats = battle_unit_get_stats_from_battle_id(battle_id);
            if (stats->mount_info != 0) {
                battle_unit_set_animation_from_facing_nibble(
                    battle_unit_get_runtime_id_by_battle_id(stats->mount_info & BATTLE_MOUNT_INFO_PARTNER_ID_MASK),
                    facing & 0xFFFF);
            }
        }
    }
    battle_unit_set_animation_from_facing_nibble(runtime_id, facing & 0xFFFF);
}
