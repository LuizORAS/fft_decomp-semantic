/* Profile gcc-2.6.3_O2_aspsx-2.34_divcheck: the bar-width division at
 * 0x801c021c carries the full ASPSX checked-division expansion
 * (`bnez`/`break 0x7`, then the INT_MIN/-1 test and `break 0x6`) at
 * 0x801c0220-0x801c0240. Nothing else in the range needs the exception.
 *
 * The overlay draws two alternating copies of the editor and status panels;
 * `frame & 1` picks one, and the copy stride fixes each packet's size. The
 * packet, state and thread layouts are the shared fft/menu.h records,
 * proven by this function's own accesses and the primitive strides it walks.
 */
#include "fft/event_debugchr.h"
#include "psx/libgpu.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Thread task 0x39 (NATIVE_THREAD_TASK_UNIT_STATUS_BANNER): the unit status banner (portrait, name and
 * job, level, experience, HP, MP and CT bars, Brave, Faith and zodiac sign) of the selected unit or, in a
 * second thread, of the comparison unit; DEBUGCHR's copy, which scales each gauge
 * bar by its value/limit and can show the zodiac sign. world_menu_unit_status_banner_thread, WORLD's copy,
 * describes the frame loop. */
void debugchr_panel_unit_status_banner_thread(void) {
    RECT rects[6];
    s32 anim_state;
    s32 cur_unit;
    s32 prev_unit;

    u8* number_pixels;                                     /* sp60 */
    battle_menu_status_panel_editor_packet_t* editor_base; /* sp68 */
    menu_number_entry_t* number_entries;                   /* sp70 */
    battle_unit_status_record_t* state;                    /* sp78 */
    s32 portrait_request;                                  /* sp80 */
    const RECT* transition;                                /* sp88 */
    u16* portrait_image;                                   /* sp90 */
    u8* value_pixels;                                      /* sp98 */
    u8* name_pixels;                                       /* spA0 */
    battle_menu_status_panel_packet_t* panel_base;         /* spA8 */
    battle_active_unit_t* unit_info;                       /* spB0 */
    battle_menu_status_panel_display_thread_t* thread;     /* spB8 */
    s32 shake;                                             /* spC0 */
    s32 dimmed;                                            /* spC8 */
    s32 hide_portrait;                                     /* spD0 */

    s32 thread_id;
    s32 frame;
    s32 i;
    s32 panel_x;
    s32 panel_shake_y;
    s32 editor_x;
    s32 editor_shake_y;
    s32* window_width;
    s32 panel_y_a;
    s32 panel_y_b;
    s32 editor_y_a;
    s32 editor_y_b;
    s16 portrait_cell;
    world_glyph_blit_t* text_position;
    battle_menu_status_panel_packet_t* panel;
    battle_menu_status_panel_editor_packet_t* editor;
    SPRT* panel_sprites;
    u8* number_pixels_b;
    u8* value_pixels_b;
    u8* value_pixels_c;
    POLY_FT4* panel_portrait;

    hide_portrait = 0;
    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_UNIT_STATUS_BANNER);
    g_debugchr_input_controller = battle_script_get_controller_input_pointer(0) + 1;
    thread_id = g_battle_thread_current_id;
    thread = (battle_menu_status_panel_display_thread_t*)g_battle_threads[thread_id].function_parameter_1;
    if (thread_id == NATIVE_SCREEN_THREAD_SLOT_UNIT_STATUS) {
        panel = g_debugchr_panel_selected_packets;
        editor = g_debugchr_panel_selected_editor_packets;
        number_pixels = g_debugchr_panel_selected_number_image;
        value_pixels = g_debugchr_panel_selected_large_number_image;
        name_pixels = g_debugchr_panel_selected_name_image;
        number_entries = g_debugchr_panel_selected_numeric_entries;
        state = &g_debugchr_panel_selected_billboard;
        unit_info = &g_debugchr_panel_selected_unit_data;
        transition = &g_debugchr_panel_selected_portrait_rect;
        panel_base = panel;
        editor_base = editor;
        portrait_image = g_debugchr_panel_selected_portrait_image;
    } else {
        panel = g_debugchr_panel_comparison_packets;
        editor = g_debugchr_panel_comparison_editor_packets;
        number_pixels = g_debugchr_panel_comparison_number_image;
        value_pixels = g_debugchr_panel_comparison_large_number_image;
        name_pixels = g_debugchr_panel_comparison_name_image;
        number_entries = g_debugchr_panel_comparison_numeric_entries;
        state = &g_debugchr_panel_comparison_billboard;
        unit_info = &g_debugchr_panel_comparison_unit_data;
        transition = &g_debugchr_panel_comparison_portrait_rect;
        panel_base = panel;
        editor_base = editor;
        portrait_image = g_debugchr_panel_comparison_portrait_image;
    }
    shake = 0;

    battle_menu_init_numeric_display_frame_primitives(&g_debugchr_gfx_portrait_origin, &editor->numeric_frame);
    battle_gfx_set_draw_mode_for_texture_page(&editor->draw_mode_a, MENU_TEXTURE_PAGE_ALLOCATED_IMAGES);
    battle_gfx_set_draw_mode_for_texture_page(&editor->draw_mode_b, MENU_TEXTURE_PAGE_FRAME);
    battle_menu_init_sprite_array(&editor->label_sprites[0], 7, 0x7CBC);
    battle_menu_init_sprite_array(&editor->value_sprites[0], 4, 0x7CBC);
    for (frame = 0; frame < 4; frame++) {
        battle_gfx_init_default_poly_ft4(&editor->portrait[frame]);
    }
    battle_text_configure_sprite_vram(&rects[0], 0x38, 0x28, &editor->value_sprites[0], 2);
    battle_text_configure_sprite_vram(&rects[1], 0x60, 0x10, &editor->value_sprites[1], 2);
    for (frame = 0; frame < 4; frame++) {
        battle_gfx_init_image_loading(&editor->value_sprites[frame], g_debugchr_editor_numeric_geometry,
            &g_debugchr_gfx_portrait_origin, &g_debugchr_panel_editor_value_cells[frame]);
    }
    if (state->team_kind >= 4) {
        state->team_kind = 0;
    }
    if (g_battle_post_battle_unit_changes_active == 0) {
        battle_util_copy_bytes(
            g_debugchr_panel_editor_mode_cell, &g_debugchr_panel_editor_mode_cells[state->team_kind], 0xC);
    } else {
        battle_util_copy_bytes(g_debugchr_panel_editor_mode_cell, g_debugchr_panel_editor_mode_cells, 0xC);
    }
    for (frame = 0; frame < 7; frame++) {
        battle_gfx_init_image_loading(&editor->label_sprites[frame], g_debugchr_editor_numeric_geometry,
            &g_debugchr_gfx_portrait_origin, &g_debugchr_panel_editor_label_cells[frame]);
    }
    battle_util_copy_bytes(editor + 1, editor, sizeof(battle_menu_status_panel_editor_packet_t));
    battle_gfx_set_draw_mode_for_texture_page(&panel->draw_mode_a, MENU_TEXTURE_PAGE_FRAME);
    battle_gfx_set_draw_mode_for_texture_page(&panel->draw_mode_b, MENU_TEXTURE_PAGE_ALLOCATED_IMAGES);
    battle_menu_init_numeric_display_frame_primitives(&g_debugchr_panel_frame_rect, &panel->numeric_frame);
    panel_sprites = &panel->sprites[0];
    battle_menu_init_sprite_array(panel_sprites, 7, 0x7C3C);
    battle_gfx_init_default_poly_ft4(&panel->portrait);
    battle_text_configure_sprite_vram(&rects[2], 0x58, 0x20, panel_sprites, 0);
    battle_text_configure_sprite_vram(&rects[3], 0x10, 0xA, &panel->sprites[1], 0);
    battle_text_configure_sprite_vram(&rects[4], 0x10, 0xA, &panel->sprites[2], 0);
    battle_text_configure_sprite_vram(&rects[5], 0x10, 0xA, &panel->sprites[3], 0);
    for (frame = 0; frame < 7; frame++) {
        battle_gfx_init_image_loading(&panel->sprites[frame], g_debugchr_editor_numeric_geometry,
            &g_debugchr_panel_frame_rect, &g_debugchr_panel_sprite_cells[frame]);
    }
    battle_gfx_init_image_loading(&panel->portrait, g_debugchr_editor_numeric_geometry, &g_debugchr_panel_frame_rect,
        g_debugchr_panel_portrait_cell);
    if (state->team_kind == 1) {
        panel->portrait.clut = 0x7FFD;
    } else {
        panel->portrait.clut = 0x7FBD;
    }
    panel->portrait.tpage = GetTPage(0, 1, 0x3C0, 0x100);
    battle_util_copy_bytes(panel + 1, panel, sizeof(battle_menu_status_panel_packet_t));
    frame = 0;
    anim_state = 0;
    prev_unit = state->unit_index;
    cur_unit = state->unit_index;
    for (;;) {
        s32 parity = frame & 1;
        panel = panel_base + parity;
        editor = editor_base + parity;
        if (state->gauges[BATTLE_UNIT_STATUS_GAUGE_CT].value >= 0x65) {
            state->gauges[BATTLE_UNIT_STATUS_GAUGE_CT].value = 0x64;
        }
        if (g_battle_post_battle_unit_changes_active == 0) {
            battle_util_copy_bytes(
                g_debugchr_panel_editor_mode_cell, &g_debugchr_panel_editor_mode_cells[state->team_kind], 0xC);
        } else {
            battle_util_copy_bytes(g_debugchr_panel_editor_mode_cell, g_debugchr_panel_editor_mode_cells, 0xC);
        }
        battle_gfx_init_image_loading(&editor->label_sprites[6], g_debugchr_editor_numeric_geometry,
            &g_debugchr_gfx_portrait_origin, g_debugchr_panel_editor_mode_cell);
        if (state->team_kind == 1) {
            panel->portrait.clut = 0x7FFD;
        } else {
            panel->portrait.clut = 0x7FBD;
        }
        dimmed = 0;
        if (battle_thread_find_running_by_task(NATIVE_THREAD_TASK_STATUS_PANEL) != 0) {
            dimmed = g_battle_menu_input_lock_mode != 0;
        }
        if (thread->dimmed != 0) {
            dimmed = 1;
        }
        for (i = 0; i < 4; i++) {
            editor->value_sprites[i].clut = g_debugchr_panel_editor_value_cluts[i][dimmed];
        }
        for (i = 0; i < 7; i++) {
            editor->label_sprites[i].clut = g_debugchr_panel_editor_label_cluts[i][dimmed];
        }
        i = 0;
        do {
            if (dimmed != 0) {
                panel->sprites[i].clut = 0x7D3C;
            } else {
                panel->sprites[i].clut = 0x7C3C;
            }
            i += 1;
        } while (i < 7);
        if (dimmed != 0) {
            battle_menu_init_primitive_colors_dimmed(&editor->numeric_frame);
            battle_menu_init_primitive_colors_dimmed(&panel->numeric_frame);
            if ((anim_state - 4) < 6U) {
                editor->portrait[0].r0 = 0x20;
                editor->portrait[0].g0 = 0x28;
                editor->portrait[0].b0 = 0x38;
            } else {
                editor->portrait[0].r0 = 0x40;
                editor->portrait[0].g0 = 0x50;
                editor->portrait[0].b0 = 0x70;
            }
            editor->portrait[1].r0 = 0x20;
            editor->portrait[1].g0 = 0x28;
            editor->portrait[1].b0 = 0x38;
            editor->portrait[2].r0 = 0x20;
            editor->portrait[2].g0 = 0x28;
            editor->portrait[2].b0 = 0x38;
            editor->portrait[3].r0 = 0x20;
            editor->portrait[3].g0 = 0x28;
            editor->portrait[3].b0 = 0x38;
        } else {
            s32 shade;
            battle_menu_init_primitive_colors_normal(&editor->numeric_frame);
            battle_menu_init_primitive_colors_normal(&panel->numeric_frame);
            if ((anim_state - 4) >= 6U) {
                editor->portrait[0].r0 = 0x80;
                editor->portrait[0].g0 = 0x80;
                editor->portrait[0].b0 = 0x80;
            } else {
                editor->portrait[0].r0 = 0x40;
                editor->portrait[0].g0 = 0x40;
                editor->portrait[0].b0 = 0x40;
            }
            shade = 0x40;
            editor->portrait[1].r0 = shade;
            editor->portrait[1].g0 = shade;
            editor->portrait[1].b0 = shade;
            editor->portrait[2].r0 = shade;
            editor->portrait[2].g0 = shade;
            editor->portrait[2].b0 = shade;
            editor->portrait[3].r0 = shade;
            editor->portrait[3].g0 = shade;
            editor->portrait[3].b0 = shade;
        }
        if (dimmed != 0) {
            panel->portrait.r0 = 0x40;
            panel->portrait.g0 = 0x40;
            panel->portrait.b0 = 0x60;
        } else {
            panel->portrait.r0 = 0x80;
            panel->portrait.g0 = 0x80;
            panel->portrait.b0 = 0x80;
        }
        g_debugchr_gfx_draw_offset_y = ((g_main_gfx_screen_polarity * 0xF0) != 0xF0) ? 0xF0 : 0;
        if ((thread->flags & MENU_STATUS_PANEL_SLIDE_DOWN) && (shake == 0)) {
            shake = 1;
        }
        if (thread->flags & MENU_STATUS_PANEL_SLIDE_UP) {
            if (shake == 0) {
                shake = 0xB;
            }
        }
        if (shake != 0) {
            if (shake < 0xA) {
                thread->shake_y.word = g_debugchr_panel_slide_down_y[shake];
                if (frame & 1) {
                    shake += 1;
                }
                if (shake >= 7) {
                    shake = 0;
                    thread->flags = thread->flags & MENU_STATUS_PANEL_HIDDEN;
                }
            } else if (shake >= 0xB) {
                thread->shake_y.word = g_debugchr_panel_slide_up_y[shake];
                if (frame & 1) {
                    shake += 1;
                }
                if (shake >= 0x11) {
                    shake = 0;
                    thread->flags = thread->flags & MENU_STATUS_PANEL_HIDDEN;
                }
            }
        }
        panel_x = thread->x;
        panel_y_a = g_debugchr_gfx_draw_offset_y;
        panel->draw_offset_a.x = panel_x - 0x80;
        panel_shake_y = thread->shake_y.low;
        panel->draw_offset_a.y = panel_shake_y + panel_y_a;
        SetDrawOffset(&panel->draw_offset_a, &panel->draw_offset_a.x);
        panel_y_b = g_debugchr_gfx_draw_offset_y;
        panel->draw_offset_b.x = -0x80;
        panel->draw_offset_b.y = panel_y_b;
        SetDrawOffset(&panel->draw_offset_b, &panel->draw_offset_b.x);
        editor_x = thread->x;
        editor_y_a = g_debugchr_gfx_draw_offset_y;
        editor->draw_offset_a.x = editor_x - 0x80;
        editor_shake_y = thread->shake_y.low;
        editor->draw_offset_a.y = editor_shake_y + editor_y_a;
        SetDrawOffset(&editor->draw_offset_a, &editor->draw_offset_a.x);
        editor_y_b = g_debugchr_gfx_draw_offset_y;
        editor->draw_offset_b.x = -0x80;
        editor->draw_offset_b.y = editor_y_b;
        SetDrawOffset(&editor->draw_offset_b, &editor->draw_offset_b.x);
        if (anim_state >= 0xA) {
            anim_state = 0;
            prev_unit = cur_unit;
            hide_portrait = thread->flags & MENU_STATUS_PANEL_HIDDEN;
        }
        if (thread->work != 0) {
            if (anim_state == 0) {
                prev_unit = cur_unit;
                portrait_request = thread->work;
                cur_unit = state->unit_index;
            } else {
                anim_state = 2;
                prev_unit = cur_unit;
                portrait_request = thread->work;
                cur_unit = state->unit_index;
            }
        }
        if (cur_unit != prev_unit) {
            if (anim_state == 0) {
                anim_state = 2;
            }
        }
        debugchr_gfx_build_portrait_transition_primitives(
            transition, &anim_state, &cur_unit, &prev_unit, portrait_image, &editor->portrait[0], portrait_request);
        {
            CVECTOR* color;
            battle_unit_status_gauge_t* bar;

            color = g_debugchr_panel_gauge_bar_colors;
            bar = state->gauges;
            for (i = 0; i < 3; i++) {
                s32 x0;
                s32 w;
                s32 y;

                SetPolyG4(&editor->bars[i]);
                x0 = g_debugchr_gfx_portrait_origin.x + 0x2F;
                if (bar->max == 0) {
                    w = 4;
                } else {
                    w = (bar->value << 5) / bar->max;
                }
                if (w == 0) {
                    x0 = 0;
                }
                editor->bars[i].x0 = x0;
                y = i * 0xB + 0x18;
                editor->bars[i].y0 = g_debugchr_gfx_portrait_origin.y + y;
                editor->bars[i].x1 = x0 + w;
                editor->bars[i].y1 = g_debugchr_gfx_portrait_origin.y + y;
                editor->bars[i].x2 = x0;
                editor->bars[i].y2 = g_debugchr_gfx_portrait_origin.y + y + 3;
                editor->bars[i].x3 = x0 + w;
                editor->bars[i].y3 = g_debugchr_gfx_portrait_origin.y + y + 3;
                editor->bars[i].x2 -= 3;
                editor->bars[i].x3 -= 3;
                if (dimmed != 0) {
                    editor->bars[i].r0 = color->r >> 1;
                    editor->bars[i].g0 = color->g >> 1;
                    editor->bars[i].b0 = color->b;
                    color++;
                    editor->bars[i].r1 = color->r >> 1;
                    editor->bars[i].g1 = color->g >> 1;
                    editor->bars[i].b1 = color->b;
                    color++;
                    editor->bars[i].r2 = color->r >> 1;
                    editor->bars[i].g2 = color->g >> 1;
                    editor->bars[i].b2 = color->b;
                    color++;
                    editor->bars[i].r3 = color->r >> 1;
                    editor->bars[i].g3 = color->g >> 1;
                    editor->bars[i].b3 = color->b;
                    color++;
                } else {
                    editor->bars[i].r0 = color->r;
                    editor->bars[i].g0 = color->g;
                    editor->bars[i].b0 = color->b;
                    color++;
                    editor->bars[i].r1 = color->r;
                    editor->bars[i].g1 = color->g;
                    editor->bars[i].b1 = color->b;
                    color++;
                    editor->bars[i].r2 = color->r;
                    editor->bars[i].g2 = color->g;
                    editor->bars[i].b2 = color->b;
                    color++;
                    editor->bars[i].r3 = color->r;
                    editor->bars[i].g3 = color->g;
                    editor->bars[i].b3 = color->b;
                    color++;
                }
                bar++;
            }
        }
        panel->sprites[6].u0 = (unit_info->zodiac % 7) * 0x18;
        portrait_cell = unit_info->zodiac;
        panel->sprites[6].w = 0x18;
        panel->sprites[6].h = 0x14;
        panel->sprites[6].v0 = (portrait_cell / 7) * 0x14 + 0x2A;
        if ((frame == 0) || (thread->work != 0)) {
            if (state->list_index < 0) {
                number_entries[6].format = 0xC00;
            } else {
                number_entries[6].format = 2;
            }
            battle_menu_clear_render_buffer(number_pixels, 0x700);
            window_width = &g_battle_menu_text_state.pen.stride;
            text_position = (world_glyph_blit_t*)(window_width - 2);
            *window_width = 0x38;
            battle_menu_draw_numeric_display_entries(number_pixels, number_entries, text_position, 6);
            LoadImage(&rects[0], number_pixels);
            number_pixels_b = number_pixels + 0x400;
            *window_width = 0x60;
            battle_menu_draw_numeric_display_entries(number_pixels_b, &number_entries[6], text_position, 4);
            LoadImage(&rects[1], number_pixels_b);
            thread->work = 0;
            battle_menu_clear_render_buffer(name_pixels, 0x580);
            *window_width = 0x58;
            battle_menu_set_text_origin(0, 0);
            battle_menu_display_text_entry(unit_info->name_id + TEXT_ID_UNIT_NAME_BASE, name_pixels, text_position);
            battle_menu_set_text_origin(0, 0x10);
            battle_menu_display_text_entry(unit_info->job_id + TEXT_ID_JOB_NAME_BASE, name_pixels, text_position);
            LoadImage(&rects[2], name_pixels);
            battle_menu_clear_render_buffer(value_pixels, 0xF0);
            *window_width = 0x10;
            battle_menu_set_text_origin(0, 0);
            battle_menu_draw_small_number_glyphs(unit_info->brave, 0x202, value_pixels, text_position);
            LoadImage(&rects[3], value_pixels);
            battle_menu_set_text_origin(0, 0);
            value_pixels_b = value_pixels + 0x50;
            battle_menu_draw_small_number_glyphs(unit_info->faith, 0x202, value_pixels_b, text_position);
            LoadImage(&rects[4], value_pixels_b);
            if (g_battle_post_battle_unit_changes_active == 0) {
                if (g_debugchr_editor_active != 0) {
                    battle_menu_set_text_origin(0, 0);
                    value_pixels_c = value_pixels + 0xA0;
                    battle_menu_draw_small_number_glyphs(
                        unit_info->battle_id + 1, 0x4002, value_pixels_c, text_position);
                    LoadImage(&rects[5], value_pixels_c);
                } else {
                    s32 zodiac;
                    battle_menu_set_text_origin(0, 0);
                    zodiac = unit_info->battle_id;
                    if (zodiac >= 0x10) {
                        battle_menu_draw_small_number_glyphs(zodiac - 0xF, 0x4002, value_pixels + 0xA0, text_position);
                    }
                    LoadImage(&rects[5], value_pixels + 0xA0);
                }
            } else {
                LoadImage(&rects[5], value_pixels + 0xA0);
            }
        }
        if (!(thread->flags & MENU_STATUS_PANEL_HIDDEN)) {
            battle_gfx_draw_or_append_gpu_primitive(&editor->draw_offset_b);
            battle_gfx_draw_or_append_gpu_primitive(&editor->value_sprites[0]);
            battle_gfx_draw_or_append_gpu_primitive(&editor->value_sprites[1]);
            battle_gfx_draw_or_append_gpu_primitive(&editor->draw_mode_a);
            for (i = 0; i < 3; i++) {
                battle_gfx_draw_or_append_gpu_primitive(&editor->bars[i]);
            }
            battle_gfx_draw_or_append_gpu_primitive(&editor->value_sprites[2]);
            battle_gfx_draw_or_append_gpu_primitive(&editor->value_sprites[3]);
            for (i = 0; i < 7; i++) {
                battle_gfx_draw_or_append_gpu_primitive(&editor->label_sprites[i]);
            }
            battle_gfx_draw_or_append_gpu_primitive(&editor->draw_mode_b);
            if (hide_portrait == 0) {
                if (anim_state >= 5) {
                    battle_gfx_draw_or_append_gpu_primitive(&editor->portrait[2]);
                    battle_gfx_draw_or_append_gpu_primitive(&editor->portrait[3]);
                    battle_gfx_draw_or_append_gpu_primitive(&editor->portrait[1]);
                }
                battle_gfx_draw_or_append_gpu_primitive(&editor->portrait[0]);
            }
            battle_menu_submit_numeric_display_frame_primitives(&editor->numeric_frame);
            battle_gfx_draw_or_append_gpu_primitive(&editor->draw_offset_a);
            battle_gfx_draw_or_append_gpu_primitive(&panel->draw_offset_b);
            panel_portrait = &panel->portrait;
            SetSemiTrans(panel_portrait, 1);
            battle_gfx_draw_or_append_gpu_primitive(panel_portrait);
            for (i = 3; i >= 0; i--) {
                battle_gfx_draw_or_append_gpu_primitive(&panel->sprites[i]);
            }
            battle_gfx_draw_or_append_gpu_primitive(&panel->draw_mode_b);
            for (i = 4; i < 7; i++) {
                battle_gfx_draw_or_append_gpu_primitive(&panel->sprites[i]);
            }
            battle_gfx_draw_or_append_gpu_primitive(&panel->draw_mode_a);
            battle_menu_submit_numeric_display_frame_primitives(&panel->numeric_frame);
            battle_gfx_draw_or_append_gpu_primitive(&panel->draw_offset_a);
        }
        battle_thread_yield();
        if (battle_thread_get_current_parameter_3() != 0) {
            break;
        }
        frame++;
    }
    battle_thread_yield();
    battle_gfx_free_tpage7_vram(&rects[0]);
    battle_gfx_free_tpage7_vram(&rects[1]);
    battle_gfx_free_tpage7_vram(&rects[2]);
    battle_gfx_free_tpage7_vram(&rects[3]);
    battle_gfx_free_tpage7_vram(&rects[4]);
    battle_gfx_free_tpage7_vram(&rects[5]);
    battle_thread_exit_current();
}
