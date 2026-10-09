/* WORLD copy of the EVENT HELPMENU require-help menu
 * (helpmenu_menu_run_require_help), byte-identical apart from addresses.
 *
 * The cursor and its shadow are two POLY_FT4 pairs, one primitive per display
 * buffer. Retail reaches the shadow pair through the cursor pair in places
 * (`quad[2]`, `quads[buffer + 2]`): the two arrays are adjacent on the stack.
 */
#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Moves the bouncing help cursor over the require-help navigation table and
 * opens the confirmed entry's help text in a text thread until cancelled. */
void world_menu_run_require_help(void) {
    POLY_FT4 cursor[2];
    POLY_FT4 shadow[2];
    native_thread_t* self;
    world_help_navigation_entry_t* navigation;
    world_help_navigation_entry_t* entry;
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
        world_menu_init_quad(poly);
        SetSemiTrans(poly, 0);
        poly->tpage = GetTPage(0, 0, 0x3C0, 0x100);
        poly->clut = 0x7D7C;
        shadow_poly = &shadow[frame];
        world_menu_init_quad(shadow_poly);
        SetSemiTrans(shadow_poly, 1);
        shadow_poly->tpage = GetTPage(0, 2, 0x3C0, 0x100);
        shadow_poly->clut = 0x7DBC;
    }
    world_script_copy_bytes(
        g_world_text_section_pointer_backup, g_world_text_section_pointers, WORLD_TEXT_SECTION_TABLE_BYTES);
    g_world_text_section_pointers[1] = g_world_help_data_base + g_world_help_text_section_offsets[1];
    g_world_text_section_pointers[2] = g_world_help_data_base + g_world_help_text_section_offsets[2];
    g_world_text_section_pointers[11] = g_world_help_data_base + g_world_help_text_section_offsets[11];
    g_world_text_section_pointers[12] = g_world_help_data_base + g_world_help_text_section_offsets[12];
    g_world_text_section_pointers[13] = g_world_help_data_base + g_world_help_text_section_offsets[13];
    g_world_text_section_pointers[15] = g_world_help_data_base + g_world_help_text_section_offsets[15];
    g_world_text_section_pointers[19] = g_world_help_data_base + g_world_help_text_section_offsets[19];
    g_world_text_section_pointers[20] = g_world_help_data_base + g_world_help_text_section_offsets[20];
    self = &g_world_threads[g_world_thread_current_id];
    world_gfx_copy_screen_setup_out(g_world_help_active_banner, g_world_help_active_unit_data, &g_world_help_billboard,
        g_world_help_menu_cursor_tile);
    if (g_world_selected_unit_stat_summary.team_state != 0 || g_world_unit_join_screen_active != 0) {
        g_world_help_require_navigation[2].neighbors[2] = 7;
        g_world_help_require_navigation[7].neighbors[3] = 2;
    }
    g_world_help_controller_state_ptr = world_input_get_menu_controller(0);
    menu_index = 0;
    g_world_help_require_navigation[0].cursor_x = 0xC4;
    g_world_help_require_navigation[2].cursor_x = 0xE0;
    g_world_help_require_navigation[0].neighbors[3] = 0;
    g_world_help_require_navigation[3].neighbors[0] = 0;
    navigation = g_world_help_require_navigation;
    text_thread_id = world_thread_resolve_id(0x10);
    text_thread = &g_world_threads[text_thread_id];
    for (frame = 0;; frame++) {
        if (text_thread->is_running == 0) {
            if (world_thread_find_running_by_task(NATIVE_THREAD_TASK_HELP_MENU) != 0) {
                g_world_thread_task_active = 0;
            }
            if (self->task_words[1] != 0) {
                self->task_words[1] = 0;
                self->task_words[0] = 0;
            }
            entry = &navigation[menu_index];
            y = entry->cursor_y;
            x = entry->cursor_x;
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
                if (entry->value != 0) {
                    help_id = entry->value_base + *entry->value;
                    if (entry->value_base == -0x6800 && g_world_help_billboard.generic_monster != 0) {
                        help_id = *entry->value | 0x7800;
                        if ((*entry->value & 0x7FF) == 0x7FF || (*entry->value & 0x7FF) == 0) {
                            *g_world_help_controller_state_ptr = 0;
                        } else {
                            g_world_sound_effect_id_to_play = MAIN_SFX_WINDOW_OPEN;
                            world_thread_start(text_thread_id, world_text_character_handling_thread);
                            world_thread_set_parameters(text_thread_id, 0x3B, help_id, 0);
                        }
                    } else if (entry->value_base == 0x7800 && (u32)(*(u16*)entry->value - 1) >= 0x301U) {
                        *g_world_help_controller_state_ptr = 0;
                    } else if (entry->value_base == 0x6800 && (u32)(*(u16*)entry->value - 1) >= 0x101U) {
                        *g_world_help_controller_state_ptr = 0;
                    } else if (entry->value_base == -0x6800 && (u32)(*(u16*)entry->value - 1) >= 0x101U) {
                        *g_world_help_controller_state_ptr = 0;
                    } else {
                        g_world_sound_effect_id_to_play = MAIN_SFX_WINDOW_OPEN;
                        world_thread_start(text_thread_id, world_text_character_handling_thread);
                        world_thread_set_parameters(text_thread_id, 0x3B, help_id, 0);
                    }
                } else {
                    help_id = entry->value_base;
                    g_world_sound_effect_id_to_play = MAIN_SFX_WINDOW_OPEN;
                    world_thread_start(text_thread_id, world_text_character_handling_thread);
                    world_thread_set_parameters(text_thread_id, 0x3B, help_id, 0);
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
                if (menu_index != entry->neighbors[help_id]) {
                    g_world_sound_effect_id_to_play = MAIN_SFX_CURSOR_MOVE;
                }
                menu_index = entry->neighbors[help_id];
            }
        } else {
            /* The help text is up: entry, x and y keep the values of the last
               idle frame. */
            if (world_thread_find_running_by_task(NATIVE_THREAD_TASK_HELP_MENU) != 0) {
                g_world_thread_task_active = 1;
            }
            if (self->task_words[1] == 0) {
                self->task_words[1] = 2;
                self->task_words[0] = 0;
            }
        }
        if (self->task_words[1] != 0) {
            bob = world_lookup_thread_parameter_threshold_value(1);
        } else {
            bob = world_lookup_thread_parameter_threshold_value(0);
        }
        self->task_words[0] += g_world_event_speed;
        bob -= 4;
        buffer = frame & 1;
        if (entry->orientation != 0) {
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
        world_gfx_draw_or_append_gpu_primitive(&cursor[buffer]);
        world_gfx_draw_or_append_gpu_primitive(&shadow[buffer]);
        world_thread_wait_frames(1);
    }
    *g_world_help_controller_state_ptr = 0;
    world_thread_wait_frames(1);
    world_text_restore_sections_and_stop_thread();
}
