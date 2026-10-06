#include "fft/battle.h"
#include "psx/types.h"

/* Stage or restore the event status snapshot of the units an event names (unit_id: one ENTD
 * unit, 0 for all, or a team filter; battle_unit_try_get_misc_data_by_unit_id). restore 0 snapshots
 * each unit and strips the statuses an event hides (battle_update_unit_status_and_staged_status_data),
 * refreshing its graphics; restore 1 replays the snapshot (battle_unit_apply_staged_status_data).
 * Waits 40 frames when a unit lost Jump or Float, 40 more after Jump. Does nothing while
 * g_battle_menu_input_disabled is set. The event interpreter stages every unit at the start of an
 * event when script variable 0x1fd is set and restores them at its end; the Inflict Status thread
 * stages its unit. The WORLD twin is world_unit_update_staged_status_data. */
void battle_unit_update_staged_status_data(u16 unit_id, u16 restore) {
    s32 misc_index;
    s32 unit_index;
    s32 battle_id;
    s32 battle_unit_index;

    g_battle_unit_status_staging_data = &g_battle_ai_workspace_ptr->event.status_staging;
    if (g_battle_menu_input_disabled == 0 && battle_unit_try_get_misc_data_by_unit_id(&unit_id, &misc_index) != 0) {
        unit_index = 0;
        g_battle_unit_status_staging_data->flags = 0;
        do {
            if (battle_script_filter_unit_id_by_mode(&unit_id, (u16*)&unit_index, &misc_index) != 0) {
                battle_unit_index = battle_unit_get_battle_index_by_misc_id(unit_id);
                if (battle_unit_index != -1) {
                    battle_find_unit_data_pointer_for_entd_unit_id(
                        battle_unit_get_stats_from_battle_id(battle_unit_index)->unit_id, &battle_id);
                    if (battle_id >= 0) {
                        if (restore == 0) {
                            if (battle_update_unit_status_and_staged_status_data(battle_unit_index) != 0) {
                                g_battle_thread_call_target
                                    = (void (*)(void))battle_unit_update_graphics_by_misc_id_wrapper;
                                battle_thread_call_on_main_stack(unit_id);
                            }
                        } else {
                            battle_unit_apply_staged_status_data(battle_unit_index, unit_id);
                        }
                    }
                }
                if (misc_index == 0) {
                    break;
                }
            }
            unit_index++;
        } while (unit_index < BATTLE_UNIT_SLOT_COUNT);
        if (g_battle_unit_status_staging_data->flags != 0) {
            battle_thread_wait_frames(40);
        }
        if (g_battle_unit_status_staging_data->flags & 1) {
            battle_thread_wait_frames(40);
        }
        if (restore == 0) {
            g_battle_thread_call_target = battle_gfx_reset_jumping_unit_graphic_triggers;
            battle_thread_call_on_main_stack();
        }
    }
}
