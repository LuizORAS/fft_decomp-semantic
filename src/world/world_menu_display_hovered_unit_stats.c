#include "fft/battle.h"
#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* &g_world_menu_sprite_page_image_params[5]; spelled as an element (params[frame + 5]
 * or (&params[5])[frame]) GCC shares the params base and the code changes. */
extern world_gfx_image_load_parameters_t g_world_menu_sprite_page_image_params_from_5[];

/*
 * Unit status panel thread body (HP/MP/CT gauges, portrait, numbers and, in
 * mode 2, the reaction-ability name), the WORLD twin of BATTLE battle_menu_display_hovered_unit_stats
 * built with divide checks. Builds two alternating 0x2ec-byte pages, slides
 * the panel in by `slide`/`speed` each event tick and out when the thread's
 * third parameter is set (mode 0 also leaves immediately while thread 12's task_id
 * is 0x13). A running task-0x15 thread dims the portrait, gauges and CLUTs.
 *
 * `i` doubles as the saved hide-numbers flag in the first-frame text pass, as
 * in the target's register use. Gauge corners are stored in setXYWH order
 * (x0, y0, x1, y1, ...); storing y0/y1 first lengthens the reloaded
 * `extent_w`'s lifetime and swaps its register with the gauge width.
 */
void world_menu_display_hovered_unit_stats(
    RECT* frame_rect, s32 mode, menu_number_entry_t* entries, battle_unit_status_record_t* status) {
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
    s32 frame;
    s32 i;
    s32 flag;
    s32 y;
    s32 row_y;
    u16 width;

    unit_id = status->unit_index;
    speed = 0x16;
    unit = battle_unit_get_stats_from_battle_id(unit_id);
    slide = 0x84;
    g_world_menu_hide_numeric_values = unit->unit_flags & UNIT_FLAG_EGG;
    if (world_thread_get_current_task_id() == NATIVE_THREAD_TASK_HOVERED_UNIT_STATS) {
        g_world_menu_hovered_stats_frame_count++;
    }
    if (mode == UNIT_STATS_PANEL_ATTACK_TARGET) {
        text_id = unit->reaction_ability;
        if (text_id >= ABILITY_ID_COUNT) {
            text_id = 0;
        }
        if (text_id != 0) {
            text = world_text_find_entry(text_id + TEXT_ID_ABILITY_NAME_BASE);
            g_world_thread_call_target = (void (*)(void))world_text_measure_pixels;
            world_thread_call_on_main_stack(&extent_w, &extent_h, text);
            if (extent_w & 3) {
                extent_w = (extent_w & 0xFFFC) + 4;
            }
            extent_w = 0x4C - extent_w;
            if (extent_w > 0x100) {
                extent_w = 0;
            }
            g_world_menu_sprite_page_image_params[25].x_screen_offset
                = g_world_menu_sprite_page_image_params[28].x_screen_offset + extent_w;
            g_world_menu_sprite_page_image_params[26].x_screen_offset
                = g_world_menu_sprite_page_image_params[29].x_screen_offset + extent_w;
            for (frame = 0; frame < 3; frame++) {
                g_world_menu_sprite_page_image_params[frame].x_screen_offset
                    = g_world_menu_sprite_page_image_params_from_5[frame].x_screen_offset + extent_w;
                g_world_menu_sprite_page_image_params[frame].width
                    = g_world_menu_sprite_page_image_params_from_5[frame].width - extent_w;
            }
            g_world_menu_sprite_page_image_params[3].x_screen_offset
                = g_world_menu_sprite_page_image_params[8].x_screen_offset + extent_w;
        }
    }
    page = &g_world_unit_status_panel_pages[g_world_unit_status_first_page_by_mode[mode]];
    world_menu_init_numeric_display_frame_primitives(frame_rect, &page->palette);
    world_gfx_set_draw_mode_for_texture_page(&page->draw_mode_1, MENU_TEXTURE_PAGE_ALLOCATED_IMAGES);
    world_gfx_set_draw_mode_for_texture_page(&page->draw_mode_0, MENU_TEXTURE_PAGE_FRAME);
    world_menu_init_sprite_array(page->sprites, 7, 0x7CBC);
    for (frame = 0; frame < 5; frame++) {
        world_gfx_init_default_poly_ft4(&page->quads[frame]);
    }
    world_gfx_init_default_poly_ft4(&page->portrait);
    world_text_configure_sprite_vram(&rects[0], 0x28, 0x28, (world_texture_prim_t*)&page->quads[0], 2);
    world_text_configure_sprite_vram(&rects[1], 0x60, 0x10, (world_texture_prim_t*)&page->quads[1], 2);
    params = g_world_unit_status_quad_image_params_by_mode[mode];
    if (mode == UNIT_STATS_PANEL_ATTACK_CASTER) {
        world_gfx_init_image_loading(
            (world_texture_prim_t*)&page->quads[4], &g_world_gfx_menu_image_source, frame_rect, &params[4]);
        for (frame = 0; frame < 2; frame++, params++) {
            world_gfx_init_image_loading(
                (world_texture_prim_t*)&page->quads[frame], &g_world_gfx_menu_image_source, frame_rect, params);
            page->quads[frame].clut = 0x7CBC;
        }
        world_menu_display_projected_action_effect(
            &page->quads[frame], &g_world_gfx_menu_image_source, frame_rect, params);
    } else if (mode == UNIT_STATS_PANEL_ATTACK_TARGET) {
        for (frame = 0; frame < 2; frame++, params++) {
            world_gfx_init_image_loading(
                (world_texture_prim_t*)&page->quads[frame], &g_world_gfx_menu_image_source, frame_rect, params);
        }
        if (text_id != 0) {
            world_gfx_init_image_loading(
                (world_texture_prim_t*)&page->quads[frame], &g_world_gfx_menu_image_source, frame_rect, params);
        }
    } else {
        for (frame = 0; frame < 4; frame++, params++) {
            world_gfx_init_image_loading(
                (world_texture_prim_t*)&page->quads[frame], &g_world_gfx_menu_image_source, frame_rect, params);
        }
    }
    world_script_copy_bytes(&page[1], page, sizeof(world_unit_status_page_t));
    text_loaded = 0;
    for (frame = 0;; frame++) {
        if (status->gauges[BATTLE_UNIT_STATUS_GAUGE_CT].value > 100) {
            status->gauges[BATTLE_UNIT_STATUS_GAUGE_CT].value = 100;
        }
        if (text_loaded != 0) {
            world_menu_free_memory(buffer);
            text_loaded = 0;
        }
        for (i = 0; i < g_world_event_speed; i++) {
            if (world_thread_get_current_parameter_3() != 0) {
                /* Both exits leave the speed and frame loops for the exit tail. */
                if (mode == UNIT_STATS_PANEL_HOVERED_UNIT
                    && g_world_thread_contexts[NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS].task_id
                        == NATIVE_THREAD_TASK_ATTACK_CASTER_STATS) {
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
        running = world_thread_find_running_by_task(NATIVE_THREAD_TASK_UNIT_EDITOR_PANEL);
        if (running != 0 && world_thread_is_running_asm(running - 1) == 0) {
            running = 0;
        }
        page = &g_world_unit_status_panel_pages[g_world_unit_status_first_page_by_mode[mode] + (frame & 1)];
        world_gfx_set_portrait_poly_texture(&page->portrait, unit_id);
        if (running != 0) {
            setRGB0(&page->portrait, 0x40, 0x50, 0x70);
        } else {
            setRGB0(&page->portrait, 0x80, 0x80, 0x80);
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
        params = g_world_unit_status_sprite_image_params_by_mode[mode];
        if (g_world_unit_status_bounce_step == 0) {
            setXY4(&page->portrait, frame_rect->x + 2, frame_rect->y + 2, frame_rect->x + 0x21, frame_rect->y + 2,
                frame_rect->x + 2, frame_rect->y + 0x32, frame_rect->x + 0x21, frame_rect->y + 0x32);
            world_script_copy_bytes(
                &params[6], &g_world_unit_status_icon_image_params[status->team_kind], sizeof(*params));
            for (i = 0; i < 7; i++, params++) {
                world_gfx_init_image_loading(
                    (world_texture_prim_t*)&page->sprites[i], &g_world_gfx_menu_image_source, frame_rect, params);
            }
        }
        if (mode != UNIT_STATS_PANEL_ATTACK_CASTER) {
            for (i = 0; i < 4; i++) {
                page->quads[i].clut = g_world_unit_status_quad_clut_pairs[i][running & 1];
            }
        }
        poly = page->bars;
        if (mode == UNIT_STATS_PANEL_ATTACK_TARGET) {
            page->quads[1].clut = 0x7C3C;
            page->quads[2].clut = 0x7D7C;
        }
        color = g_world_unit_status_gauge_colors;
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
            flag = g_world_menu_hide_numeric_values;
            if (flag != 0) {
                width = 0x20;
            }
            row_y = extent_h + i * 0xB;
            setXY4(poly, extent_w, row_y, extent_w + width, row_y, extent_w, row_y + 3, extent_w + width, row_y + 3);
            width = (gauge->bonus << 5) / gauge->max;
            if (width == 0) {
                extent_w = 0;
            }
            poly->x2 -= 3;
            poly->x3 -= 3;
            if (flag != 0) {
                setRGB0(poly, 0x40, 0x40, 0x40);
                setRGB1(poly, 0x80, 0x80, 0x80);
                setRGB2(poly, 0x40, 0x40, 0x40);
                setRGB3(poly, 0xA0, 0xC4, 0xC4);
            } else if (running != 0) {
                setRGB0(poly, color->r >> 1, color->g >> 1, color->b);
                color++;
                setRGB1(poly, color->r >> 1, color->g >> 1, color->b);
                color++;
                setRGB2(poly, color->r >> 1, color->g >> 1, color->b);
                color++;
                setRGB3(poly, color->r >> 1, color->g >> 1, color->b);
                color++;
            } else {
                setRGB0(poly, color->r, color->g, color->b);
                color++;
                setRGB1(poly, color->r, color->g, color->b);
                color++;
                setRGB2(poly, color->r, color->g, color->b);
                color++;
                setRGB3(poly, color->r, color->g, color->b);
                color++;
            }
            poly++;
            gauge++;
        }
        if (frame == 0) {
            buffer = world_menu_alloc_buffer(0x700);
            world_clear_menu_render_buffer(buffer, 0x700);
            g_world_menu_text_state.pen.stride = 0x28;
            world_menu_draw_numeric_display_entries(buffer, entries, &g_world_menu_text_state.pen, 6);
            LoadImage(&rects[0], (u32*)buffer);
            i = g_world_menu_hide_numeric_values;
            g_world_menu_text_state.pen.stride = 0x60;
            g_world_menu_hide_numeric_values = 0;
            if (mode == UNIT_STATS_PANEL_HOVERED_UNIT || mode == UNIT_STATS_PANEL_RIGHT_UNIT) {
                world_menu_draw_numeric_display_entries(buffer + 0x400, entries + 6, &g_world_menu_text_state.pen, 2);
                g_world_menu_hide_numeric_values = i;
                world_menu_draw_numeric_display_entries(buffer + 0x400, entries + 8, &g_world_menu_text_state.pen, 2);
            } else if (mode == UNIT_STATS_PANEL_ATTACK_CASTER) {
                world_menu_draw_numeric_display_entries(buffer + 0x400, entries + 6, &g_world_menu_text_state.pen, 2);
                g_world_menu_hide_numeric_values = i;
            } else if (text_id != 0) {
                world_menu_set_text_origin(2, 0);
                world_menu_display_text_string(buffer + 0x400, &g_world_menu_text_state.pen, text);
            }
            LoadImage(&rects[1], (u32*)(buffer + 0x400));
            text_loaded = 1;
        }
        offs = &offsets[frame & 1];
        y = (g_main_gfx_screen_polarity & 1) == 0 ? 0xF0 : 0;
        if (mode == UNIT_STATS_PANEL_ATTACK_TARGET || mode == UNIT_STATS_PANEL_RIGHT_UNIT) {
            offs->a.position.x = slide - 0x80;
        } else {
            offs->a.position.x = -0x80 - slide;
        }
        if (g_world_unit_status_bounce_step != 0) {
            offs->a.position.y = y + g_world_unit_status_bounce_offsets[g_world_unit_status_bounce_step];
        } else {
            offs->a.position.y = y;
        }
        SetDrawOffset(&offs->a.packet, (u16*)&offs->a.position);
        offs->b.position.x = -0x80;
        offs->b.position.y = y;
        SetDrawOffset(&offs->b.packet, (u16*)&offs->b.position);
        world_gfx_draw_or_append_gpu_primitive(&offs->b);
        world_gfx_draw_or_append_gpu_primitive(&page->quads[0]);
        world_gfx_draw_or_append_gpu_primitive(&page->quads[1]);
        world_gfx_draw_or_append_gpu_primitive(&page->draw_mode_1);
        poly--;
        for (i = 0; i < 3; i++) {
            world_gfx_draw_or_append_gpu_primitive(poly);
            poly--;
        }
        world_gfx_draw_or_append_gpu_primitive(&page->quads[2]);
        if (mode == UNIT_STATS_PANEL_ATTACK_CASTER) {
            page->quads[4].clut = 0x7D7C;
            world_gfx_draw_or_append_gpu_primitive(&page->quads[4]);
        }
        if (mode != UNIT_STATS_PANEL_ATTACK_TARGET) {
            world_gfx_draw_or_append_gpu_primitive(&page->quads[3]);
        } else if (text_id != 0) {
            world_menu_build_sprite_page(frame, frame_rect);
        }
        for (i = 0; i < 7; i++) {
            page->sprites[i].clut = g_world_unit_status_sprite_clut_pairs[i][running & 1];
            world_gfx_draw_or_append_gpu_primitive(&page->sprites[i]);
        }
        world_gfx_draw_or_append_gpu_primitive(&page->draw_mode_0);
        world_gfx_draw_or_append_gpu_primitive(&page->portrait);
        if (running != 0) {
            world_menu_init_primitive_colors_dimmed(&page->palette);
        } else {
            world_menu_init_primitive_colors_normal(&page->palette);
        }
        world_menu_submit_numeric_display_frame_primitives(&page->palette);
        world_gfx_draw_or_append_gpu_primitive(&offs->a);
        world_thread_yield();
        g_world_menu_hide_numeric_values = unit->unit_flags & UNIT_FLAG_EGG;
        if (world_thread_get_current_task_id() == NATIVE_THREAD_TASK_HOVERED_UNIT_STATS) {
            g_world_menu_hovered_stats_frame_count++;
        }
    }
exit:
    world_thread_yield();
    if (world_thread_get_current_task_id() == NATIVE_THREAD_TASK_HOVERED_UNIT_STATS) {
        g_world_menu_hovered_stats_frame_count = 0;
    }
    world_gfx_free_tpage7_vram(&rects[0]);
    world_gfx_free_tpage7_vram(&rects[1]);
    world_thread_exit_current();
}
