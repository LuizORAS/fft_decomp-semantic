#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/libgte.h"
#include "psx/types.h"

/*
 * Thread task 0x39: the unit status banner (portrait, name and job, level,
 * exp, HP/MP/CT bars, Brave, Faith and zodiac). WORLD copy of
 * debugchr_render_unit_status_panel_thread.
 *
 * Slot 8 draws the selected unit's banner and any other slot the comparison
 * unit's; each has its own packets, text images and unit records. The thread's
 * first parameter is its menu_status_panel_display_thread_t. Both
 * packet sets are built once and copied for the second frame buffer, then
 * every frame the thread:
 *  - dims the banner while a task-0x3b status panel thread runs with a text
 *    task active, or the display record asks for it;
 *  - runs the slide-down or slide-up table once when the display flags ask
 *    for it, leaving only MENU_STATUS_PANEL_HIDDEN set at the end;
 *  - cross-fades the portrait when the shown unit changes
 *    (world_gfx_build_portrait_transition_primitives);
 *  - redraws the number and name images on the first frame and whenever
 *    display->work is set. While the preview-stats window is open the first
 *    number block is drawn by five separate calls.
 * The gauge bars have a fixed width here. On the unit-join screen the mode
 * row is always row 0 and the roster number is left blank. The thread runs
 * until its third parameter is set, then frees its six text images.
 */
void world_menu_unit_status_banner_thread(void) {
    RECT rects[6];
    s32 anim_state;
    s32 cur_unit;
    s32 prev_unit;

    u8* render_a;                                    /* sp60 */
    menu_status_panel_editor_packet_t* summary_base; /* sp68 */
    menu_number_entry_t* entries;                    /* sp70 */
    menu_unit_status_record_t* state;                /* sp78 */
    s32 portrait_arg;                                /* sp80 */
    const RECT* transition;                          /* sp88 */
    palette_t* portrait_palettes;                    /* sp90 */
    u8* render_b;                                    /* sp98 */
    u8* render_c;                                    /* spA0 */
    menu_status_panel_packet_t* status_base;         /* spA8 */
    menu_unit_status_identity_t* identity;           /* spB0 */
    menu_status_panel_display_thread_t* display;     /* spB8 */
    s32 frame;                                       /* spC0 */
    s32 shake;                                       /* spC8 */
    s32 dimmed;                                      /* spD0 */
    s32 suppress;                                    /* spD8 */

    s32 thread_id;
    native_thread_t* threads;
    s32 i;
    s32 draw_x;
    s32 draw_y;
    s16 cell;
    menu_status_panel_packet_t* status;
    menu_status_panel_editor_packet_t* summary;
    u32* input;
    SPRT* sprites;
    u8* render_a_upper;
    u8* render_b_second;
    u8* render_b_third;
    POLY_FT4* portrait_prim;

    suppress = 0;
    world_thread_set_current_task_id(NATIVE_THREAD_TASK_UNIT_STATUS_BANNER);
    input = world_input_get_menu_controller(0);
    thread_id = g_world_thread_current_id;
    threads = g_world_threads;
    g_world_input_frame_controller_input = input + 1;
    display = (menu_status_panel_display_thread_t*)threads[thread_id].function_parameter_1;
    if (thread_id == NATIVE_SCREEN_THREAD_SLOT_UNIT_STATUS) {
        status = g_world_selected_unit_status_packets;
        summary = g_world_selected_unit_summary_packets;
        render_a = g_world_selected_unit_stat_text_image;
        render_b = g_world_selected_unit_number_text_image;
        render_c = g_world_selected_unit_name_text_image;
        entries = g_world_selected_unit_number_entries;
        state = &g_world_selected_unit_stat_summary;
        identity = &g_world_selected_unit_identity;
        transition = &g_world_selected_unit_portrait_rect;
        status_base = status;
        summary_base = summary;
        portrait_palettes = g_world_selected_unit_portrait_palettes;
    } else {
        status = g_world_comparison_unit_status_packets;
        summary = g_world_comparison_unit_summary_packets;
        render_a = g_world_comparison_unit_stat_text_image;
        render_b = g_world_comparison_unit_number_text_image;
        render_c = g_world_comparison_unit_name_text_image;
        entries = g_world_comparison_unit_number_entries;
        state = &g_world_comparison_unit_stat_summary;
        identity = &g_world_comparison_unit_identity;
        transition = &g_world_comparison_unit_portrait_rect;
        status_base = status;
        summary_base = summary;
        portrait_palettes = g_world_comparison_unit_portrait_palettes;
    }
    shake = 0;

    world_menu_init_numeric_display_frame_primitives(&g_world_gfx_portrait_origin, &summary->numeric_frame);
    world_gfx_set_draw_mode_for_texture_page(&summary->draw_mode_a, MENU_TEXTURE_PAGE_ALLOCATED_IMAGES);
    world_gfx_set_draw_mode_for_texture_page(&summary->draw_mode_b, MENU_TEXTURE_PAGE_FRAME);
    world_menu_init_sprite_array(&summary->label_sprites[0], 7, 0x7CBC);
    world_menu_init_sprite_array(&summary->value_sprites[0], 4, 0x7CBC);
    for (frame = 0; frame < 4; frame++) {
        world_gfx_init_default_poly_ft4(&summary->portrait[frame]);
    }
    world_text_configure_sprite_vram(&rects[0], 0x38, 0x28, (texture_primitive_t*)&summary->value_sprites[0], 2);
    world_text_configure_sprite_vram(&rects[1], 0x60, 0x10, (texture_primitive_t*)&summary->value_sprites[1], 2);
    for (frame = 0; frame < 4; frame++) {
        world_gfx_init_image_loading((texture_primitive_t*)&summary->value_sprites[frame],
            g_world_editor_numeric_geometry, &g_world_gfx_portrait_origin,
            &g_world_unit_summary_value_sprite_params[frame]);
    }
    if (state->team_kind >= 4) {
        state->team_kind = 0;
    }
    if (g_world_unit_join_screen_active == 0) {
        world_script_copy_bytes(&g_world_unit_summary_label_sprite_params[6],
            g_world_unit_summary_mode_row_params + state->team_kind * 0xC, 0xC);
    } else {
        world_script_copy_bytes(
            &g_world_unit_summary_label_sprite_params[6], g_world_unit_summary_mode_row_params, 0xC);
    }
    for (frame = 0; frame < 7; frame++) {
        world_gfx_init_image_loading((texture_primitive_t*)&summary->label_sprites[frame],
            g_world_editor_numeric_geometry, &g_world_gfx_portrait_origin,
            &g_world_unit_summary_label_sprite_params[frame]);
    }
    world_script_copy_bytes(summary + 1, summary, sizeof(menu_status_panel_editor_packet_t));
    world_gfx_set_draw_mode_for_texture_page(&status->draw_mode_a, MENU_TEXTURE_PAGE_FRAME);
    world_gfx_set_draw_mode_for_texture_page(&status->draw_mode_b, MENU_TEXTURE_PAGE_ALLOCATED_IMAGES);
    world_menu_init_numeric_display_frame_primitives(&g_world_unit_status_panel_origin, &status->numeric_frame);
    sprites = &status->sprites[0];
    world_menu_init_sprite_array(sprites, 7, 0x7C3C);
    world_gfx_init_default_poly_ft4(&status->portrait);
    world_text_configure_sprite_vram(&rects[2], 0x58, 0x20, (texture_primitive_t*)sprites, 0);
    world_text_configure_sprite_vram(&rects[3], 0x10, 0xA, (texture_primitive_t*)&status->sprites[1], 0);
    world_text_configure_sprite_vram(&rects[4], 0x10, 0xA, (texture_primitive_t*)&status->sprites[2], 0);
    world_text_configure_sprite_vram(&rects[5], 0x10, 0xA, (texture_primitive_t*)&status->sprites[3], 0);
    for (frame = 0; frame < 7; frame++) {
        world_gfx_init_image_loading((texture_primitive_t*)&status->sprites[frame], g_world_editor_numeric_geometry,
            &g_world_unit_status_panel_origin, &g_world_unit_status_sprite_params[frame]);
    }
    world_gfx_init_image_loading((texture_primitive_t*)&status->portrait, g_world_editor_numeric_geometry,
        &g_world_unit_status_panel_origin, (const image_load_parameters_t*)g_world_unit_status_portrait_params);
    if (state->team_kind == 1) {
        status->portrait.clut = 0x7FFD;
    } else {
        status->portrait.clut = 0x7FBD;
    }
    status->portrait.tpage = GetTPage(0, 1, 0x3C0, 0x100);
    world_script_copy_bytes(status + 1, status, sizeof(menu_status_panel_packet_t));
    frame = 0;
    anim_state = 0;
    prev_unit = state->unit_index;
    cur_unit = state->unit_index;
    for (;;) {
        s32 parity = frame & 1;
        status = status_base + parity;
        summary = summary_base + parity;
        if (state->gauges[MENU_UNIT_STATUS_GAUGE_CT].value >= 0x65) {
            state->gauges[MENU_UNIT_STATUS_GAUGE_CT].value = 0x64;
        }
        if (g_world_unit_join_screen_active == 0) {
            world_script_copy_bytes(&g_world_unit_summary_label_sprite_params[6],
                g_world_unit_summary_mode_row_params + state->team_kind * 0xC, 0xC);
        } else {
            world_script_copy_bytes(
                &g_world_unit_summary_label_sprite_params[6], g_world_unit_summary_mode_row_params, 0xC);
        }
        world_gfx_init_image_loading((texture_primitive_t*)&summary->label_sprites[6], g_world_editor_numeric_geometry,
            &g_world_gfx_portrait_origin, &g_world_unit_summary_label_sprite_params[6]);
        if (state->team_kind == 1) {
            status->portrait.clut = 0x7FFD;
        } else {
            status->portrait.clut = 0x7FBD;
        }
        dimmed = 0;
        if (world_thread_find_running_by_task(NATIVE_THREAD_TASK_STATUS_PANEL) != 0) {
            dimmed = g_world_thread_task_active != 0;
        }
        if (display->dimmed != 0) {
            dimmed = 1;
        }
        for (i = 0; i < 4; i++) {
            summary->value_sprites[i].clut = g_world_unit_summary_value_cluts[i][dimmed];
        }
        for (i = 0; i < 7; i++) {
            summary->label_sprites[i].clut = g_world_unit_summary_label_cluts[i][dimmed];
        }
        for (i = 0; i < 7; i++) {
            if (dimmed != 0) {
                status->sprites[i].clut = 0x7D3C;
            } else {
                status->sprites[i].clut = 0x7C3C;
            }
        }
        if (dimmed != 0) {
            world_menu_init_primitive_colors_dimmed(&summary->numeric_frame);
            world_menu_init_primitive_colors_dimmed(&status->numeric_frame);
            if ((anim_state - 4) < 6U) {
                setRGB0(&summary->portrait[0], 0x20, 0x28, 0x38);
            } else {
                setRGB0(&summary->portrait[0], 0x40, 0x50, 0x70);
            }
            setRGB0(&summary->portrait[1], 0x20, 0x28, 0x38);
            setRGB0(&summary->portrait[2], 0x20, 0x28, 0x38);
            setRGB0(&summary->portrait[3], 0x20, 0x28, 0x38);
        } else {
            s32 shade;
            world_menu_init_primitive_colors_normal(&summary->numeric_frame);
            world_menu_init_primitive_colors_normal(&status->numeric_frame);
            if ((anim_state - 4) >= 6U) {
                setRGB0(&summary->portrait[0], 0x80, 0x80, 0x80);
            } else {
                setRGB0(&summary->portrait[0], 0x40, 0x40, 0x40);
            }
            shade = 0x40;
            setRGB0(&summary->portrait[1], shade, shade, shade);
            setRGB0(&summary->portrait[2], shade, shade, shade);
            setRGB0(&summary->portrait[3], shade, shade, shade);
        }
        if (dimmed != 0) {
            setRGB0(&status->portrait, 0x40, 0x40, 0x60);
        } else {
            setRGB0(&status->portrait, 0x80, 0x80, 0x80);
        }
        g_world_gfx_draw_area_y = (g_world_frame_arg != 0xF0) ? 0xF0 : 0;
        if ((display->flags & MENU_STATUS_PANEL_SLIDE_DOWN) && (shake == 0)) {
            shake = 1;
        }
        if (display->flags & MENU_STATUS_PANEL_SLIDE_UP) {
            if (shake == 0) {
                shake = 0xB;
            }
        }
        if (shake != 0) {
            if (shake < 0xA) {
                display->shake_y = g_world_panel_slide_down_y[shake];
                if (frame & 1) {
                    shake += 1;
                }
                if (shake >= 7) {
                    shake = 0;
                    display->flags = display->flags & MENU_STATUS_PANEL_HIDDEN;
                }
            } else if (shake >= 0xB) {
                display->shake_y = g_world_panel_slide_up_y[shake];
                if (frame & 1) {
                    shake += 1;
                }
                if (shake >= 0x11) {
                    shake = 0;
                    display->flags = display->flags & MENU_STATUS_PANEL_HIDDEN;
                }
            }
        }
        draw_x = display->x;
        draw_y = g_world_gfx_draw_area_y;
        status->draw_offset_a.position.x = draw_x - 0x80;
        status->draw_offset_a.position.y = (u16)display->shake_y + draw_y;
        SetDrawOffset(&status->draw_offset_a.packet, (u16*)&status->draw_offset_a.position);
        status->draw_offset_b.position.x = -0x80;
        status->draw_offset_b.position.y = g_world_gfx_draw_area_y;
        SetDrawOffset(&status->draw_offset_b.packet, (u16*)&status->draw_offset_b.position);
        draw_x = display->x;
        draw_y = g_world_gfx_draw_area_y;
        summary->draw_offset_a.position.x = draw_x - 0x80;
        summary->draw_offset_a.position.y = (u16)display->shake_y + draw_y;
        SetDrawOffset(&summary->draw_offset_a.packet, (u16*)&summary->draw_offset_a.position);
        summary->draw_offset_b.position.x = -0x80;
        summary->draw_offset_b.position.y = g_world_gfx_draw_area_y;
        SetDrawOffset(&summary->draw_offset_b.packet, (u16*)&summary->draw_offset_b.position);
        if (anim_state >= 0xA) {
            anim_state = 0;
            prev_unit = cur_unit;
            suppress = display->flags & MENU_STATUS_PANEL_HIDDEN;
        }
        if (display->work != 0) {
            if (anim_state == 0) {
                prev_unit = cur_unit;
                portrait_arg = display->work;
                cur_unit = state->unit_index;
            } else {
                anim_state = 2;
                prev_unit = cur_unit;
                portrait_arg = display->work;
                cur_unit = state->unit_index;
            }
        }
        if (cur_unit != prev_unit) {
            if (anim_state == 0) {
                anim_state = 2;
            }
        }
        world_gfx_build_portrait_transition_primitives(
            transition, &anim_state, &cur_unit, &prev_unit, portrait_palettes, &summary->portrait[0], portrait_arg);
        {
            CVECTOR* color;

            color = g_world_unit_panel_bar_colors;
            for (i = 0; i < 3; i++) {
                s32 x0;
                s32 y;

                SetPolyG4(&summary->bars[i]);
                x0 = g_world_gfx_portrait_origin.x + 0x2F;
                summary->bars[i].x0 = x0;
                y = i * 0xB + 0x18;
                summary->bars[i].y0 = g_world_gfx_portrait_origin.y + y;
                summary->bars[i].x1 = x0 + 0x20;
                summary->bars[i].y1 = g_world_gfx_portrait_origin.y + y;
                summary->bars[i].x2 = x0;
                summary->bars[i].y2 = g_world_gfx_portrait_origin.y + y + 3;
                summary->bars[i].x3 = x0 + 0x20;
                summary->bars[i].y3 = g_world_gfx_portrait_origin.y + y + 3;
                summary->bars[i].x2 -= 3;
                summary->bars[i].x3 -= 3;
                if (dimmed != 0) {
                    setRGB0(&summary->bars[i], color->r >> 1, color->g >> 1, color->b);
                    color++;
                    setRGB1(&summary->bars[i], color->r >> 1, color->g >> 1, color->b);
                    color++;
                    setRGB2(&summary->bars[i], color->r >> 1, color->g >> 1, color->b);
                    color++;
                    setRGB3(&summary->bars[i], color->r >> 1, color->g >> 1, color->b);
                    color++;
                } else {
                    setRGB0(&summary->bars[i], color->r, color->g, color->b);
                    color++;
                    setRGB1(&summary->bars[i], color->r, color->g, color->b);
                    color++;
                    setRGB2(&summary->bars[i], color->r, color->g, color->b);
                    color++;
                    setRGB3(&summary->bars[i], color->r, color->g, color->b);
                    color++;
                }
            }
        }
        status->sprites[6].u0 = (identity->zodiac % 7) * 0x18;
        cell = identity->zodiac;
        status->sprites[6].w = 0x18;
        status->sprites[6].h = 0x14;
        status->sprites[6].v0 = (cell / 7) * 0x14 + 0x2A;
        if ((frame == 0) || (display->work != 0)) {
            world_menu_copy_billboard_data_in(&g_world_selected_unit_stat_summary, &g_world_selected_unit_identity,
                &g_world_selected_unit_stat_detail);
            if (state->list_index < 0) {
                entries[6].format = 0xC00;
            } else {
                entries[6].format = 2;
            }
            world_clear_menu_render_buffer(render_a, 0x700);
            {
                g_world_menu_text_state.pen.stride = 0x38;
                if (g_world_preview_stats_window_active == 0) {
                    world_menu_draw_numeric_display_entries(render_a, entries, &g_world_menu_text_state.pen, 6);
                } else {
                    world_text_render_decimal_entry_list(render_a, &entries[10], &g_world_menu_text_state.pen, 1);
                    world_text_render_decimal_entry_list(render_a, &entries[12], &g_world_menu_text_state.pen, 1);
                    world_menu_draw_numeric_display_entries(render_a, &entries[11], &g_world_menu_text_state.pen, 1);
                    world_menu_draw_numeric_display_entries(render_a, &entries[13], &g_world_menu_text_state.pen, 1);
                    world_menu_draw_numeric_display_entries(render_a, &entries[4], &g_world_menu_text_state.pen, 2);
                }
            }
            LoadImage(&rects[0], (u32*)render_a);
            render_a_upper = render_a + 0x400;
            g_world_menu_text_state.pen.stride = 0x60;
            world_menu_draw_numeric_display_entries(render_a_upper, &entries[8], &g_world_menu_text_state.pen, 2);
            LoadImage(&rects[1], (u32*)render_a_upper);
            display->work = 0;
            world_clear_menu_render_buffer(render_c, 0x580);
            g_world_menu_text_state.pen.stride = 0x58;
            world_menu_set_text_origin(0, 0);
            world_menu_display_text_entry(
                identity->unit_index + TEXT_ID_UNIT_NAME_BASE, render_c, &g_world_menu_text_state.pen);
            world_menu_set_text_origin(0, 0x10);
            world_menu_display_text_entry(
                identity->job_id + TEXT_ID_JOB_NAME_BASE, render_c, &g_world_menu_text_state.pen);
            LoadImage(&rects[2], (u32*)render_c);
            world_clear_menu_render_buffer(render_b, 0xF0);
            g_world_menu_text_state.pen.stride = 0x10;
            world_menu_set_text_origin(0, 0);
            world_text_render_decimal_value_or_dashes(identity->brave, 0x202, render_b, &g_world_menu_text_state.pen);
            LoadImage(&rects[3], (u32*)render_b);
            world_menu_set_text_origin(0, 0);
            render_b_second = render_b + 0x50;
            world_text_render_decimal_value_or_dashes(
                identity->faith, 0x202, render_b_second, &g_world_menu_text_state.pen);
            LoadImage(&rects[4], (u32*)render_b_second);
            if (g_world_unit_join_screen_active == 0) {
                world_menu_set_text_origin(0, 0);
                render_b_third = render_b + 0xA0;
                world_text_render_decimal_value_or_dashes(
                    identity->roster_slot + 1, 0x4002, render_b_third, &g_world_menu_text_state.pen);
                LoadImage(&rects[5], (u32*)render_b_third);
            } else {
                LoadImage(&rects[5], (u32*)(render_b + 0xA0));
            }
        }
        if (!(display->flags & MENU_STATUS_PANEL_HIDDEN)) {
            world_gfx_draw_or_append_gpu_primitive(&summary->draw_offset_b);
            world_gfx_draw_or_append_gpu_primitive(&summary->value_sprites[0]);
            world_gfx_draw_or_append_gpu_primitive(&summary->value_sprites[1]);
            world_gfx_draw_or_append_gpu_primitive(&summary->draw_mode_a);
            for (i = 0; i < 3; i++) {
                world_gfx_draw_or_append_gpu_primitive(&summary->bars[i]);
            }
            world_gfx_draw_or_append_gpu_primitive(&summary->value_sprites[2]);
            world_gfx_draw_or_append_gpu_primitive(&summary->value_sprites[3]);
            for (i = 0; i < 7; i++) {
                world_gfx_draw_or_append_gpu_primitive(&summary->label_sprites[i]);
            }
            world_gfx_draw_or_append_gpu_primitive(&summary->draw_mode_b);
            if (suppress == 0) {
                if (anim_state >= 5) {
                    world_gfx_draw_or_append_gpu_primitive(&summary->portrait[2]);
                    world_gfx_draw_or_append_gpu_primitive(&summary->portrait[3]);
                    world_gfx_draw_or_append_gpu_primitive(&summary->portrait[1]);
                }
                world_gfx_draw_or_append_gpu_primitive(&summary->portrait[0]);
            }
            world_menu_submit_numeric_display_frame_primitives(&summary->numeric_frame);
            world_gfx_draw_or_append_gpu_primitive(&summary->draw_offset_a);
            world_gfx_draw_or_append_gpu_primitive(&status->draw_offset_b);
            portrait_prim = &status->portrait;
            SetSemiTrans(portrait_prim, 1);
            world_gfx_draw_or_append_gpu_primitive(portrait_prim);
            for (i = 3; i >= 0; i--) {
                world_gfx_draw_or_append_gpu_primitive(&status->sprites[i]);
            }
            world_gfx_draw_or_append_gpu_primitive(&status->draw_mode_b);
            for (i = 4; i < 7; i++) {
                world_gfx_draw_or_append_gpu_primitive(&status->sprites[i]);
            }
            world_gfx_draw_or_append_gpu_primitive(&status->draw_mode_a);
            world_menu_submit_numeric_display_frame_primitives(&status->numeric_frame);
            world_gfx_draw_or_append_gpu_primitive(&status->draw_offset_a);
        }
        world_thread_yield();
        if (world_thread_get_current_parameter_3() != 0) {
            break;
        }
        frame += 1;
    }
    world_thread_yield();
    world_gfx_free_tpage7_vram(&rects[0]);
    world_gfx_free_tpage7_vram(&rects[1]);
    world_gfx_free_tpage7_vram(&rects[2]);
    world_gfx_free_tpage7_vram(&rects[3]);
    world_gfx_free_tpage7_vram(&rects[4]);
    world_gfx_free_tpage7_vram(&rects[5]);
    world_thread_exit_current();
}
