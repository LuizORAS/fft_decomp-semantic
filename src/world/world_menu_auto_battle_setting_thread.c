#include "fft/battle_menu.h"
#include "fft/menu.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Formation-screen auto-battle setting menu thread.
 *
 * Initialize the cursor from the unit's current policy. Circle redraws the
 * five choices; choosing manual control clears the mode and rebuilds the idle
 * action menu if its menu id changes.
 */
void world_menu_auto_battle_setting_thread(void) {
    RECT rect;
    menu_icon_record_t records[2];
    s32 cursor;
    s32 previous;
    battle_stats_t* stats;
    menu_entry_t* param;
    u8* buffer;
    s32 i;
    s32 j;
    s32 fresh;
    s32 before;
    s32 after;
    s32 selected;
    s16* colour;
    menu_icon_record_t* record;

    param = world_thread_get_current_parameter_1();
    cursor = param->selected_index;
    stats = world_unit_get_battle_stats_for_stored();
    if (stats->auto_battle_setting == AI_AUTOBATTLE_MANUAL) {
        cursor = 0;
    } else if (stats->auto_battle_setting == AI_AUTOBATTLE_TARGETED) {
        cursor = 1;
    } else if (stats->auto_battle_setting == AI_AUTOBATTLE_RECOVERY) {
        cursor = 2;
    } else if (stats->auto_battle_setting == AI_AUTOBATTLE_FADING_LIFE) {
        cursor = 3;
    } else {
        cursor = 4;
    }
    previous = cursor;
    world_menu_build_icon_record(&rect, param, &records[0]);
    world_script_copy_bytes(&records[1], &records[0], sizeof(records[1]));
    fresh = 0;
    i = 0;
    colour = g_world_auto_battle_option_colors;
    for (;; i++) {
        record = &records[i & 1];
        if (i == 0 || (g_world_menu_new_button_input & PSX_PAD_CIRCLE)) {
            buffer = world_menu_build_and_upload_window_frame_image(
                param->inner_width, param->inner_height, &rect, MENU_WINDOW_FRAME_BUILD);
            if (((g_world_menu_new_button_input & PSX_PAD_CIRCLE) && world_thread_is_previous_running() == 0)
                || i == 0) {
                selected = cursor;
                if (selected == 0) {
                    before = battle_menu_get_id_based_on_mount_moveable_actable(g_world_active_unit_data.battle_id);
                    stats->auto_battle_setting = AI_AUTOBATTLE_MANUAL;
                    g_menu_pending_selection[0] = 0x12;
                    colour[0] = 8;
                    colour[1] = 4;
                    colour[2] = 4;
                    colour[3] = 4;
                    colour[4] = 4;
                    after = battle_menu_get_id_based_on_mount_moveable_actable(g_world_active_unit_data.battle_id);
                    if (before != after) {
                        world_thread_set_parameters(NATIVE_THREAD_SLOT_MENU, 0, 0, 1);
                        world_menu_build_idle_action_stub(after);
                    }
                } else {
                    colour[0] = 4;
                    colour[1] = 0;
                    colour[2] = 0;
                    colour[3] = 0;
                    colour[4] = 0;
                    colour[selected] = 8;
                }
                g_world_menu_auto_battle_row_actions[1] = BATTLE_MENU_ID_AUTO_BATTLE_CONFIRM;
                g_world_menu_auto_battle_row_actions[2] = BATTLE_MENU_ID_AUTO_BATTLE_CONFIRM;
                g_world_menu_auto_battle_row_actions[3] = BATTLE_MENU_ID_AUTO_BATTLE_CONFIRM;
                g_world_menu_auto_battle_row_actions[4] = BATTLE_MENU_ID_AUTO_BATTLE_CONFIRM;
                if (cursor == previous) {
                    g_world_menu_auto_battle_row_actions[1] = BATTLE_MENU_ID_AUTO_BATTLE_TARGET;
                    g_world_menu_auto_battle_row_actions[2] = BATTLE_MENU_ID_AUTO_BATTLE_TARGET;
                }
                previous = cursor;
                g_world_menu_thread_menu_data[BATTLE_MENU_ID_AUTO_BATTLE_TARGET].selected_index = 0;
                world_menu_handle_entry_confirm(param, previous);
            }
            for (j = 0; j < 5; j++) {
                g_world_menu_text_color = colour[j];
                g_world_menu_text_state.pen.stride = param->inner_width;
                world_menu_set_text_origin(8, j * 16 + 9);
                world_menu_display_text_entry(j + 0x5009, buffer, &g_world_menu_text_state.pen);
            }
            LoadImage(&rect, buffer);
            fresh = 1;
        }
        thread_yield();
        if (fresh != 0) {
            world_menu_free_memory(buffer);
            fresh = 0;
        }
        param->selected_index = cursor;
        if (world_menu_check_thread_completion(&g_world_menu_new_button_input) != 0) {
            break;
        }
        world_menu_step_wrapping_cursor_on_scroll_buttons(param, &cursor);
        world_menu_cancel_thread_group(param);
        world_menu_select_icon_cluts(&record->base);
        world_menu_update_icon_cursor_sprites(param, &record->base, i, cursor);
        world_menu_submit_icon_primitives(&record->base);
    }
    thread_yield();
    thread_exit_current();
}
