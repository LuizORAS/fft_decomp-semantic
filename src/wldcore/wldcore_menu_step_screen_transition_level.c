#include "fft/wldcore.h"
#include "psx/libetc.h"
#include "psx/libgpu.h"

void wldcore_menu_step_screen_transition_level(wldcore_menu_screen_transition_level_t* level) {
    RECT rect;

    if (level->delay == 0) {
        if (level->phase == 0) {
            DrawSync(0);
            rect.x = 0;
            rect.y = 0;
            rect.w = 0x100;
            rect.h = 0xF0;
            StoreImage(&rect, (u32*)g_wldcore_scratch_buffer);
            DrawSync(0);
            VSync(0);
            world_thread_set_parameters(WLDCORE_THREAD_SLOT_TRANSITION, 0, 0, 1);
            g_wldcore_loaded_background_set = -1;
            level->phase++;
            return;
        }
        if (world_thread_is_running(WLDCORE_THREAD_SLOT_TRANSITION) != 0) {
            return;
        }
        g_wldcore_menu_stack_depth--;
        switch (level->screen) {
        case WLDCORE_MENU_LEVEL_BRAVE_STORY:
            world_thread_start(WLDCORE_THREAD_SLOT_MESSAGE, world_text_message_box_thread);
            world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, 0xB848, 0);
            wldcore_menu_push_brave_story_level();
            break;
        case WLDCORE_MENU_LEVEL_STORY_EVENTS:
            world_thread_start(WLDCORE_THREAD_SLOT_MESSAGE, world_text_message_box_thread);
            world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, 0xB848, 0);
            wldcore_menu_push_story_event_text_level(g_main_replay_story_event_index);
            break;
        case WLDCORE_MENU_LEVEL_TUTORIAL_CATEGORIES:
            world_thread_start(WLDCORE_THREAD_SLOT_MESSAGE, world_text_message_box_thread);
            world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, 0xB8DD, 0);
            wldcore_list_open_tutorial_categories(level->param_a, level->param_b);
            break;
        case WLDCORE_MENU_LEVEL_FORMATION_TUTORIALS:
            world_thread_start(WLDCORE_THREAD_SLOT_MESSAGE, world_text_message_box_thread);
            world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, 0xB8DD, 0);
            g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                         .list_window.main_window]
                .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
            g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                         .list_window.side_window]
                .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
            g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                                .list_window.frame_render]
                .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
            wldcore_list_open_formation_tutorials();
            break;
        case WLDCORE_MENU_LEVEL_TUTORIAL_MASK_1_ENTRIES:
            world_thread_start(WLDCORE_THREAD_SLOT_MESSAGE, world_text_message_box_thread);
            world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, 0xB8DD, 0);
            g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                         .list_window.main_window]
                .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
            g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                         .list_window.side_window]
                .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
            g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1]
                                                .list_window.frame_render]
                .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
            wldcore_list_open_tutorial_mask_1_entries();
            break;
        case WLDCORE_MENU_LEVEL_TREASURE_DETAIL:
            world_thread_start(WLDCORE_THREAD_SLOT_MESSAGE, world_text_message_box_thread);
            world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, 0xB848, 0);
            wldcore_menu_push_treasure_detail_level(level->param_a);
            break;
        }
    } else {
        level->delay--;
    }
}
