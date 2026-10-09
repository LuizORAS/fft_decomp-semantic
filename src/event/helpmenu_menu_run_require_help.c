/* Run the help menu over the REQUIRE screen.
 *
 * The cursor and its shadow are two POLY_FT4 pairs, one primitive per display
 * buffer. Retail reaches the shadow pair through the cursor pair in places
 * (`quad[2]`, `quads[buffer + 2]`): the two arrays are adjacent on the stack.
 */
#include "fft/event_helpmenu.h"
#include "psx/libgpu.h"
#include "psx/pad.h"
#include "psx/types.h"

void battle_thread_start(s32 thread_id, void (*function)(void));

void helpmenu_menu_run_require_help(void) {
    POLY_FT4 cursor[2];
    POLY_FT4 shadow[2];
    native_thread_t* self;
    help_navigation_record_t* navigation;
    help_navigation_record_t* entry;
    POLY_FT4* poly;
    POLY_FT4* shadow_poly;
    s32 menu_index;
    native_thread_t* text_thread;
    s32 text_thread_id;
    s32 frame;
    s32 buffer;
    s32 x;
    s32 y;
    s32 bob;
    s32 input;
    /* Also carries the navigation direction. */
    s32 help_id;
    s32 u;

    for (frame = 0; frame < 2; frame++) {
        poly = &cursor[frame];
        battle_gfx_init_default_poly_ft4(poly);
        SetSemiTrans(poly, 0);
        poly->tpage = GetTPage(0, 0, 0x3C0, 0x100);
        poly->clut = 0x7D7C;
        shadow_poly = &shadow[frame];
        battle_gfx_init_default_poly_ft4(shadow_poly);
        SetSemiTrans(shadow_poly, 1);
        shadow_poly->tpage = GetTPage(0, 2, 0x3C0, 0x100);
        shadow_poly->clut = 0x7DBC;
    }
    battle_copy_bytes(g_helpmenu_text_saved_pointers, g_battle_text_section_pointers, 0x80);
    g_battle_text_section_pointers[TEXT_SECTION_HELP_UNIT_STATUS]
        = g_helpmenu_text_data + g_helpmenu_text_section_offsets[TEXT_SECTION_HELP_UNIT_STATUS];
    g_battle_text_section_pointers[TEXT_SECTION_HELP_MENU_COMMANDS]
        = g_helpmenu_text_data + g_helpmenu_text_section_offsets[TEXT_SECTION_HELP_MENU_COMMANDS];
    g_battle_text_section_pointers[TEXT_SECTION_HELP_TERRAIN]
        = g_helpmenu_text_data + g_helpmenu_text_section_offsets[TEXT_SECTION_HELP_TERRAIN];
    g_battle_text_section_pointers[TEXT_SECTION_HELP_JOBS]
        = g_helpmenu_text_data + g_helpmenu_text_section_offsets[TEXT_SECTION_HELP_JOBS];
    g_battle_text_section_pointers[TEXT_SECTION_HELP_ITEMS]
        = g_helpmenu_text_data + g_helpmenu_text_section_offsets[TEXT_SECTION_HELP_ITEMS];
    g_battle_text_section_pointers[TEXT_SECTION_HELP_ABILITIES]
        = g_helpmenu_text_data + g_helpmenu_text_section_offsets[TEXT_SECTION_HELP_ABILITIES];
    g_battle_text_section_pointers[TEXT_SECTION_HELP_SKILLSETS]
        = g_helpmenu_text_data + g_helpmenu_text_section_offsets[TEXT_SECTION_HELP_SKILLSETS];
    g_battle_text_section_pointers[TEXT_SECTION_HELP_STATUSES]
        = g_helpmenu_text_data + g_helpmenu_text_section_offsets[TEXT_SECTION_HELP_STATUSES];
    self = &g_battle_threads[g_battle_current_thread_id];
    battle_menu_copy_billboard_data_out(
        g_helpmenu_active_banner, g_helpmenu_active_unit_data, &g_helpmenu_billboard, g_helpmenu_menu_cursor_tile);
    if (g_helpmenu_selected_unit_panel_mode != 0 || g_battle_post_battle_unit_changes_active != 0) {
        g_helpmenu_require_navigation[2].destination[2] = 7;
        g_helpmenu_require_navigation[7].destination[3] = 2;
    }
    g_helpmenu_controller_state_ptr = (s32*)battle_script_get_controller_input_pointer(0);
    menu_index = 0;
    g_helpmenu_require_navigation[0].x = 0xC4;
    g_helpmenu_require_navigation[2].x = 0xE0;
    g_helpmenu_require_navigation[0].destination[3] = 0;
    g_helpmenu_require_navigation[3].destination[0] = 0;
    navigation = g_helpmenu_require_navigation;
    text_thread_id = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
    text_thread = &g_battle_threads[text_thread_id];
    for (frame = 0;; frame++) {
        if (text_thread->is_running == 0) {
            if (battle_thread_find_running_by_task(NATIVE_THREAD_TASK_HELP_MENU) != 0) {
                g_battle_menu_input_lock_mode = 0;
            }
            if (self->task_words[1] != 0) {
                self->task_words[1] = 0;
                self->task_words[0] = 0;
            }
            entry = &navigation[menu_index];
            y = entry->y;
            x = entry->x;
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
                if (entry->value != 0) {
                    help_id = entry->help_base + *entry->value;
                    if (entry->help_base == -0x6800 && g_helpmenu_billboard.generic_monster != 0) {
                        help_id = *entry->value | 0x7800;
                        if ((*entry->value & 0x7FF) == 0x7FF || (*entry->value & 0x7FF) == 0) {
                            *g_helpmenu_controller_state_ptr = 0;
                        } else {
                            g_battle_sound_pending_sfx_id = MAIN_SFX_WINDOW_OPEN;
                            battle_thread_start(text_thread_id, battle_text_character_handling_thread);
                            battle_thread_set_parameters(text_thread_id, 0x3B, help_id, 0);
                        }
                    } else if (entry->help_base == 0x7800 && (*(u16*)entry->value - 1) >= 0x301U) {
                        *g_helpmenu_controller_state_ptr = 0;
                    } else if (entry->help_base == 0x6800 && (*(u16*)entry->value - 1) >= 0x101U) {
                        *g_helpmenu_controller_state_ptr = 0;
                    } else if (entry->help_base == -0x6800 && (*(u16*)entry->value - 1) >= 0x101U) {
                        *g_helpmenu_controller_state_ptr = 0;
                    } else {
                        g_battle_sound_pending_sfx_id = MAIN_SFX_WINDOW_OPEN;
                        battle_thread_start(text_thread_id, battle_text_character_handling_thread);
                        battle_thread_set_parameters(text_thread_id, 0x3B, help_id, 0);
                    }
                } else {
                    help_id = entry->help_base;
                    g_battle_sound_pending_sfx_id = MAIN_SFX_WINDOW_OPEN;
                    battle_thread_start(text_thread_id, battle_text_character_handling_thread);
                    battle_thread_set_parameters(text_thread_id, 0x3B, help_id, 0);
                }
            } else if (input & PSX_PAD_DPAD_MASK) {
                if (input & PSX_PAD_UP) {
                    help_id = 0;
                } else if (input & PSX_PAD_DOWN) {
                    help_id = 1;
                } else if (input & PSX_PAD_RIGHT) {
                    help_id = 2;
                } else {
                    help_id = 3;
                }
                if (menu_index != entry->destination[help_id]) {
                    g_battle_sound_pending_sfx_id = MAIN_SFX_CURSOR_MOVE;
                }
                menu_index = entry->destination[help_id];
            }
        } else {
            /* The help text is up: entry, x and y keep the values of the last
               idle frame. */
            if (battle_thread_find_running_by_task(NATIVE_THREAD_TASK_HELP_MENU) != 0) {
                g_battle_menu_input_lock_mode = 1;
            }
            if (self->task_words[1] == 0) {
                self->task_words[1] = 2;
                self->task_words[0] = 0;
            }
        }
        if (self->task_words[1] != 0) {
            bob = battle_menu_get_cursor_bob_offset(1);
        } else {
            bob = battle_menu_get_cursor_bob_offset(0);
        }
        self->task_words[0] += g_battle_event_speed;
        bob -= 4;
        buffer = frame & 1;
        if (entry->vertical_cursor != 0) {
            POLY_FT4* quad;
            POLY_FT4* shadow_quad;

            /* The upright cursor is the same texture cell transposed. */
            u = 0xA8;
            quad = &cursor[buffer];
            quad->u0 = u;
            quad->u1 = u;
            quad->u2 = u + 16;
            quad->u3 = u + 16;
            quad->v0 = 0;
            quad->v1 = 16;
            quad->v2 = 0;
            quad->v3 = 16;
            u += 16;
            shadow_quad = &shadow[buffer];
            shadow_quad->u0 = u;
            shadow_quad->u1 = u;
            shadow_quad->u2 = u + 16;
            shadow_quad->u3 = u + 16;
            shadow_quad->v0 = 0;
            shadow_quad->v1 = 16;
            shadow_quad->v2 = 0;
            shadow_quad->v3 = 16;
            setXYWH(quad, x, y + bob, 16, 16);
            setXYWH(&quad[2], x + 2, y + bob + 2, 16, 16);
        } else {
            POLY_FT4* quads;
            s32 shadow_x;

            /* Retail loads each quad's third u from its own constant, not from `u`. */
            u = 0xA8;
            quads = cursor;
            setUV4(&quads[buffer], u, 0, u + 16, 0, 0xA8, 16, u + 16, 16);
            u += 16;
            setUV4(&quads[buffer + 2], u, 0, u + 16, 0, 0xB8, 16, u + 16, 16);
            setXYWH(&quads[buffer], x + bob, y, 16, 16);
            shadow_x = x + 2 + bob;
            setXYWH(&quads[buffer + 2], shadow_x, y + 2, 16, 16);
        }
        if (self->task_words[1] != 0) {
            cursor[buffer].clut = 0x7DFC;
            shadow[buffer].clut = 0x7E3C;
        } else {
            cursor[buffer].clut = 0x7D7C;
            shadow[buffer].clut = 0x7DBC;
        }
        battle_gfx_draw_or_append_gpu_primitive(&cursor[buffer]);
        battle_gfx_draw_or_append_gpu_primitive(&shadow[buffer]);
        battle_thread_wait_frames(1);
    }
    *g_helpmenu_controller_state_ptr = 0;
    battle_thread_wait_frames(1);
    helpmenu_menu_close();
}
