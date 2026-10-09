#include "fft/event_helpmenu.h"
#include "psx/libgpu.h"
#include "psx/pad.h"
#include "psx/types.h"

void battle_thread_start(s32 thread_id, void (*function)(void));

/* Run the battle help menu thread: open the help text for the running menu's
 * pending request, or move the bouncing help cursor over the navigation table
 * of that menu or of the unit view.
 *
 * `text_thread_id` stays pinned: left to the allocator it takes $s4 ahead of
 * `node` (31 weighted references against 28 over nearly the same live range),
 * where the target has `node` in $s4 and the thread id in $s5. A pinned
 * register is never hoisted, so the slot offset of the wait loop is computed by
 * hand before it.
 */
void helpmenu_menu_run_battle_help(void) {
    POLY_FT4 cursor_polys[2];
    POLY_FT4 shadow_polys[2];
    help_navigation_record_t* navigation;
    s32 menu_index;
    s32 x;
    native_thread_t* text_thread;
    help_navigation_record_t* node;
    s32 y;
    s32 bob;
    s32 buffer;
    s32 u;
    s32 u_mid;
    s32 shadow_offset;
    s32 help_id; /* also the request table mode flag and the pad direction index */
    s16 request_kind;
    s32 text_base;
    s32 help_mode;
    s32 input;
    s32 poly_index;
    s32 frame; /* first walks the thread slots looking for a help request */
    register s32 text_thread_id __asm__("$21");
    s32 wait_slot_offset;
    native_thread_t* scan_thread;
    help_request_t* request;
    native_thread_t* self_thread;
    void* banner;
    void* unit_data;
    void* billboard;
    void* cursor_tile;
    s32 missing_value;

    u_mid = 0xB8;
    if (g_option_menu_open != 0) {
        battle_thread_exit_current();
    }
    battle_copy_bytes(g_helpmenu_text_saved_pointers, g_battle_text_section_pointers, 0x80);
    banner = g_helpmenu_active_banner;
    unit_data = g_helpmenu_active_unit_data;
    billboard = &g_helpmenu_billboard;
    cursor_tile = g_helpmenu_menu_cursor_tile;
    g_battle_text_section_pointers[1] = g_helpmenu_text_data + g_helpmenu_text_section_offsets[1];
    g_battle_text_section_pointers[2] = g_helpmenu_text_data + g_helpmenu_text_section_offsets[2];
    g_battle_text_section_pointers[11] = g_helpmenu_text_data + g_helpmenu_text_section_offsets[11];
    g_battle_text_section_pointers[12] = g_helpmenu_text_data + g_helpmenu_text_section_offsets[12];
    g_battle_text_section_pointers[13] = g_helpmenu_text_data + g_helpmenu_text_section_offsets[13];
    g_battle_text_section_pointers[15] = g_helpmenu_text_data + g_helpmenu_text_section_offsets[15];
    g_battle_text_section_pointers[19] = g_helpmenu_text_data + g_helpmenu_text_section_offsets[19];
    g_battle_text_section_pointers[20] = g_helpmenu_text_data + g_helpmenu_text_section_offsets[20];
    self_thread = &g_battle_threads[g_battle_current_thread_id];
    battle_menu_copy_billboard_data_out(banner, unit_data, billboard, cursor_tile);
    if ((g_helpmenu_selected_unit_panel_mode != 0) || (g_battle_post_battle_unit_changes_active != 0)) {
        g_helpmenu_require_navigation[2].destination[2] = 7;
        g_helpmenu_require_navigation[7].destination[3] = 2;
    }
    text_thread_id = helpmenu_thread_find_running_from_4();
    g_helpmenu_controller_state_ptr = battle_script_get_controller_input_pointer(0);
    menu_index = 0;
    for (poly_index = 0; poly_index < 2; poly_index++) {
        battle_gfx_init_default_poly_ft4(&cursor_polys[poly_index]);
        SetSemiTrans(&cursor_polys[poly_index], 0);
        cursor_polys[poly_index].tpage = GetTPage(0, 0, 0x3C0, 0x100);
        cursor_polys[poly_index].clut = 0x7D7C;
        battle_gfx_init_default_poly_ft4(&shadow_polys[poly_index]);
        SetSemiTrans(&shadow_polys[poly_index], 1);
        shadow_polys[poly_index].tpage = GetTPage(0, 2, 0x3C0, 0x100);
        shadow_polys[poly_index].clut = 0x7DBC;
    }
    if (text_thread_id < 9) {
        frame = text_thread_id;
        missing_value = -1;
        scan_thread = &g_battle_threads[frame];
        for (; frame < 9; frame++) {
            request = (help_request_t*)scan_thread->function_parameter_1;
            if (request->kind != missing_value) {
                break;
            }
            scan_thread++;
        }
        if (frame != 9) {
            if ((s32)g_battle_help_text_id_tables_ptr == missing_value) {
                g_battle_help_text_id_tables_ptr = (s32)g_helpmenu_menu_text_id_tables;
            }
            request_kind = request->kind;
            if (request_kind != 0x10) {
                if (request_kind == 0x100) {
                    help_id = *request->value;
                    help_id &= 0x7FF;
                    help_id |= 0x7800;
                    g_battle_sound_pending_sfx_id = MAIN_SFX_WINDOW_OPEN;
                    battle_thread_start(text_thread_id - 1, battle_text_character_handling_thread);
                    battle_thread_set_parameters(text_thread_id - 1, 0x3B, help_id, 0);
                    *g_helpmenu_controller_state_ptr = 0;
                    battle_thread_wait_frames(1);
                    helpmenu_menu_close();
                } else if (request_kind == 4) {
                    help_id = g_battle_action_menu_skillsets[request->selected_index];
                    if (help_id == 0xBC) {
                        *g_helpmenu_controller_state_ptr = 0;
                        battle_thread_wait_frames(1);
                        helpmenu_menu_close();
                    }
                    help_id |= 0x9800;
                    g_battle_sound_pending_sfx_id = MAIN_SFX_WINDOW_OPEN;
                    battle_thread_start(text_thread_id - 1, battle_text_character_handling_thread);
                    battle_thread_set_parameters(text_thread_id - 1, 0x3B, help_id, 0);
                } else if (request_kind == 0) {
                    *g_helpmenu_controller_state_ptr = 0;
                    battle_thread_wait_frames(1);
                    helpmenu_menu_close();
                } else if (request_kind == 5) {
                    help_id = request->table->primary_ids[request->selected_index];
                    help_id &= 0x7FF;
                    help_id |= 0x7800;
                    g_battle_sound_pending_sfx_id = MAIN_SFX_WINDOW_OPEN;
                    battle_thread_start(text_thread_id - 1, battle_text_character_handling_thread);
                    battle_thread_set_parameters(text_thread_id - 1, 0x3B, help_id, 0);
                } else if (request_kind == 0x11) {
                    help_id = request->table->primary_ids[request->selected_index];
                    help_id &= 0x7FF;
                    if (help_id == 0) {
                        *g_helpmenu_controller_state_ptr = 0;
                        battle_thread_wait_frames(1);
                        helpmenu_menu_close();
                    }
                    help_id |= 0x6800;
                    g_battle_sound_pending_sfx_id = MAIN_SFX_WINDOW_OPEN;
                    battle_thread_start(text_thread_id - 1, battle_text_character_handling_thread);
                    battle_thread_set_parameters(text_thread_id - 1, 0x3B, help_id, 0);
                } else if (request_kind == 0x17) {
                    help_id = request->table->primary_ids[request->selected_index];
                    help_id &= 0x7FF;
                    if (help_id == 0) {
                        *g_helpmenu_controller_state_ptr = 0;
                        battle_thread_wait_frames(1);
                        helpmenu_menu_close();
                    }
                    help_id += 0x26;
                    help_id |= 0x7800;
                    g_battle_sound_pending_sfx_id = MAIN_SFX_WINDOW_OPEN;
                    battle_thread_start(text_thread_id - 1, battle_text_character_handling_thread);
                    battle_thread_set_parameters(text_thread_id - 1, 0x3B, help_id, 0);
                } else if (request_kind == 0x12) {
                    help_id = *request->table->mode_flag;
                    if (help_id == 0) {
                        help_id = request->table->primary_ids[request->selected_index];
                    } else {
                        help_id = request->table->secondary_ids[request->selected_index];
                    }
                    help_id &= 0x7FF;
                    if (help_id == 0) {
                        *g_helpmenu_controller_state_ptr = 0;
                        battle_thread_wait_frames(1);
                        helpmenu_menu_close();
                    }
                    help_id |= 0xA000;
                    g_battle_sound_pending_sfx_id = MAIN_SFX_WINDOW_OPEN;
                    battle_thread_start(text_thread_id - 1, battle_text_character_handling_thread);
                    battle_thread_set_parameters(text_thread_id - 1, 0x3B, help_id, 0);
                } else if (request_kind == 0x13) {
                    help_id = request->table->secondary_ids[request->selected_index];
                    text_base = help_id & 0xF800;
                    if (text_base == TEXT_ID_ABILITY_NAME_BASE) {
                        help_id &= 0x7FF;
                        help_id |= 0x7800;
                    } else if (text_base == 0xB000) {
                        help_id &= 0x7FF;
                        help_id |= 0x9800;
                    } else if (text_base == TEXT_ID_ITEM_NAME_BASE) {
                        help_id &= 0x7FF;
                        if ((u32)(help_id - 0x26) < 0xAU) {
                            help_id += 0x26;
                            help_id |= 0x7800;
                        } else {
                            help_id |= 0x6800;
                        }
                    } else {
                        help_id = 0x807;
                    }
                    g_battle_sound_pending_sfx_id = MAIN_SFX_WINDOW_OPEN;
                    battle_thread_start(text_thread_id - 1, battle_text_character_handling_thread);
                    battle_thread_set_parameters(text_thread_id - 1, 0x3B, help_id, 0);
                } else {
                    g_battle_sound_pending_sfx_id = MAIN_SFX_WINDOW_OPEN;
                    g_helpmenu_current_text_ids = (u16*)((s32*)g_battle_help_text_id_tables_ptr)[request_kind];
                    battle_thread_start(text_thread_id - 1, battle_text_character_handling_thread);
                    battle_thread_set_parameters(
                        text_thread_id - 1, 0x3B, g_helpmenu_current_text_ids[request->selected_index], 0);
                }
                /* Wait for the text thread in the preceding slot. `offset + (s32)base`
                   keeps the addu operand order; pointer + offset swaps it. */
                wait_slot_offset = text_thread_id << 0xA;
                do {
                    battle_thread_wait_frames(1);
                } while (((native_thread_t*)(wait_slot_offset + (s32)g_battle_threads))[-1].is_running != 0);
                *g_helpmenu_controller_state_ptr = 0;
                battle_thread_wait_frames(1);
                helpmenu_menu_close();
            }
        } else {
            *g_helpmenu_controller_state_ptr = 0;
            battle_thread_wait_frames(1);
            helpmenu_menu_close();
        }
    }
    if (text_thread_id < 9) {
        navigation = g_helpmenu_require_navigation;
        text_thread_id -= 1;
    } else {
        help_mode = g_battle_unit_view_mode;
        if (help_mode == 1 || help_mode == 2) {
            g_battle_sound_pending_sfx_id = MAIN_SFX_WINDOW_OPEN;
            battle_thread_start(2, battle_text_character_handling_thread);
            battle_thread_set_parameters(2, 0x3B, g_helpmenu_menu_cursor_tile[0] + 0x5800, 0);
            g_battle_threads[2].task_words[3] = -0x20;
            battle_thread_wait_until_inactive(2);
            *g_helpmenu_controller_state_ptr = 0;
            battle_thread_wait_frames(1);
            helpmenu_menu_close();
        } else if (help_mode == 2) {
            /* Unreachable: the arm above already takes modes 1 and 2. */
            navigation = g_helpmenu_view_mode_2_navigation;
            text_thread_id = 2;
        } else if (help_mode == 3) {
            navigation = g_helpmenu_view_mode_3_navigation;
            text_thread_id = 2;
        } else if (help_mode == 4) {
            navigation = g_helpmenu_view_mode_4_navigation;
            text_thread_id = 2;
        }
    }
    frame = 0;
    text_thread = &g_battle_threads[text_thread_id];
    while (1) {
        if (text_thread->is_running == 0) {
            if (battle_thread_find_running_by_task(NATIVE_THREAD_TASK_HELP_MENU) != 0) {
                g_battle_menu_input_lock_mode = 0;
            }
            if (self_thread->task_words[1] != 0) {
                self_thread->task_words[1] = 0;
                self_thread->task_words[0] = 0;
            }
            node = &navigation[menu_index];
            y = node->y;
            x = node->x;
            if (battle_thread_find_running_by_task(NATIVE_THREAD_TASK_DEBUG_UNIT_EDITOR) != 0) {
                y += 8;
            }
            if (battle_thread_find_running_by_task(NATIVE_THREAD_TASK_HELP_MENU) != 0) {
                y += 8;
            }
            input = *g_helpmenu_controller_state_ptr;
            if (input & PSX_PAD_CROSS) {
                g_battle_sound_pending_sfx_id = MAIN_SFX_CANCEL;
                break;
            }
            if (input & PSX_PAD_CIRCLE) {
                if (node->value != 0) {
                    help_id = node->help_base + *node->value;
                    if (node->help_base == -0x6800 && g_helpmenu_billboard.generic_monster != 0) {
                        help_id = *node->value | 0x7800;
                        if ((*node->value & 0x7FF) == 0x7FF || (*node->value & 0x7FF) == 0) {
                            *g_helpmenu_controller_state_ptr = 0;
                        } else {
                            g_battle_sound_pending_sfx_id = MAIN_SFX_WINDOW_OPEN;
                            battle_thread_start(text_thread_id, battle_text_character_handling_thread);
                            battle_thread_set_parameters(text_thread_id, 0x3B, help_id, 0);
                        }
                    } else if (node->help_base == 0x7800 && (u32)((u16)*node->value - 1) >= 0x301) {
                        *g_helpmenu_controller_state_ptr = 0;
                    } else if (node->help_base == 0x6800 && (u32)((u16)*node->value - 1) >= 0x101) {
                        *g_helpmenu_controller_state_ptr = 0;
                    } else if (node->help_base == -0x6800 && (u32)((u16)*node->value - 1) >= 0x101) {
                        *g_helpmenu_controller_state_ptr = 0;
                    } else {
                        g_battle_sound_pending_sfx_id = MAIN_SFX_WINDOW_OPEN;
                        battle_thread_start(text_thread_id, battle_text_character_handling_thread);
                        battle_thread_set_parameters(text_thread_id, 0x3B, help_id, 0);
                    }
                } else {
                    help_id = node->help_base;
                    g_battle_sound_pending_sfx_id = MAIN_SFX_WINDOW_OPEN;
                    battle_thread_start(text_thread_id, battle_text_character_handling_thread);
                    battle_thread_set_parameters(text_thread_id, 0x3B, help_id, 0);
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
                    g_battle_sound_pending_sfx_id = MAIN_SFX_CURSOR_MOVE;
                }
                menu_index = node->destination[help_id];
            }
        } else {
            if (battle_thread_find_running_by_task(NATIVE_THREAD_TASK_HELP_MENU) != 0) {
                g_battle_menu_input_lock_mode = 1;
            }
            if (self_thread->task_words[1] == 0) {
                self_thread->task_words[1] = 2;
                self_thread->task_words[0] = 0;
            }
        }
        if (self_thread->task_words[1] != 0) {
            bob = battle_menu_get_cursor_bob_offset(1);
        } else {
            bob = battle_menu_get_cursor_bob_offset(0);
        }
        self_thread->task_words[0] += g_battle_event_speed;
        bob -= 4;
        buffer = frame & 1;
        if (node->vertical_cursor != 0) {
            u = 0xA8;
            cursor_polys[buffer].u0 = u;
            cursor_polys[buffer].u1 = u;
            cursor_polys[buffer].u2 = u_mid;
            cursor_polys[buffer].u3 = u_mid;
            cursor_polys[buffer].v0 = 0;
            cursor_polys[buffer].v1 = 0x10;
            cursor_polys[buffer].v2 = 0;
            cursor_polys[buffer].v3 = 0x10;
            u = 0xB8;
            shadow_polys[buffer].u0 = u;
            shadow_polys[buffer].u1 = u;
            shadow_polys[buffer].u2 = 0xC8;
            shadow_polys[buffer].u3 = 0xC8;
            shadow_polys[buffer].v0 = 0;
            shadow_polys[buffer].v1 = 0x10;
            shadow_polys[buffer].v2 = 0;
            shadow_polys[buffer].v3 = 0x10;
            cursor_polys[buffer].x0 = x;
            cursor_polys[buffer].y0 = y + bob;
            cursor_polys[buffer].x1 = x + 0x10;
            cursor_polys[buffer].y1 = y + bob;
            cursor_polys[buffer].x2 = x;
            cursor_polys[buffer].y2 = y + bob + 0x10;
            cursor_polys[buffer].x3 = x + 0x10;
            cursor_polys[buffer].y3 = y + bob + 0x10;
            (shadow_polys + buffer)->x0 = x + 2;
            (shadow_polys + buffer)->y0 = y + bob + 2;
            (shadow_polys + buffer)->x1 = x + 0x12;
            (shadow_polys + buffer)->y1 = y + bob + 2;
            (shadow_polys + buffer)->x2 = x + 2;
            (shadow_polys + buffer)->y2 = y + bob + 0x12;
            (shadow_polys + buffer)->x3 = x + 0x12;
            (shadow_polys + buffer)->y3 = y + bob + 0x12;
        } else {
            u = 0xA8;
            (cursor_polys + buffer)->u0 = u;
            (cursor_polys + buffer)->v0 = 0;
            (cursor_polys + buffer)->u1 = u_mid;
            (cursor_polys + buffer)->v1 = 0;
            (cursor_polys + buffer)->u2 = 0xA8;
            (cursor_polys + buffer)->v2 = 0x10;
            (cursor_polys + buffer)->u3 = u_mid;
            (cursor_polys + buffer)->v3 = 0x10;
            u = 0xB8;
            (shadow_polys + buffer)->u0 = u;
            (shadow_polys + buffer)->v0 = 0;
            (shadow_polys + buffer)->u1 = 0xC8;
            (shadow_polys + buffer)->v1 = 0;
            (shadow_polys + buffer)->u2 = u_mid;
            (shadow_polys + buffer)->v2 = 0x10;
            (shadow_polys + buffer)->u3 = 0xC8;
            (shadow_polys + buffer)->v3 = 0x10;
            (cursor_polys + buffer)->x0 = x + bob;
            (cursor_polys + buffer)->y0 = y;
            (cursor_polys + buffer)->x1 = x + bob + 0x10;
            (cursor_polys + buffer)->y1 = y;
            (cursor_polys + buffer)->x2 = x + bob;
            (cursor_polys + buffer)->y2 = y + 0x10;
            (cursor_polys + buffer)->x3 = x + bob + 0x10;
            (cursor_polys + buffer)->y3 = y + 0x10;
            shadow_offset = bob + 2;
            (shadow_polys + buffer)->x0 = x + shadow_offset;
            (shadow_polys + buffer)->y0 = y + 2;
            (shadow_polys + buffer)->x1 = x + shadow_offset + 0x10;
            (shadow_polys + buffer)->y1 = y + 2;
            (shadow_polys + buffer)->x2 = x + shadow_offset;
            (shadow_polys + buffer)->y2 = y + 0x12;
            (shadow_polys + buffer)->x3 = x + shadow_offset + 0x10;
            (shadow_polys + buffer)->y3 = y + 0x12;
        }
        if (self_thread->task_words[1] != 0) {
            cursor_polys[buffer].clut = 0x7DFC;
            shadow_polys[buffer].clut = 0x7E3C;
        } else {
            cursor_polys[buffer].clut = 0x7D7C;
            shadow_polys[buffer].clut = 0x7DBC;
        }
        battle_gfx_draw_or_append_gpu_primitive(&cursor_polys[buffer]);
        battle_gfx_draw_or_append_gpu_primitive(&shadow_polys[buffer]);
        battle_thread_wait_frames(1);
        frame++;
    }
    *g_helpmenu_controller_state_ptr = 0;
    battle_thread_wait_frames(1);
    helpmenu_menu_close();
}
