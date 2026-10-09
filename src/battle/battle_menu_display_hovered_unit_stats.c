#include "fft/battle.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* 0x2ec-byte pages in pairs at 0x8017225c; the first page depends on the mode. */
extern world_unit_status_page_t g_battle_unit_status_panel_pages[];
/* g_battle_menu_sprite_page_image_params[5..7], the unscaled templates of
 * elements 0..2; bound separately because the target addresses the two
 * arrays from different bases. */
extern world_gfx_image_load_parameters_t g_battle_menu_sprite_page_image_param_bases[];

/*
 * Unit status panel thread body (HP/MP/CT gauges, portrait, numbers and, in
 * mode 2, the reaction-ability name), the BATTLE twin of world_menu_display_hovered_unit_stats.
 * Builds two alternating 0x2ec-byte pages, slides the panel in by
 * `slide`/`speed` each event tick and out when the thread's third parameter
 * is set (mode 0 also leaves immediately while g_battle_thread_contexts[12].task_id is
 NATIVE_THREAD_TASK_ATTACK_CASTER_STATS). A running
 * task-0x15 thread dims the portrait, gauges and CLUTs.
 *
 * `i` doubles as the saved hide-numbers flag in the first-frame text pass, as
 * in the target's register use. Gauge corners are stored in setXYWH order
 * (x0, y0, x1, y1, ...); storing y0/y1 first lengthens the reloaded
 * `extent_w`'s lifetime and swaps its register with the gauge width.

 */
void battle_menu_display_hovered_unit_stats(
    RECT* frame_rect, s32 mode, struct menu_number_entry* entries, battle_unit_status_record_t* status) {
    RECT rects[4];
    world_unit_status_offsets_t offsets[2];
    u16 extent_w;
    u16 extent_h;
    u8* buffer;
    u8* text;
    s32 text_loaded;
    s32 running;
    s32 unit_id;
    s32 slide;
    s32 speed;
    s32 text_id;
    battle_stats_t* unit;
    world_unit_status_page_t* page;
    world_gfx_image_load_parameters_t* params;
    world_unit_status_offsets_t* offs;
    POLY_G4* poly;
    battle_unit_status_gauge_t* gauge;
    CVECTOR* color;
    s32* inner_width;
    s32 frame;
    s32 i;
    s32 hide_values;
    s32 base_y;
    s32 row_y;
    u16 width;

    unit_id = status->unit_index;
    speed = 0x16;
    unit = battle_unit_get_stats_from_battle_id(unit_id);
    slide = 0x84;
    g_battle_menu_hide_numeric_values = unit->unit_flags & UNIT_FLAG_EGG;
    if (battle_thread_get_current_task_id() == NATIVE_THREAD_TASK_HOVERED_UNIT_STATS) {
        g_battle_menu_hovered_stats_frame_count++;
    }
    if (mode == UNIT_STATS_PANEL_ATTACK_TARGET) {
        text_id = unit->reaction_ability;
        if (text_id >= ABILITY_ID_COUNT) {
            text_id = 0;
        }
        if (text_id != 0) {
            text = battle_text_find_entry(text_id + TEXT_ID_ABILITY_NAME_BASE);
            g_battle_thread_call_target = (void (*)(void))battle_text_measure_pixels;
            battle_thread_call_on_main_stack(&extent_w, &extent_h, text);
            if (extent_w & 3) {
                extent_w = (extent_w & 0xFFFC) + 4;
            }
            extent_w = 0x4C - extent_w;
            if (extent_w > 0x100) {
                extent_w = 0;
            }
            g_battle_menu_sprite_page_image_params[25].x_screen_offset
                = g_battle_menu_sprite_page_image_params[28].x_screen_offset + extent_w;
            g_battle_menu_sprite_page_image_params[26].x_screen_offset
                = g_battle_menu_sprite_page_image_params[29].x_screen_offset + extent_w;
            for (frame = 0; frame < 3; frame++) {
                g_battle_menu_sprite_page_image_params[frame].x_screen_offset
                    = g_battle_menu_sprite_page_image_param_bases[frame].x_screen_offset + extent_w;
                g_battle_menu_sprite_page_image_params[frame].width
                    = g_battle_menu_sprite_page_image_param_bases[frame].width - extent_w;
            }
            g_battle_menu_sprite_page_image_params[3].x_screen_offset
                = g_battle_menu_sprite_page_image_params[8].x_screen_offset + extent_w;
        }
    }
    page = &g_battle_unit_status_panel_pages[g_battle_unit_status_first_page_by_mode[mode]];
    battle_menu_init_numeric_display_frame_primitives(frame_rect, &page->palette);
    battle_gfx_set_draw_mode_for_texture_page(&page->draw_mode_1, MENU_TEXTURE_PAGE_ALLOCATED_IMAGES);
    battle_gfx_set_draw_mode_for_texture_page(&page->draw_mode_0, MENU_TEXTURE_PAGE_FRAME);
    battle_menu_init_sprite_array(page->sprites, 7, 0x7CBC);
    for (frame = 0; frame < 5; frame++) {
        battle_gfx_init_default_poly_ft4(&page->quads[frame]);
    }
    battle_gfx_init_default_poly_ft4(&page->portrait);
    battle_text_configure_sprite_vram(&rects[0], 0x28, 0x28, (SPRT*)&page->quads[0], 2);
    battle_text_configure_sprite_vram(&rects[1], 0x60, 0x10, (SPRT*)&page->quads[1], 2);
    params = g_battle_unit_status_quad_image_params_by_mode[mode];
    if (mode == UNIT_STATS_PANEL_ATTACK_CASTER) {
        battle_gfx_init_image_loading(&page->quads[4], &g_battle_menu_texture_location, frame_rect, &params[4]);
        for (frame = 0; frame < 2; frame++, params++) {
            battle_gfx_init_image_loading(&page->quads[frame], &g_battle_menu_texture_location, frame_rect, params);
            page->quads[frame].clut = 0x7CBC;
        }
        battle_menu_display_projected_action_effect(
            &page->quads[frame], &g_battle_menu_texture_location, frame_rect, params);
    } else if (mode == UNIT_STATS_PANEL_ATTACK_TARGET) {
        for (frame = 0; frame < 2; frame++, params++) {
            battle_gfx_init_image_loading(&page->quads[frame], &g_battle_menu_texture_location, frame_rect, params);
        }
        if (text_id != 0) {
            battle_gfx_init_image_loading(&page->quads[frame], &g_battle_menu_texture_location, frame_rect, params);
        }
    } else {
        for (frame = 0; frame < 4; frame++, params++) {
            battle_gfx_init_image_loading(&page->quads[frame], &g_battle_menu_texture_location, frame_rect, params);
        }
    }
    battle_copy_bytes(&page[1], page, sizeof(world_unit_status_page_t));
    text_loaded = 0;
    for (frame = 0;; frame++) {
        if (status->gauges[BATTLE_UNIT_STATUS_GAUGE_CT].value > 100) {
            status->gauges[BATTLE_UNIT_STATUS_GAUGE_CT].value = 100;
        }
        if (text_loaded != 0) {
            battle_menu_free_memory(buffer);
            text_loaded = 0;
        }
        for (i = 0; i < g_battle_event_speed; i++) {
            if (battle_thread_get_current_parameter_3() != 0) {
                if (mode == UNIT_STATS_PANEL_HOVERED_UNIT
                    && g_battle_thread_contexts[NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS].task_id
                        == NATIVE_THREAD_TASK_ATTACK_CASTER_STATS) {
                    /* Both exits leave the step loop and the frame loop. */
                    goto exit;
                }
                slide += 0x1E;
                if (slide > 0x80) {
                    goto exit;
                }
            } else {
                if (slide != 0) {
                    slide -= speed;
                    if (slide < 0) {
                        slide = 0;
                    }
                }
                if (speed >= 3) {
                    speed -= 2;
                }
            }
        }
        running = battle_thread_find_running_by_task(NATIVE_THREAD_TASK_UNIT_EDITOR_PANEL);
        if (running != 0 && battle_thread_is_running_8014cc94(running - 1) == 0) {
            running = 0;
        }
        page = &g_battle_unit_status_panel_pages[g_battle_unit_status_first_page_by_mode[mode] + (frame & 1)];
        battle_menu_build_unit_portrait_poly(&page->portrait, unit_id);
        if (running != 0) {
            page->portrait.r0 = 0x40;
            page->portrait.g0 = 0x50;
            page->portrait.b0 = 0x70;
        } else {
            page->portrait.r0 = 0x80;
            page->portrait.g0 = 0x80;
            page->portrait.b0 = 0x80;
        }
        status->team_kind = 0;
        if (unit->initial_team_flags & BATTLE_TEAM_MASK) {
            status->team_kind = 1;
        }
        if (!(unit->initial_team_flags & BATTLE_TEAM_OR_PLAYER_CONTROL_MASK)) {
            status->team_kind = 2;
        }
        if (unit->auto_battle_setting != BATTLE_AI_AUTOBATTLE_MANUAL) {
            status->team_kind = 3;
        }
        params = g_battle_unit_status_sprite_image_params_by_mode[mode];
        if (g_battle_unit_status_bounce_step == 0) {
            page->portrait.x0 = frame_rect->x + 2;
            page->portrait.y0 = frame_rect->y + 2;
            page->portrait.x1 = frame_rect->x + 0x21;
            page->portrait.y1 = frame_rect->y + 2;
            page->portrait.x2 = frame_rect->x + 2;
            page->portrait.y2 = frame_rect->y + 0x32;
            page->portrait.x3 = frame_rect->x + 0x21;
            page->portrait.y3 = frame_rect->y + 0x32;
            battle_copy_bytes(&params[6], &g_battle_unit_status_icon_image_params[status->team_kind], sizeof(*params));
            for (i = 0; i < 7; i++, params++) {
                battle_gfx_init_image_loading(&page->sprites[i], &g_battle_menu_texture_location, frame_rect, params);
            }
        }
        if (mode != UNIT_STATS_PANEL_ATTACK_CASTER) {
            for (i = 0; i < 4; i++) {
                page->quads[i].clut = g_battle_unit_status_quad_clut_pairs[i][running & 1];
            }
        }
        poly = page->bars;
        if (mode == UNIT_STATS_PANEL_ATTACK_TARGET) {
            page->quads[1].clut = 0x7C3C;
            page->quads[2].clut = 0x7D7C;
        }
        color = g_battle_unit_status_gauge_colors;
        gauge = status->gauges;
        for (i = 0; i < 3; i++) {
            SetPolyG4(poly);
            extent_h = 0xC2;
            extent_w = frame_rect->x + 0x2F;
            if (mode == UNIT_STATS_PANEL_ATTACK_TARGET || mode == UNIT_STATS_PANEL_RIGHT_UNIT) {
                extent_w -= 0x6E;
                extent_h -= 0x10;
            }
            width = (gauge->value << 5) / gauge->max;
            if (width == 0) {
                extent_w = 0;
            }
            hide_values = g_battle_menu_hide_numeric_values;
            if (hide_values != 0) {
                width = 0x20;
            }
            row_y = extent_h + i * 0xB;
            poly->x0 = extent_w;
            poly->y0 = row_y;
            poly->x1 = extent_w + width;
            poly->y1 = row_y;
            poly->x2 = extent_w;
            poly->y2 = row_y + 3;
            poly->x3 = extent_w + width;
            poly->y3 = row_y + 3;
            width = (gauge->bonus << 5) / gauge->max;
            if (width == 0) {
                extent_w = 0;
            }
            poly->x2 -= 3;
            poly->x3 -= 3;
            if (hide_values != 0) {
                poly->r0 = 0x40;
                poly->g0 = 0x40;
                poly->b0 = 0x40;
                poly->r1 = 0x80;
                poly->g1 = 0x80;
                poly->b1 = 0x80;
                poly->r2 = 0x40;
                poly->g2 = 0x40;
                poly->b2 = 0x40;
                poly->r3 = 0xA0;
                poly->g3 = 0xC4;
                poly->b3 = 0xC4;
            } else if (running != 0) {
                poly->r0 = color->r >> 1;
                poly->g0 = color->g >> 1;
                poly->b0 = color->b;
                color++;
                poly->r1 = color->r >> 1;
                poly->g1 = color->g >> 1;
                poly->b1 = color->b;
                color++;
                poly->r2 = color->r >> 1;
                poly->g2 = color->g >> 1;
                poly->b2 = color->b;
                color++;
                poly->r3 = color->r >> 1;
                poly->g3 = color->g >> 1;
                poly->b3 = color->b;
                color++;
            } else {
                poly->r0 = color->r;
                poly->g0 = color->g;
                poly->b0 = color->b;
                color++;
                poly->r1 = color->r;
                poly->g1 = color->g;
                poly->b1 = color->b;
                color++;
                poly->r2 = color->r;
                poly->g2 = color->g;
                poly->b2 = color->b;
                color++;
                poly->r3 = color->r;
                poly->g3 = color->g;
                poly->b3 = color->b;
                color++;
            }
            poly++;
            gauge++;
        }
        if (frame == 0) {
            buffer = battle_menu_alloc_buffer(0x700);
            battle_clear_menu_render_buffer(buffer, 0x700);
            inner_width = &g_menu_text_state.pen.stride;
            *inner_width = 0x28;
            battle_menu_draw_numeric_display_entries(buffer, entries, (world_glyph_blit_t*)(inner_width - 2), 6);
            LoadImage(&rects[0], (u32*)buffer);
            i = g_battle_menu_hide_numeric_values;
            *inner_width = 0x60;
            g_battle_menu_hide_numeric_values = 0;
            if (mode == UNIT_STATS_PANEL_HOVERED_UNIT || mode == UNIT_STATS_PANEL_RIGHT_UNIT) {
                battle_menu_draw_numeric_display_entries(
                    buffer + 0x400, entries + 6, (world_glyph_blit_t*)(inner_width - 2), 2);
                g_battle_menu_hide_numeric_values = i;
                battle_menu_draw_numeric_display_entries(
                    buffer + 0x400, entries + 8, (world_glyph_blit_t*)(inner_width - 2), 2);
            } else if (mode == UNIT_STATS_PANEL_ATTACK_CASTER) {
                battle_menu_draw_numeric_display_entries(
                    buffer + 0x400, entries + 6, (world_glyph_blit_t*)(inner_width - 2), 2);
                g_battle_menu_hide_numeric_values = i;
            } else if (text_id != 0) {
                battle_menu_set_text_origin(2, 0);
                battle_menu_display_text_string((s32)(buffer + 0x400), (s32)(inner_width - 2), (s32)text);
            }
            LoadImage(&rects[1], (u32*)(buffer + 0x400));
            text_loaded = 1;
        }
        offs = &offsets[frame & 1];
        base_y = (g_main_gfx_screen_polarity & 1) == 0 ? 0xF0 : 0;
        if (mode == UNIT_STATS_PANEL_ATTACK_TARGET || mode == UNIT_STATS_PANEL_RIGHT_UNIT) {
            offs->a.x = slide - 0x80;
        } else {
            offs->a.x = -0x80 - slide;
        }
        if (g_battle_unit_status_bounce_step != 0) {
            offs->a.y = base_y + g_battle_unit_status_bounce_offsets[g_battle_unit_status_bounce_step];
        } else {
            offs->a.y = base_y;
        }
        SetDrawOffset(&offs->a, &offs->a.x);
        offs->b.x = -0x80;
        offs->b.y = base_y;
        SetDrawOffset(&offs->b, &offs->b.x);
        battle_gfx_draw_or_append_gpu_primitive(&offs->b);
        battle_gfx_draw_or_append_gpu_primitive(&page->quads[0]);
        battle_gfx_draw_or_append_gpu_primitive(&page->quads[1]);
        battle_gfx_draw_or_append_gpu_primitive(&page->draw_mode_1);
        poly--;
        for (i = 0; i < 3; i++) {
            battle_gfx_draw_or_append_gpu_primitive(poly);
            poly--;
        }
        battle_gfx_draw_or_append_gpu_primitive(&page->quads[2]);
        if (mode == UNIT_STATS_PANEL_ATTACK_CASTER) {
            page->quads[4].clut = 0x7D7C;
            battle_gfx_draw_or_append_gpu_primitive(&page->quads[4]);
        }
        if (mode != UNIT_STATS_PANEL_ATTACK_TARGET) {
            battle_gfx_draw_or_append_gpu_primitive(&page->quads[3]);
        } else if (text_id != 0) {
            battle_menu_build_sprite_page(frame, frame_rect);
        }
        for (i = 0; i < 7; i++) {
            page->sprites[i].clut = g_battle_unit_status_sprite_clut_pairs[i][running & 1];
            battle_gfx_draw_or_append_gpu_primitive(&page->sprites[i]);
        }
        battle_gfx_draw_or_append_gpu_primitive(&page->draw_mode_0);
        battle_gfx_draw_or_append_gpu_primitive(&page->portrait);
        if (running != 0) {
            battle_menu_init_primitive_colors_dimmed(&page->palette);
        } else {
            battle_menu_init_primitive_colors_normal(&page->palette);
        }
        battle_menu_submit_numeric_display_frame_primitives(&page->palette);
        battle_gfx_draw_or_append_gpu_primitive(&offs->a);
        battle_thread_yield();
        g_battle_menu_hide_numeric_values = unit->unit_flags & UNIT_FLAG_EGG;
        if (battle_thread_get_current_task_id() == NATIVE_THREAD_TASK_HOVERED_UNIT_STATS) {
            g_battle_menu_hovered_stats_frame_count++;
        }
    }
exit:
    battle_thread_yield();
    if (battle_thread_get_current_task_id() == NATIVE_THREAD_TASK_HOVERED_UNIT_STATS) {
        g_battle_menu_hovered_stats_frame_count = 0;
    }
    battle_gfx_free_tpage7_vram(&rects[0]);
    battle_gfx_free_tpage7_vram(&rects[1]);
    battle_thread_exit_current();
}
