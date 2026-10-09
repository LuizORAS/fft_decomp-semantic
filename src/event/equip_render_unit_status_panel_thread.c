#include "fft/event_equip.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* The packet, state and thread layouts are the shared fft/menu.h
 * records: the editor packets at g_equip_panel_selected_editor_packets / g_equip_panel_comparison_editor_packets, the
 * status-panel packets at g_equip_panel_selected_packets / g_equip_panel_comparison_packets, the editor states
 * g_equip_selected_unit_stat_summary / g_equip_panel_comparison_billboard, and the per-thread display record.
 * g_equip_panel_gauge_bar_colors holds the four Gouraud vertex colours of each bar. */

struct menu_number_position;

/*
 * EQUIP twin of debugchr_render_unit_status_panel_thread (DEBUGCHR
 * 0x801bf750): the same unit editor/status display thread, with thread slot
 * 0xD selecting the first packet set. When g_equip_item_numeric_thread_enabled is set, the first
 * numeric block is drawn as five separate entries (two through
 * equip_text_render_decimal_entry_list), as in world_menu_unit_status_banner_thread; the
 * zodiac/g_debugchr_editor_active branch of the DEBUGCHR version is absent.
 */
void equip_render_unit_status_panel_thread(void) {
    RECT rects[6];
    s32 anim_state;
    s32 cur_unit;
    s32 prev_unit;

    u8* number_image;                                      /* sp60 */
    battle_menu_status_panel_editor_packet_t* editor_base; /* sp68 */
    menu_number_entry_t* numeric_entries;                  /* sp70 */
    battle_unit_status_record_t* state;                    /* sp78 */
    s32 portrait_arg;                                      /* sp80 */
    const RECT* portrait_rect;                             /* sp88 */
    u8* portrait_image;                                    /* sp90 */
    u8* small_text_image;                                  /* sp98 */
    u8* name_image;                                        /* spA0 */
    battle_menu_status_panel_packet_t* panel_base;         /* spA8 */
    s16* unit_data;                                        /* spB0 */
    battle_menu_status_panel_display_thread_t* thread;     /* spB8 */
    s32 shake;                                             /* spC0 */
    s32 highlight;                                         /* spC8 */
    s32 suppress;                                          /* spD0 */

    s32 thread_id;
    u8* threads;
    s32 frame;
    s32 i;
    s32 panel_x;
    s32 panel_shake_y;
    s32 editor_x;
    s32 editor_shake_y;
    s32* window_width;
    s32 panel_offset_y;
    s32 panel_base_offset_y;
    s32 editor_offset_y;
    s32 editor_base_offset_y;
    s16 icon_index;
    s32* text_position;
    battle_menu_status_panel_packet_t* panel;
    battle_menu_status_panel_editor_packet_t* editor;
    u8* input_state;
    SPRT* name_sprite;
    u8* number_image_lower;
    u8* small_text_image_2;
    u8* small_text_image_3;
    u8* portrait_poly;

    suppress = 0;
    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_UNIT_STATUS_BANNER);
    input_state = (u8*)battle_script_get_controller_input_pointer(0);
    thread_id = g_battle_current_thread_id;
    threads = (u8*)g_battle_threads;
    g_equip_input_controller = (u32*)(input_state + 4);
    /* Raw index arithmetic: g_battle_threads holds one pointer per 0x400-byte
       thread slot. The equivalent `threads + (thread_id << 10)` pointer form
       moves the shift and changes the prologue. */
    thread = *(battle_menu_status_panel_display_thread_t**)((thread_id * NATIVE_THREAD_STRIDE) + (u32)threads);
    if (thread_id == 0xD) {
        panel = g_equip_panel_selected_packets;
        editor = g_equip_panel_selected_editor_packets;
        number_image = g_equip_panel_selected_number_image;
        small_text_image = g_equip_panel_selected_large_number_image;
        name_image = g_equip_panel_selected_name_image;
        numeric_entries = g_equip_panel_selected_numeric_entries;
        /* Declared elsewhere in EQUIP as the 0x28-byte world_item_stat_summary_t;
         * this function reads it as the first of two 0x24-spaced editor states. */
        state = (battle_unit_status_record_t*)&g_equip_selected_unit_stat_summary;
        unit_data = (s16*)g_equip_panel_selected_unit_data;
        portrait_rect = &g_equip_panel_selected_portrait_rect;
        panel_base = panel;
        editor_base = editor;
        portrait_image = g_equip_panel_selected_portrait_image;
    } else {
        panel = g_equip_panel_comparison_packets;
        editor = g_equip_panel_comparison_editor_packets;
        number_image = g_equip_panel_comparison_number_image;
        small_text_image = g_equip_panel_comparison_large_number_image;
        name_image = g_equip_panel_comparison_name_image;
        numeric_entries = g_equip_panel_comparison_numeric_entries;
        state = &g_equip_panel_comparison_billboard;
        unit_data = g_equip_panel_comparison_unit_data;
        portrait_rect = &g_equip_panel_comparison_portrait_rect;
        panel_base = panel;
        editor_base = editor;
        portrait_image = g_equip_panel_comparison_portrait_image;
    }
    shake = 0;

    battle_menu_init_numeric_display_frame_primitives(&g_equip_gfx_portrait_origin.rect, &editor->numeric_frame);
    battle_gfx_set_draw_mode_for_texture_page(&editor->draw_mode_a, 1);
    battle_gfx_set_draw_mode_for_texture_page(&editor->draw_mode_b, 0);
    battle_menu_init_sprite_array(&editor->label_sprites[0], 7, 0x7CBC);
    battle_menu_init_sprite_array(&editor->value_sprites[0], 4, 0x7CBC);
    for (frame = 0; frame < 4; frame++) {
        battle_gfx_init_default_poly_ft4(&editor->portrait[frame]);
    }
    battle_text_configure_sprite_vram(&rects[0], 0x38, 0x28, &editor->value_sprites[0], 2);
    battle_text_configure_sprite_vram(&rects[1], 0x60, 0x10, &editor->value_sprites[1], 2);
    for (frame = 0; frame < 4; frame++) {
        battle_gfx_init_image_loading(&editor->value_sprites[frame], g_equip_editor_numeric_geometry,
            &g_equip_gfx_portrait_origin.location, &g_equip_panel_editor_value_cells[frame * 0xC]);
    }
    if (state->team_kind >= 4) {
        state->team_kind = 0;
    }
    if (g_battle_post_battle_unit_changes_active == 0) {
        battle_copy_bytes(
            g_equip_panel_editor_mode_cell, g_equip_panel_editor_mode_cells + state->team_kind * 0xC, 0xC);
    } else {
        battle_copy_bytes(g_equip_panel_editor_mode_cell, g_equip_panel_editor_mode_cells, 0xC);
    }
    for (frame = 0; frame < 7; frame++) {
        battle_gfx_init_image_loading(&editor->label_sprites[frame], g_equip_editor_numeric_geometry,
            &g_equip_gfx_portrait_origin.location, &g_equip_panel_editor_label_cells[frame * 0xC]);
    }
    battle_copy_bytes(editor + 1, editor, sizeof(battle_menu_status_panel_editor_packet_t));
    battle_gfx_set_draw_mode_for_texture_page(&panel->draw_mode_a, 0);
    battle_gfx_set_draw_mode_for_texture_page(&panel->draw_mode_b, 1);
    battle_menu_init_numeric_display_frame_primitives(&g_equip_panel_frame_rect.rect, &panel->numeric_frame);
    name_sprite = &panel->sprites[0];
    battle_menu_init_sprite_array(name_sprite, 7, 0x7C3C);
    battle_gfx_init_default_poly_ft4(&panel->portrait);
    battle_text_configure_sprite_vram(&rects[2], 0x58, 0x20, name_sprite, 0);
    battle_text_configure_sprite_vram(&rects[3], 0x10, 0xA, &panel->sprites[1], 0);
    battle_text_configure_sprite_vram(&rects[4], 0x10, 0xA, &panel->sprites[2], 0);
    battle_text_configure_sprite_vram(&rects[5], 0x10, 0xA, &panel->sprites[3], 0);
    for (frame = 0; frame < 7; frame++) {
        battle_gfx_init_image_loading(&panel->sprites[frame], g_equip_editor_numeric_geometry,
            &g_equip_panel_frame_rect.location, &g_equip_panel_sprite_cells[frame * 0xC]);
    }
    battle_gfx_init_image_loading(&panel->portrait, g_equip_editor_numeric_geometry, &g_equip_panel_frame_rect.location,
        g_equip_panel_portrait_cell);
    if (state->team_kind == 1) {
        panel->portrait.clut = 0x7FFD;
    } else {
        panel->portrait.clut = 0x7FBD;
    }
    panel->portrait.tpage = GetTPage(0, 1, 0x3C0, 0x100);
    battle_copy_bytes(panel + 1, panel, sizeof(battle_menu_status_panel_packet_t));
    frame = 0;
    anim_state = 0;
    prev_unit = state->unit_index;
    cur_unit = state->unit_index;
    for (;;) {
        s32 parity = frame & 1;
        panel = panel_base + parity;
        editor = editor_base + parity;
        if (state->gauges[2].value >= 0x65) {
            state->gauges[2].value = 0x64;
        }
        if (g_battle_post_battle_unit_changes_active == 0) {
            battle_copy_bytes(
                g_equip_panel_editor_mode_cell, g_equip_panel_editor_mode_cells + state->team_kind * 0xC, 0xC);
        } else {
            battle_copy_bytes(g_equip_panel_editor_mode_cell, g_equip_panel_editor_mode_cells, 0xC);
        }
        battle_gfx_init_image_loading(&editor->label_sprites[6], g_equip_editor_numeric_geometry,
            &g_equip_gfx_portrait_origin.location, g_equip_panel_editor_mode_cell);
        if (state->team_kind == 1) {
            panel->portrait.clut = 0x7FFD;
        } else {
            panel->portrait.clut = 0x7FBD;
        }
        highlight = 0;
        if (battle_thread_find_running_by_task(NATIVE_THREAD_TASK_STATUS_PANEL) != 0) {
            highlight = g_battle_menu_input_lock_mode != 0;
        }
        if (thread->highlight != 0) {
            highlight = 1;
        }
        for (i = 0; i < 4; i++) {
            editor->value_sprites[i].clut = g_equip_panel_editor_value_cluts[i][highlight];
        }
        for (i = 0; i < 7; i++) {
            editor->label_sprites[i].clut = g_equip_panel_editor_label_cluts[i][highlight];
        }
        i = 0;
        do {
            if (highlight != 0) {
                panel->sprites[i].clut = 0x7D3C;
            } else {
                panel->sprites[i].clut = 0x7C3C;
            }
            i += 1;
        } while (i < 7);
        if (highlight != 0) {
            battle_menu_init_primitive_colors_palette_bank_1(&editor->numeric_frame);
            battle_menu_init_primitive_colors_palette_bank_1(&panel->numeric_frame);
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
            battle_menu_init_primitive_colors_palette_bank_0(&editor->numeric_frame);
            battle_menu_init_primitive_colors_palette_bank_0(&panel->numeric_frame);
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
        if (highlight != 0) {
            panel->portrait.r0 = 0x40;
            panel->portrait.g0 = 0x40;
            panel->portrait.b0 = 0x60;
        } else {
            panel->portrait.r0 = 0x80;
            panel->portrait.g0 = 0x80;
            panel->portrait.b0 = 0x80;
        }
        g_equip_gfx_draw_offset_y = ((g_main_gfx_screen_polarity * 0xF0) != 0xF0) ? 0xF0 : 0;
        if ((thread->flags & 0x20) && (shake == 0)) {
            shake = 1;
        }
        if (thread->flags & 0x40) {
            if (shake == 0) {
                shake = 0xB;
            }
        }
        if (shake != 0) {
            if (shake < 0xA) {
                thread->shake_y.word = g_equip_panel_slide_down_y[shake];
                if (frame & 1) {
                    shake += 1;
                }
                if (shake >= 7) {
                    shake = 0;
                    thread->flags = thread->flags & 0x80;
                }
            } else if (shake >= 0xB) {
                thread->shake_y.word = g_equip_panel_slide_up_y[shake];
                if (frame & 1) {
                    shake += 1;
                }
                if (shake >= 0x11) {
                    shake = 0;
                    thread->flags = thread->flags & 0x80;
                }
            }
        }
        panel_x = thread->x;
        panel_offset_y = g_equip_gfx_draw_offset_y;
        panel->draw_offset_a.x = panel_x - 0x80;
        panel_shake_y = thread->shake_y.low;
        panel->draw_offset_a.y = panel_shake_y + panel_offset_y;
        SetDrawOffset(&panel->draw_offset_a, &panel->draw_offset_a.x);
        panel_base_offset_y = g_equip_gfx_draw_offset_y;
        panel->draw_offset_b.x = -0x80;
        panel->draw_offset_b.y = panel_base_offset_y;
        SetDrawOffset(&panel->draw_offset_b, &panel->draw_offset_b.x);
        editor_x = thread->x;
        editor_offset_y = g_equip_gfx_draw_offset_y;
        editor->draw_offset_a.x = editor_x - 0x80;
        editor_shake_y = thread->shake_y.low;
        editor->draw_offset_a.y = editor_shake_y + editor_offset_y;
        SetDrawOffset(&editor->draw_offset_a, &editor->draw_offset_a.x);
        editor_base_offset_y = g_equip_gfx_draw_offset_y;
        editor->draw_offset_b.x = -0x80;
        editor->draw_offset_b.y = editor_base_offset_y;
        SetDrawOffset(&editor->draw_offset_b, &editor->draw_offset_b.x);
        if (anim_state >= 0xA) {
            anim_state = 0;
            prev_unit = cur_unit;
            suppress = thread->flags & 0x80;
        }
        if (thread->work != 0) {
            if (anim_state == 0) {
                prev_unit = cur_unit;
                portrait_arg = thread->work;
                cur_unit = state->unit_index;
            } else {
                anim_state = 2;
                prev_unit = cur_unit;
                portrait_arg = thread->work;
                cur_unit = state->unit_index;
            }
        }
        if (cur_unit != prev_unit) {
            if (anim_state == 0) {
                anim_state = 2;
            }
        }
        equip_gfx_build_portrait_transition_primitives(
            portrait_rect, &anim_state, &cur_unit, &prev_unit, portrait_image, &editor->portrait[0], portrait_arg);
        {
            CVECTOR* color;
            battle_unit_status_gauge_t* bar;

            color = g_equip_panel_gauge_bar_colors;
            bar = state->gauges;
            for (i = 0; i < 3; i++) {
                s32 x0;
                s32 w;
                s32 y;

                SetPolyG4(&editor->bars[i]);
                x0 = g_equip_gfx_portrait_origin.coordinates[0] + 0x2F;
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
                editor->bars[i].y0 = g_equip_gfx_portrait_origin.coordinates[1] + y;
                editor->bars[i].x1 = x0 + w;
                editor->bars[i].y1 = g_equip_gfx_portrait_origin.coordinates[1] + y;
                editor->bars[i].x2 = x0;
                editor->bars[i].y2 = g_equip_gfx_portrait_origin.coordinates[1] + y + 3;
                editor->bars[i].x3 = x0 + w;
                editor->bars[i].y3 = g_equip_gfx_portrait_origin.coordinates[1] + y + 3;
                editor->bars[i].x2 -= 3;
                editor->bars[i].x3 -= 3;
                if (highlight != 0) {
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
        panel->sprites[6].u0 = (unit_data[4] % 7) * 0x18;
        icon_index = unit_data[4];
        panel->sprites[6].w = 0x18;
        panel->sprites[6].h = 0x14;
        panel->sprites[6].v0 = (icon_index / 7) * 0x14 + 0x2A;
        if ((frame == 0) || (thread->work != 0)) {
            if (state->list_index < 0) {
                numeric_entries[6].format = 0xC00;
            } else {
                numeric_entries[6].format = 2;
            }
            battle_clear_menu_render_buffer(number_image, 0x700);
            {
                s32* stride;
                s32* position;
                stride = &g_menu_text_state.stride;
                *stride = 0x38;
                if (g_equip_item_numeric_thread_enabled == 0) {
                    battle_menu_draw_numeric_display_entries(
                        (s32)number_image, numeric_entries, (struct menu_number_position*)(stride - 2), 6);
                } else {
                    position = stride - 2;
                    equip_text_render_decimal_entry_list(
                        (s32)number_image, &numeric_entries[10], (world_glyph_blit_t*)position, 1);
                    equip_text_render_decimal_entry_list(
                        (s32)number_image, &numeric_entries[12], (world_glyph_blit_t*)position, 1);
                    battle_menu_draw_numeric_display_entries(
                        (s32)number_image, &numeric_entries[11], (struct menu_number_position*)position, 1);
                    battle_menu_draw_numeric_display_entries(
                        (s32)number_image, &numeric_entries[13], (struct menu_number_position*)position, 1);
                    battle_menu_draw_numeric_display_entries(
                        (s32)number_image, &numeric_entries[4], (struct menu_number_position*)position, 2);
                }
            }
            LoadImage(&rects[0], number_image);
            window_width = &g_menu_text_state.stride;
            text_position = window_width - 2;
            number_image_lower = number_image + 0x400;
            *window_width = 0x60;
            battle_menu_draw_numeric_display_entries(
                (s32)number_image_lower, &numeric_entries[6], (struct menu_number_position*)text_position, 4);
            LoadImage(&rects[1], number_image_lower);
            thread->work = 0;
            battle_clear_menu_render_buffer(name_image, 0x580);
            *window_width = 0x58;
            battle_menu_set_text_origin(0, 0);
            battle_menu_display_text_entry(unit_data[0] + TEXT_ID_UNIT_NAME_BASE, name_image, text_position);
            battle_menu_set_text_origin(0, 0x10);
            battle_menu_display_text_entry(unit_data[1] + TEXT_ID_JOB_NAME_BASE, name_image, text_position);
            LoadImage(&rects[2], name_image);
            battle_clear_menu_render_buffer(small_text_image, 0xF0);
            *window_width = 0x10;
            battle_menu_set_text_origin(0, 0);
            battle_text_draw_large_number_glyphs(unit_data[2], 0x202, small_text_image, text_position);
            LoadImage(&rects[3], small_text_image);
            battle_menu_set_text_origin(0, 0);
            small_text_image_2 = small_text_image + 0x50;
            battle_text_draw_large_number_glyphs(unit_data[3], 0x202, small_text_image_2, text_position);
            LoadImage(&rects[4], small_text_image_2);
            if (g_battle_post_battle_unit_changes_active == 0) {
                battle_menu_set_text_origin(0, 0);
                small_text_image_3 = small_text_image + 0xA0;
                battle_text_draw_large_number_glyphs(unit_data[5] + 1, 0x4002, small_text_image_3, text_position);
                LoadImage(&rects[5], small_text_image_3);
            } else {
                LoadImage(&rects[5], small_text_image + 0xA0);
            }
        }
        if (!(thread->flags & 0x80)) {
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
            if (suppress == 0) {
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
            portrait_poly = (u8*)&panel->portrait;
            SetSemiTrans(portrait_poly, 1);
            battle_gfx_draw_or_append_gpu_primitive(portrait_poly);
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
