#include "fft/battle.h"
#include "fft/menu.h"
#include "fft/script.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_update_menus_and_event_speed. */
s32 world_menu_update_menus_and_event_speed(u32* ot, u32 input) {
    s32 i;
    s16 value;
    s32 result;

    g_ai_workspace_ptr = (world_event_work_t*)g_battle_ai_workspace;
    if (g_menu_input_mode != MENU_INPUT_PLAYER) {
        input = g_script_unfiltered_controller_input;
    }
    world_map_store_selected_tile_data();
    value = -2;
    for (i = 3; i >= 0; i--) {
        g_menu_pending_selection[i] = value;
    }
    world_script_run_event_frame(ot, input);
    world_menu_update_panel_fade();
    result = world_menu_resolve_selection();
    if ((world_thread_is_running_asm(THREAD_SLOT_MENU) != 0
            && g_thread_contexts[THREAD_SLOT_MENU].task_id != THREAD_TASK_MINI_MENU)
        || world_thread_is_running_asm(THREAD_SLOT_MENU_CHILD) != 0
        || world_thread_is_running_asm(THREAD_SLOT_MENU_GRANDCHILD) != 0) {
        if (g_world_animation_speed_forced == 0) {
            g_world_animation_speed_forced = 1;
            g_world_saved_animation_speed = battle_state_get_vsync_interval();
            world_script_set_event_speed(2);
        }
    } else if (g_world_animation_speed_forced == 1) {
        g_world_animation_speed_forced = 0;
        world_script_set_event_speed(g_world_saved_animation_speed);
    }
    return result;
}
