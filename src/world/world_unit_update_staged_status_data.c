#include "fft/battle_ai.h"
#include "fft/battle_gfx.h"
#include "fft/battle_unit.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Saves or restores the event status snapshot for every unit record of a unit.
 *
 * With restore clear, each resolved battle unit is snapshotted and its graphics are
 * refreshed on the main stack; otherwise the snapshot is replayed through
 * world_unit_apply_staged_status_data. Follow-up flags add 40-frame waits. */
void world_unit_update_staged_status_data(u16 unit_id, u16 restore) {
    s32 filter_mode;
    s32 unit_index;
    s32 battle_id;
    s32 battle_unit_index;

    g_world_unit_status_staging_data = &g_ai_workspace_ptr->status_staging;
    if (g_world_menu_input_mode == MENU_INPUT_PLAYER && world_unit_prepare_event_filter(&unit_id, &filter_mode) != 0) {
        g_world_unit_status_staging_data->jump_float_flags = 0;
        for (unit_index = 0; unit_index < BATTLE_UNIT_SLOT_COUNT; unit_index++) {
            if (world_script_filter_unit_id_by_mode(&unit_id, &unit_index, &filter_mode) != 0) {
                battle_unit_index = battle_unit_get_battle_id_by_runtime_id(unit_id);
                if (battle_unit_index != -1) {
                    battle_unit_find_stats_by_entd_unit_id(
                        battle_unit_get_stats_from_battle_id(battle_unit_index)->unit_id, &battle_id);
                    if (battle_id >= 0) {
                        if (restore == 0) {
                            if (world_unit_save_staged_status_data(battle_unit_index) != 0) {
                                g_thread_call_target = (void (*)(void))battle_unit_update_graphics_by_runtime_id;
                                thread_call_on_main_stack(unit_id);
                            }
                        } else {
                            world_unit_apply_staged_status_data(battle_unit_index, unit_id);
                        }
                    }
                }
                if (filter_mode == 0) {
                    break;
                }
            }
        }
        if (g_world_unit_status_staging_data->jump_float_flags != 0) {
            thread_wait_frames(40);
        }
        if (g_world_unit_status_staging_data->jump_float_flags & UNIT_STATUS_STAGING_JUMP_REMOVED) {
            thread_wait_frames(40);
        }
        if (restore == 0) {
            g_thread_call_target = battle_gfx_reset_jumping_unit_graphic_triggers;
            thread_call_on_main_stack();
        }
    }
}
