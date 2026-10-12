#include "fft/battle.h"
#include "fft/battle_map.h"
#include "fft/battle_script.h"
#include "fft/battle_unit.h"
#include "psx/types.h"

/* Request an animation for a unit by misc id: what the event instructions
 * and the pending-animation list call.
 *
 * While EVENT_SCRIPT_VAR_BATTLE_NOT_STARTED is 0 and
 * EVENT_SCRIPT_VAR_SHOP_ITEM_AVAILABILITY is not, an animation below 500 is
 * not given to a unit that rides or is ridden, which takes its status
 * animation instead, nor to one in water two or more deep, which takes the
 * one of battle_unit_set_animation_based_on_mounted_state. Both run on the
 * main stack. WORLD twin: world_unit_set_animation. */
void battle_unit_set_animation(s32 misc_id, u32 animation) {
    s32 battle_id;
    battle_stats_t* stats;
    s32 depth;

    if ((script_get_variable(EVENT_SCRIPT_VAR_BATTLE_NOT_STARTED) == 0)
        && (script_get_variable(EVENT_SCRIPT_VAR_SHOP_ITEM_AVAILABILITY) != 0)) {
        battle_id = battle_unit_get_battle_index_by_misc_id(misc_id);
        if ((battle_id >= 0) && (animation < BATTLE_SEQUENCE_EVTCHR_SLOT_0)) {
            stats = battle_unit_get_stats_from_battle_id(battle_id);
            depth = map_get_tile_data_value(
                MAP_TILE_DATA_DEPTH, stats->x, stats->position.y, stats->position.higher_elevation);
            if ((stats->mount_info & BATTLE_MOUNT_INFO_ROLE_MASK) != 0) {
                g_battle_thread_call_target = (void (*)(void))battle_unit_set_status_animation_by_misc_id;
            } else if ((depth & 0xFF) >= 2) {
                g_battle_thread_call_target = (void (*)(void))battle_unit_set_mounted_animation_by_misc_id;
            } else {
                battle_unit_set_specific_animation_by_misc_id(misc_id, animation);
                return;
            }
            battle_thread_call_on_main_stack(misc_id);
            return;
        }
    }
    battle_unit_set_specific_animation_by_misc_id(misc_id, animation);
}
