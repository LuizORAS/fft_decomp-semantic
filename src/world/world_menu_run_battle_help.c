/* WORLD copy of the EVENT HELPMENU battle help menu
 * (helpmenu_menu_run_battle_help), identical apart from addresses.
 */
#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Opens help for the running menu's pending request, or runs the bouncing
 * help cursor over the navigation table for that menu or the unit view.
 *
 * `help_thread_id` stays pinned: left to the allocator it takes $s4 ahead of
 * `node` (31 weighted references against 28 over nearly the same live range),
 * where the target has `node` in $s4 and the thread id in $s5. A pinned
 * register is never hoisted, so the slot offset of the wait loop is computed by
 * hand before it.
 */
void world_menu_run_battle_help(void) {
    POLY_FT4 cursor_quads[2];
    POLY_FT4 shadow_quads[2];
    help_navigation_record_t* navigation;
    s32 menu_index;
    s32 x;
    native_thread_t* help_thread;
    help_navigation_record_t* node;
    s32 y;
    s32 bob;
    s32 buffer;
    s32 u;
    s32 u_mid;
    s32 shadow_offset;
    s32 help_id; /* also the request table mode flag and the pad direction index */
    s16 request_kind;
    s32 text_section;
    s32 view_mode;
    s32 input;
    s32 quad_index;
    s32 frame; /* first walks the thread slots looking for a help request */
    register s32 help_thread_id __asm__("$21");
    s32 help_thread_offset;
    native_thread_t* scan_thread;
    help_request_t* request;
    native_thread_t* self;
    void* setup_banner;
    void* setup_unit_data;
    void* setup_billboard;
    void* setup_cursor_tile;
    s32 missing_value;

    u_mid = 0xB8;
    if (g_world_menu_transition_active != 0) {
        world_thread_exit_current();
    }
    world_script_copy_bytes(g_world_text_section_pointer_backup, g_world_text_section_pointers, 0x80);
    setup_banner = &g_world_help_active_banner;
    setup_unit_data = &g_world_help_active_unit_data;
    setup_billboard = &g_world_help_billboard;
    setup_cursor_tile = &g_world_help_menu_cursor_tile;
    g_world_text_section_pointers[HELP_TEXT_SECTION_UNIT_STATUS]
        = g_world_help_data_base + g_world_help_text_section_offsets[HELP_TEXT_SECTION_UNIT_STATUS];
    g_world_text_section_pointers[HELP_TEXT_SECTION_MENU_COMMANDS]
        = g_world_help_data_base + g_world_help_text_section_offsets[HELP_TEXT_SECTION_MENU_COMMANDS];
    g_world_text_section_pointers[HELP_TEXT_SECTION_TERRAIN]
        = g_world_help_data_base + g_world_help_text_section_offsets[HELP_TEXT_SECTION_TERRAIN];
    g_world_text_section_pointers[HELP_TEXT_SECTION_JOBS]
        = g_world_help_data_base + g_world_help_text_section_offsets[HELP_TEXT_SECTION_JOBS];
    g_world_text_section_pointers[HELP_TEXT_SECTION_ITEMS]
        = g_world_help_data_base + g_world_help_text_section_offsets[HELP_TEXT_SECTION_ITEMS];
    g_world_text_section_pointers[HELP_TEXT_SECTION_ABILITIES]
        = g_world_help_data_base + g_world_help_text_section_offsets[HELP_TEXT_SECTION_ABILITIES];
    g_world_text_section_pointers[HELP_TEXT_SECTION_SKILLSETS]
        = g_world_help_data_base + g_world_help_text_section_offsets[HELP_TEXT_SECTION_SKILLSETS];
    g_world_text_section_pointers[HELP_TEXT_SECTION_STATUSES]
        = g_world_help_data_base + g_world_help_text_section_offsets[HELP_TEXT_SECTION_STATUSES];
    self = &g_world_threads[g_world_thread_current_id];
    world_menu_copy_billboard_data_out(setup_banner, setup_unit_data, setup_billboard, setup_cursor_tile);
    if ((g_world_selected_unit_stat_summary.team_kind != 0) || (g_world_unit_join_screen_active != 0)) {
        g_world_help_require_navigation[2].destination[2] = 7;
        g_world_help_require_navigation[7].destination[3] = 2;
    }
    help_thread_id = world_thread_find_first_running_menu_thread();
    g_world_help_controller_state_ptr = world_input_get_menu_controller(0);
    menu_index = 0;
    for (quad_index = 0; quad_index < 2; quad_index++) {
        world_gfx_init_default_poly_ft4(&cursor_quads[quad_index]);
        SetSemiTrans(&cursor_quads[quad_index], 0);
        cursor_quads[quad_index].tpage = GetTPage(0, 0, 0x3C0, 0x100);
        cursor_quads[quad_index].clut = 0x7D7C;
        world_gfx_init_default_poly_ft4(&shadow_quads[quad_index]);
        SetSemiTrans(&shadow_quads[quad_index], 1);
        shadow_quads[quad_index].tpage = GetTPage(0, 2, 0x3C0, 0x100);
        shadow_quads[quad_index].clut = 0x7DBC;
    }
    if (help_thread_id < 9) {
        frame = help_thread_id;
        missing_value = -1;
        scan_thread = &g_world_threads[frame];
        for (; frame < 9; frame++) {
            request = (help_request_t*)scan_thread->function_parameter_1;
            if (request->kind != missing_value) {
                break;
            }
            scan_thread++;
        }
        if (frame != 9) {
            if (g_world_help_text_id_tables_ptr == missing_value) {
                g_world_help_text_id_tables_ptr = (s32)g_world_help_menu_text_id_tables;
            }
            request_kind = request->kind;
            if (request_kind != 0x10) {
                if (request_kind == 0x100) {
                    help_id = *request->value;
                    help_id &= 0x7FF;
                    help_id |= 0x7800;
                    g_world_sound_effect_id_to_play = MAIN_SFX_WINDOW_OPEN;
                    world_thread_start(help_thread_id - 1, world_text_character_handling_thread);
                    world_thread_set_parameters(help_thread_id - 1, 0x3B, help_id, 0);
                    *g_world_help_controller_state_ptr = 0;
                    world_thread_wait_frames(1);
                    world_menu_close_help_menu();
                } else if (request_kind == 4) {
                    help_id = g_world_action_menu_skillsets[request->selected_index];
                    if (help_id == ACTION_MENU_PSEUDO_SKILLSET_ANYTHING) {
                        *g_world_help_controller_state_ptr = 0;
                        world_thread_wait_frames(1);
                        world_menu_close_help_menu();
                    }
                    help_id |= 0x9800;
                    g_world_sound_effect_id_to_play = MAIN_SFX_WINDOW_OPEN;
                    world_thread_start(help_thread_id - 1, world_text_character_handling_thread);
                    world_thread_set_parameters(help_thread_id - 1, 0x3B, help_id, 0);
                } else if (request_kind == 0) {
                    *g_world_help_controller_state_ptr = 0;
                    world_thread_wait_frames(1);
                    world_menu_close_help_menu();
                } else if (request_kind == 5) {
                    help_id = request->table->primary_ids[request->selected_index];
                    help_id &= 0x7FF;
                    help_id |= 0x7800;
                    g_world_sound_effect_id_to_play = MAIN_SFX_WINDOW_OPEN;
                    world_thread_start(help_thread_id - 1, world_text_character_handling_thread);
                    world_thread_set_parameters(help_thread_id - 1, 0x3B, help_id, 0);
                } else if (request_kind == 0x11) {
                    help_id = request->table->primary_ids[request->selected_index];
                    help_id &= 0x7FF;
                    if (help_id == 0) {
                        *g_world_help_controller_state_ptr = 0;
                        world_thread_wait_frames(1);
                        world_menu_close_help_menu();
                    }
                    help_id |= 0x6800;
                    g_world_sound_effect_id_to_play = MAIN_SFX_WINDOW_OPEN;
                    world_thread_start(help_thread_id - 1, world_text_character_handling_thread);
                    world_thread_set_parameters(help_thread_id - 1, 0x3B, help_id, 0);
                } else if (request_kind == 0x17) {
                    help_id = request->table->primary_ids[request->selected_index];
                    help_id &= 0x7FF;
                    if (help_id == 0) {
                        *g_world_help_controller_state_ptr = 0;
                        world_thread_wait_frames(1);
                        world_menu_close_help_menu();
                    }
                    help_id += ABILITY_ID_DRAW_OUT_ASURA - ITEM_ID_ASURA_KNIFE;
                    help_id |= 0x7800;
                    g_world_sound_effect_id_to_play = MAIN_SFX_WINDOW_OPEN;
                    world_thread_start(help_thread_id - 1, world_text_character_handling_thread);
                    world_thread_set_parameters(help_thread_id - 1, 0x3B, help_id, 0);
                } else if (request_kind == 0x12) {
                    help_id = *request->table->mode_flag;
                    if (help_id == 0) {
                        help_id = request->table->primary_ids[request->selected_index];
                    } else {
                        help_id = request->table->secondary_ids[request->selected_index];
                    }
                    help_id &= 0x7FF;
                    if (help_id == 0) {
                        *g_world_help_controller_state_ptr = 0;
                        world_thread_wait_frames(1);
                        world_menu_close_help_menu();
                    }
                    help_id |= 0xA000;
                    g_world_sound_effect_id_to_play = MAIN_SFX_WINDOW_OPEN;
                    world_thread_start(help_thread_id - 1, world_text_character_handling_thread);
                    world_thread_set_parameters(help_thread_id - 1, 0x3B, help_id, 0);
                } else if (request_kind == 0x13) {
                    help_id = request->table->secondary_ids[request->selected_index];
                    text_section = help_id & 0xF800;
                    if (text_section == TEXT_ID_ABILITY_NAME_BASE) {
                        help_id &= 0x7FF;
                        help_id |= 0x7800;
                    } else if (text_section == TEXT_ID_SKILLSET_NAME_SOURCE_BASE) {
                        help_id &= 0x7FF;
                        help_id |= 0x9800;
                    } else if (text_section == TEXT_ID_ITEM_NAME_BASE) {
                        help_id &= 0x7FF;
                        if ((help_id - ITEM_ID_ASURA_KNIFE) < 0xAU) {
                            help_id += ABILITY_ID_DRAW_OUT_ASURA - ITEM_ID_ASURA_KNIFE;
                            help_id |= 0x7800;
                        } else {
                            help_id |= 0x6800;
                        }
                    } else {
                        help_id = 0x807;
                    }
                    g_world_sound_effect_id_to_play = MAIN_SFX_WINDOW_OPEN;
                    world_thread_start(help_thread_id - 1, world_text_character_handling_thread);
                    world_thread_set_parameters(help_thread_id - 1, 0x3B, help_id, 0);
                } else {
                    g_world_sound_effect_id_to_play = MAIN_SFX_WINDOW_OPEN;
                    g_world_help_topic_text_ids = (u16*)((s32*)g_world_help_text_id_tables_ptr)[request_kind];
                    world_thread_start(help_thread_id - 1, world_text_character_handling_thread);
                    world_thread_set_parameters(
                        help_thread_id - 1, 0x3B, g_world_help_topic_text_ids[request->selected_index], 0);
                }
                /* Wait for the text thread in the preceding slot. `offset + (s32)base`
                   keeps the addu operand order; pointer + offset swaps it. */
                help_thread_offset = help_thread_id << 0xA;
                do {
                    world_thread_wait_frames(1);
                } while (((native_thread_t*)(help_thread_offset + (s32)g_world_threads))[-1].is_running != 0);
                *g_world_help_controller_state_ptr = 0;
                world_thread_wait_frames(1);
                world_menu_close_help_menu();
            }
        } else {
            *g_world_help_controller_state_ptr = 0;
            world_thread_wait_frames(1);
            world_menu_close_help_menu();
        }
    }
    if (help_thread_id < 9) {
        navigation = g_world_help_require_navigation;
        help_thread_id -= 1;
    } else {
        view_mode = g_world_unit_view_mode;
        if (view_mode == UNIT_VIEW_MODE_TILE_INFO || view_mode == UNIT_VIEW_MODE_HOVERED_UNIT) {
            g_world_sound_effect_id_to_play = MAIN_SFX_WINDOW_OPEN;
            world_thread_start(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, world_text_character_handling_thread);
            world_thread_set_parameters(
                NATIVE_SCREEN_THREAD_SLOT_MESSAGE, 0x3B, g_world_help_menu_cursor_tile.surface_type + 0x5800, 0);
            g_world_threads[NATIVE_SCREEN_THREAD_SLOT_MESSAGE].task_words[3] = -0x20;
            world_thread_wait_until_inactive(NATIVE_SCREEN_THREAD_SLOT_MESSAGE);
            *g_world_help_controller_state_ptr = 0;
            world_thread_wait_frames(1);
            world_menu_close_help_menu();
        } else if (view_mode == UNIT_VIEW_MODE_HOVERED_UNIT) {
            /* Unreachable: the arm above already takes modes 1 and 2. */
            navigation = g_world_help_terrain_navigation;
            help_thread_id = 2;
        } else if (view_mode == UNIT_VIEW_MODE_UNIT_SUMMARY) {
            navigation = g_world_help_unit_box_navigation;
            help_thread_id = 2;
        } else if (view_mode == UNIT_VIEW_MODE_RIGHT_UNIT) {
            navigation = g_world_help_attack_preview_navigation;
            help_thread_id = 2;
        }
    }
    frame = 0;
    help_thread = &g_world_threads[help_thread_id];
    while (1) {
        if (help_thread->is_running == 0) {
            if (world_thread_find_running_by_task(NATIVE_THREAD_TASK_HELP_MENU) != 0) {
                g_world_thread_task_active = 0;
            }
            if (self->task_words[1] != 0) {
                self->task_words[1] = 0;
                self->task_words[0] = 0;
            }
            node = &navigation[menu_index];
            y = node->y;
            x = node->x;
            if (world_thread_find_running_by_task(NATIVE_THREAD_TASK_DEBUG_UNIT_EDITOR) != 0) {
                y += 8;
            }
            if (world_thread_find_running_by_task(NATIVE_THREAD_TASK_HELP_MENU) != 0) {
                y += 8;
            }
            input = *g_world_help_controller_state_ptr;
            if (input & PSX_PAD_CROSS) {
                g_world_sound_effect_id_to_play = MAIN_SFX_CANCEL;
                break;
            }
            if (input & PSX_PAD_CIRCLE) {
                if (node->value != 0) {
                    help_id = node->help_base + *node->value;
                    if (node->help_base == -0x6800 && g_world_help_billboard.generic_monster != 0) {
                        help_id = *node->value | 0x7800;
                        if ((*node->value & 0x7FF) == 0x7FF || (*node->value & 0x7FF) == 0) {
                            *g_world_help_controller_state_ptr = 0;
                        } else {
                            g_world_sound_effect_id_to_play = MAIN_SFX_WINDOW_OPEN;
                            world_thread_start(help_thread_id, world_text_character_handling_thread);
                            world_thread_set_parameters(help_thread_id, 0x3B, help_id, 0);
                        }
                    } else if (node->help_base == 0x7800 && (u32)((u16)*node->value - 1) >= 0x301) {
                        *g_world_help_controller_state_ptr = 0;
                    } else if (node->help_base == 0x6800 && (u32)((u16)*node->value - 1) >= 0x101) {
                        *g_world_help_controller_state_ptr = 0;
                    } else if (node->help_base == -0x6800 && (u32)((u16)*node->value - 1) >= 0x101) {
                        *g_world_help_controller_state_ptr = 0;
                    } else {
                        g_world_sound_effect_id_to_play = MAIN_SFX_WINDOW_OPEN;
                        world_thread_start(help_thread_id, world_text_character_handling_thread);
                        world_thread_set_parameters(help_thread_id, 0x3B, help_id, 0);
                    }
                } else {
                    help_id = node->help_base;
                    g_world_sound_effect_id_to_play = MAIN_SFX_WINDOW_OPEN;
                    world_thread_start(help_thread_id, world_text_character_handling_thread);
                    world_thread_set_parameters(help_thread_id, 0x3B, help_id, 0);
                }
            } else if (input & PSX_PAD_DPAD_MASK) {
                help_id = 0;
                if (!(input & PSX_PAD_UP)) {
                    if (input & PSX_PAD_DOWN) {
                        help_id = 1;
                    } else {
                        help_id = 3;
                        if (input & PSX_PAD_RIGHT) {
                            help_id = 2;
                        }
                    }
                }
                if (menu_index != node->destination[help_id]) {
                    g_world_sound_effect_id_to_play = MAIN_SFX_CURSOR_MOVE;
                }
                menu_index = node->destination[help_id];
            }
        } else {
            if (world_thread_find_running_by_task(NATIVE_THREAD_TASK_HELP_MENU) != 0) {
                g_world_thread_task_active = 1;
            }
            if (self->task_words[1] == 0) {
                self->task_words[1] = 2;
                self->task_words[0] = 0;
            }
        }
        if (self->task_words[1] != 0) {
            bob = world_menu_get_cursor_bob_offset(1);
        } else {
            bob = world_menu_get_cursor_bob_offset(0);
        }
        self->task_words[0] += g_world_event_speed;
        bob -= 4;
        buffer = frame & 1;
        if (node->vertical_cursor != 0) {
            u = 0xA8;
            cursor_quads[buffer].u0 = u;
            cursor_quads[buffer].u1 = u;
            cursor_quads[buffer].u2 = u_mid;
            cursor_quads[buffer].u3 = u_mid;
            cursor_quads[buffer].v0 = 0;
            cursor_quads[buffer].v1 = 0x10;
            cursor_quads[buffer].v2 = 0;
            cursor_quads[buffer].v3 = 0x10;
            u = 0xB8;
            shadow_quads[buffer].u0 = u;
            shadow_quads[buffer].u1 = u;
            shadow_quads[buffer].u2 = 0xC8;
            shadow_quads[buffer].u3 = 0xC8;
            shadow_quads[buffer].v0 = 0;
            shadow_quads[buffer].v1 = 0x10;
            shadow_quads[buffer].v2 = 0;
            shadow_quads[buffer].v3 = 0x10;
            cursor_quads[buffer].x0 = x;
            cursor_quads[buffer].y0 = y + bob;
            cursor_quads[buffer].x1 = x + 0x10;
            cursor_quads[buffer].y1 = y + bob;
            cursor_quads[buffer].x2 = x;
            cursor_quads[buffer].y2 = y + bob + 0x10;
            cursor_quads[buffer].x3 = x + 0x10;
            cursor_quads[buffer].y3 = y + bob + 0x10;
            (shadow_quads + buffer)->x0 = x + 2;
            (shadow_quads + buffer)->y0 = y + bob + 2;
            (shadow_quads + buffer)->x1 = x + 0x12;
            (shadow_quads + buffer)->y1 = y + bob + 2;
            (shadow_quads + buffer)->x2 = x + 2;
            (shadow_quads + buffer)->y2 = y + bob + 0x12;
            (shadow_quads + buffer)->x3 = x + 0x12;
            (shadow_quads + buffer)->y3 = y + bob + 0x12;
        } else {
            u = 0xA8;
            (cursor_quads + buffer)->u0 = u;
            (cursor_quads + buffer)->v0 = 0;
            (cursor_quads + buffer)->u1 = u_mid;
            (cursor_quads + buffer)->v1 = 0;
            (cursor_quads + buffer)->u2 = 0xA8;
            (cursor_quads + buffer)->v2 = 0x10;
            (cursor_quads + buffer)->u3 = u_mid;
            (cursor_quads + buffer)->v3 = 0x10;
            u = 0xB8;
            (shadow_quads + buffer)->u0 = u;
            (shadow_quads + buffer)->v0 = 0;
            (shadow_quads + buffer)->u1 = 0xC8;
            (shadow_quads + buffer)->v1 = 0;
            (shadow_quads + buffer)->u2 = u_mid;
            (shadow_quads + buffer)->v2 = 0x10;
            (shadow_quads + buffer)->u3 = 0xC8;
            (shadow_quads + buffer)->v3 = 0x10;
            (cursor_quads + buffer)->x0 = x + bob;
            (cursor_quads + buffer)->y0 = y;
            (cursor_quads + buffer)->x1 = x + bob + 0x10;
            (cursor_quads + buffer)->y1 = y;
            (cursor_quads + buffer)->x2 = x + bob;
            (cursor_quads + buffer)->y2 = y + 0x10;
            (cursor_quads + buffer)->x3 = x + bob + 0x10;
            (cursor_quads + buffer)->y3 = y + 0x10;
            shadow_offset = bob + 2;
            (shadow_quads + buffer)->x0 = x + shadow_offset;
            (shadow_quads + buffer)->y0 = y + 2;
            (shadow_quads + buffer)->x1 = x + shadow_offset + 0x10;
            (shadow_quads + buffer)->y1 = y + 2;
            (shadow_quads + buffer)->x2 = x + shadow_offset;
            (shadow_quads + buffer)->y2 = y + 0x12;
            (shadow_quads + buffer)->x3 = x + shadow_offset + 0x10;
            (shadow_quads + buffer)->y3 = y + 0x12;
        }
        if (self->task_words[1] != 0) {
            cursor_quads[buffer].clut = 0x7DFC;
            shadow_quads[buffer].clut = 0x7E3C;
        } else {
            cursor_quads[buffer].clut = 0x7D7C;
            shadow_quads[buffer].clut = 0x7DBC;
        }
        world_gfx_draw_or_append_gpu_primitive(&cursor_quads[buffer]);
        world_gfx_draw_or_append_gpu_primitive(&shadow_quads[buffer]);
        world_thread_wait_frames(1);
        frame++;
    }
    *g_world_help_controller_state_ptr = 0;
    world_thread_wait_frames(1);
    world_menu_close_help_menu();
}
