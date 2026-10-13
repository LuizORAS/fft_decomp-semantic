#include "fft/gfx.h"
#include "fft/thread.h"
/*
 * WORLD twin: world_menu_run_unit_editor_panel_thread (same code, globals and callees rebound).
 */
#include "fft/battle.h"
#include "fft/battle_script.h"
#include "psx/libgpu.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Unit debug editor panel thread (task 0x15). Builds two copies of the panel
 * frame (19 sprites, 5 item icons, two tiles, eight lines and the text images
 * for the unit's items and labels), then redraws the current copy each frame
 * until cancelled or told to stop; square opens the status list thread. The
 * WORLD twin is world_menu_run_unit_editor_panel_thread. */
void battle_menu_run_unit_editor_panel_thread(void) {
    /* The frame reserves 0x100 bytes for the first copy's line-box
       primitives, the stride of the palette records (WORLD 0x80195ed0). */
    menu_palette_slot_t palette;
    menu_unit_editor_frame_t* records;
    menu_unit_editor_frame_t* frame_record;
    LINE_F2* line;
    u8* buffer;
    /* One variable for the label text id and, in the frame loop, whether the
       status thread runs: the target keeps both in one callee-saved register. */
    s32 value;
    s32 frame;
    s32 i;

    battle_menu_open_debugchr_unit_editor();
    thread_set_current_task_id(NATIVE_THREAD_TASK_UNIT_EDITOR_PANEL);
    battle_menu_enter_status_screen_selection();
    g_battle_menu_suspend_state = 1;
    g_battle_menu_panel_fade_mode = MENU_PANEL_FADE_OUT;
    thread_set_parameters(NATIVE_THREAD_SLOT_TILE_INFO_PANEL, 0, 0, 1);
    thread_set_parameters(NATIVE_THREAD_SLOT_MENU, 0, 0, 1);
    battle_menu_record_selection_for_selected_unit();
    battle_menu_set_modal_open();
    do {
        thread_yield();
    } while (g_battle_menu_hovered_stats_frame_count < 2);
    battle_menu_clear_modal_open();

    frame_record = g_battle_menu_unit_editor_frames;
    records = frame_record;
    battle_gfx_init_scaled_draw_area_packets(&frame_record->draw_areas);
    g_battle_menu_hovered_unit_stats_display.y = 0x1A;
    g_battle_unit_summary_panel_rect.y = 0x1A;
    battle_gfx_set_draw_mode_for_texture_page(&frame_record->draw_modes[0], MENU_TEXTURE_PAGE_FRAME);
    battle_gfx_set_draw_mode_for_texture_page(&frame_record->draw_modes[1], MENU_TEXTURE_PAGE_TEXT_IMAGES);
    battle_gfx_set_draw_mode_for_texture_page(&frame_record->draw_modes[2], MENU_TEXTURE_PAGE_ITEM_ICONS);
    SetTile(&frame_record->tiles[0]);
    SetSemiTrans(&frame_record->tiles[0], 1);
    setRGB0(&frame_record->tiles[0], 0x30, 0x30, 0x30);
    frame_record->tiles[0].w = 0x10;
    frame_record->tiles[0].h = 0x5A;
    frame_record->tiles[0].x0 = g_battle_unit_editor_panel_rect.x;
    frame_record->tiles[0].y0 = g_battle_unit_editor_panel_rect.y + 1;
    battle_copy_bytes(&frame_record->tiles[1], &frame_record->tiles[0], sizeof(TILE));
    frame_record->tiles[0].x0 += 0x12;
    frame_record->tiles[1].x0 += 0x7C;
    line = frame_record->lines;
    for (i = 0; i < 8 * MENU_UNIT_EDITOR_LINE_SIZE; i += MENU_UNIT_EDITOR_LINE_SIZE) {
        SetLineF2(line);
        SetSemiTrans(line, 1);
        setRGB0(line, g_battle_unit_editor_line_colors[i + MENU_UNIT_EDITOR_LINE_R],
            g_battle_unit_editor_line_colors[i + MENU_UNIT_EDITOR_LINE_G],
            g_battle_unit_editor_line_colors[i + MENU_UNIT_EDITOR_LINE_B]);
        setXY2(line, g_battle_unit_editor_line_colors[i + MENU_UNIT_EDITOR_LINE_X0] + g_battle_unit_editor_panel_rect.x,
            g_battle_unit_editor_line_colors[i + MENU_UNIT_EDITOR_LINE_Y0] + g_battle_unit_editor_panel_rect.y,
            g_battle_unit_editor_line_colors[i + MENU_UNIT_EDITOR_LINE_X1] + g_battle_unit_editor_panel_rect.x,
            g_battle_unit_editor_line_colors[i + MENU_UNIT_EDITOR_LINE_Y1] + g_battle_unit_editor_panel_rect.y);
        line++;
    }
    battle_menu_init_sprite_array(&frame_record->sprites[0], 0xC, 0x7D7C);
    battle_menu_init_sprite_array(&frame_record->sprites[12], 7, 0x7C3C);

    /* Each arm copies all four rows itself; the compiler cross-jumps the
       common tails, leaving only the first call's table loads in the arms. */
    if (g_battle_menu_billboard_data.state.two_hands != 0) {
        battle_copy_bytes(&g_battle_unit_editor_sprite_image_params_a[2], g_battle_unit_editor_row_image_params_b,
            2 * sizeof(image_load_parameters_t));
        battle_copy_bytes(&g_battle_unit_editor_sprite_image_params_a[14], &g_battle_unit_editor_row_image_params_b[2],
            2 * sizeof(image_load_parameters_t));
        battle_copy_bytes(&g_battle_unit_editor_sprite_image_params_b[2], g_battle_unit_editor_row_image_params_b,
            2 * sizeof(image_load_parameters_t));
        battle_copy_bytes(&g_battle_unit_editor_sprite_image_params_b[14], &g_battle_unit_editor_row_image_params_b[2],
            2 * sizeof(image_load_parameters_t));
    } else {
        battle_copy_bytes(&g_battle_unit_editor_sprite_image_params_a[2], g_battle_unit_editor_row_image_params_a,
            2 * sizeof(image_load_parameters_t));
        battle_copy_bytes(&g_battle_unit_editor_sprite_image_params_a[14], &g_battle_unit_editor_row_image_params_a[2],
            2 * sizeof(image_load_parameters_t));
        battle_copy_bytes(&g_battle_unit_editor_sprite_image_params_b[2], g_battle_unit_editor_row_image_params_a,
            2 * sizeof(image_load_parameters_t));
        battle_copy_bytes(&g_battle_unit_editor_sprite_image_params_b[14], &g_battle_unit_editor_row_image_params_a[2],
            2 * sizeof(image_load_parameters_t));
    }
    /* These sprite and rectangle records share the fields read by the image loader. */
    if (g_battle_menu_billboard_data.state.generic_monster == 0) {
        for (i = 0; i < 19; i++) {
            gfx_init_image_loading(&frame_record->sprites[i], &g_battle_menu_texture_location,
                &g_battle_unit_editor_panel_rect, &g_battle_unit_editor_sprite_image_params_a[i]);
        }
    } else {
        for (i = 0; i < 19; i++) {
            gfx_init_image_loading(&frame_record->sprites[i], &g_battle_menu_texture_location,
                &g_battle_unit_editor_panel_rect, &g_battle_unit_editor_sprite_image_params_b[i]);
        }
    }
    battle_menu_init_sprite_array(&frame_record->sprites[19], 5, 0x7CFC);
    for (i = 0; i < 5; i++) {
        gfx_init_image_loading(&frame_record->sprites[19 + i], &g_battle_item_icon_image_location,
            &g_battle_unit_editor_panel_rect, &g_battle_unit_editor_item_icon_image_params[i]);
        battle_gfx_set_item_icon_sprite(
            &frame_record->sprites[19 + i], g_battle_menu_billboard_data.state.equipment[i]);
    }

    buffer = battle_menu_alloc_buffer(0xC80);
    battle_clear_menu_render_buffer(buffer, 0xC80);
    g_menu_text_state.pen.stride = 0x50;
    battle_menu_set_text_origin(0, 0);
    for (i = 0; i < 5; i++) {
        value = g_battle_menu_billboard_data.state.equipment[i] + TEXT_ID_ITEM_NAME_BASE;
        if ((u16)g_battle_menu_billboard_data.state.equipment[i] == 0xFFFF) {
            g_menu_text_state.pen.rect.y += 0x10;
        } else {
            battle_menu_display_text_entry(value, buffer, &g_menu_text_state.pen);
            g_menu_text_state.pen.rect.y += 0x10;
        }
    }
    LoadImage(&g_battle_unit_editor_text_rect_a, (u32*)buffer);
    thread_yield();
    battle_clear_menu_render_buffer(buffer, 0xC80);
    g_menu_text_state.pen.stride = 0x50;
    battle_menu_set_text_origin(0, 0);
    for (i = 0; i < 2; i++) {
        value = g_battle_menu_billboard_data.state.ability_ids[i] + TEXT_ID_SKILLSET_NAME_SOURCE_BASE;
        if (g_battle_menu_billboard_data.state.generic_monster != 0) {
            value = g_battle_menu_billboard_data.state.ability_ids[i] + TEXT_ID_ABILITY_NAME_BASE;
        }
        if ((u16)g_battle_menu_billboard_data.state.ability_ids[i] == MENU_STATUS_PANEL_LABEL_NONE) {
            g_menu_text_state.pen.rect.y += 0x10;
        } else {
            battle_menu_display_text_entry(value, buffer, &g_menu_text_state.pen);
            g_menu_text_state.pen.rect.y += 0x10;
        }
    }
    for (i = 2; i < 5; i++) {
        value = g_battle_menu_billboard_data.state.ability_ids[i] + TEXT_ID_ABILITY_NAME_BASE;
        if ((u16)g_battle_menu_billboard_data.state.ability_ids[i] == MENU_STATUS_PANEL_LABEL_NONE) {
            g_menu_text_state.pen.rect.y += 0x10;
        } else {
            battle_menu_display_text_entry(value, buffer, &g_menu_text_state.pen);
            g_menu_text_state.pen.rect.y += 0x10;
        }
    }
    if (g_battle_menu_billboard_data.state.generic_monster != 0) {
        for (i = 0; i < 5; i++) {
            if ((u16)g_battle_menu_billboard_data.state.ability_ids[i] == MENU_STATUS_PANEL_LABEL_NONE) {
                frame_record->sprites[7 + i].x0 -= 0x200;
            }
        }
    }
    LoadImage(&g_battle_unit_editor_text_rect_b, (u32*)buffer);
    thread_yield();
    battle_menu_free_memory(buffer);

    battle_copy_bytes(&records[1], frame_record, sizeof(menu_unit_editor_frame_t));
    records[0].palette = &palette.primitives;
    records[1].palette = &g_battle_unit_editor_palette;
    battle_menu_init_numeric_display_frame_primitives(&g_battle_unit_editor_panel_rect, records[0].palette);
    battle_menu_init_numeric_display_frame_primitives(&g_battle_unit_editor_panel_rect, records[1].palette);
    thread_start(NATIVE_THREAD_SLOT_TILE_INFO_PANEL, battle_menu_run_numeric_display_panel_thread);
    thread_set_parameters(NATIVE_THREAD_SLOT_TILE_INFO_PANEL, 0, 0, 0);
    thread_yield();

    frame = 0;
    for (;;) {
        thread_yield();
        frame_record = &records[frame & 1];
        battle_gfx_build_scaled_draw_area_pair_swapped(
            &frame_record->draw_areas, &g_battle_unit_editor_cursor_rect, frame, g_main_gfx_screen_polarity);
        if (g_battle_menu_input_lock_mode == 0 && g_battle_menu_help_open == 0
            && battle_thread_get_current_parameter_3() != 0) {
            break;
        }
        value = battle_thread_is_running_asm(g_native_current_thread_id - 1);
        for (i = 0; i < 19; i++) {
            frame_record->sprites[i].clut = g_battle_unit_editor_sprite_clut_pairs[i][value];
        }
        for (i = 0; i < 5; i++) {
            if (value != 0) {
                SetShadeTex(&frame_record->sprites[19 + i], 0);
                /* Spelled from sprites[i]: the target's store pointer walks
                   frame_record + i * 0x14 with the item-icon bias in the
                   displacement; sprites[19 + i] adds the base per store. */
                (&frame_record->sprites[i])[19].r0 = 0x40;
                (&frame_record->sprites[i])[19].g0 = 0x40;
                (&frame_record->sprites[i])[19].b0 = 0x80;
            } else {
                SetShadeTex(&frame_record->sprites[19 + i], 1);
            }
        }
        if (value == 0) {
            battle_menu_init_primitive_colors_normal(frame_record->palette);
        } else {
            battle_menu_init_primitive_colors_dimmed(frame_record->palette);
        }
        if (battle_menu_can_accept_input() != 0) {
            if (g_script_event_input & PSX_PAD_CROSS) {
                thread_set_parameters(NATIVE_THREAD_SLOT_TILE_INFO_PANEL, 0, 0, 1);
                break;
            }
            if ((g_script_event_input & PSX_PAD_CIRCLE)
                && battle_thread_is_running_asm(NATIVE_THREAD_SLOT_MENU_BUILDING) == 0
                && (battle_unit_has_any_current_status(g_battle_active_turn_unit.battle_id) != 0
                    || (g_battle_menu_billboard_data.state.generic_monster == 0
                        && g_battle_menu_active_turn_banner.team_kind != 1))) {
                thread_start(g_native_current_thread_id - 1, battle_menu_build_unit_status_list);
                thread_set_parameters(g_native_current_thread_id - 1,
                    (s32)&g_battle_menu_thread_menu_data[BATTLE_MENU_ID_STATUS_LIST], 0, 0);
            }
        }
        battle_gfx_draw_or_append_gpu_primitive(&frame_record->draw_areas.areas[1]);
        for (i = 0; i < 12; i++) {
            battle_gfx_draw_or_append_gpu_primitive(&frame_record->sprites[i]);
        }
        for (i = 14; i < 19; i++) {
            battle_gfx_draw_or_append_gpu_primitive(&frame_record->sprites[i]);
        }
        battle_gfx_draw_or_append_gpu_primitive(&frame_record->draw_modes[0]);
        battle_gfx_draw_or_append_gpu_primitive(&frame_record->sprites[12]);
        battle_gfx_draw_or_append_gpu_primitive(&frame_record->sprites[13]);
        battle_gfx_draw_or_append_gpu_primitive(&frame_record->draw_modes[1]);
        for (i = 0; i < 5; i++) {
            if ((u16)g_battle_menu_billboard_data.state.equipment[i] != 0xFFFF) {
                battle_gfx_draw_or_append_gpu_primitive(&frame_record->sprites[19 + i]);
            }
        }
        battle_gfx_draw_or_append_gpu_primitive(&frame_record->draw_modes[2]);
        battle_gfx_draw_or_append_gpu_primitive(&frame_record->tiles[0]);
        battle_gfx_draw_or_append_gpu_primitive(&frame_record->tiles[1]);
        for (i = 0; i < 8; i++) {
            battle_gfx_draw_or_append_gpu_primitive(&frame_record->lines[i]);
        }
        frame++;
        battle_menu_submit_numeric_display_frame_primitives(frame_record->palette);
        battle_gfx_draw_or_append_gpu_primitive(&frame_record->draw_areas.areas[0]);
    }
    g_battle_menu_hovered_unit_stats_display.y = 0xAA;
    g_battle_unit_summary_panel_rect.y = 0xAA;
    g_battle_menu_panel_fade_mode = MENU_PANEL_FADE_IN;
    g_battle_unit_status_bounce_step = 6;
    sound_queue_sfx_cancel();
    do {
        thread_yield();
    } while (g_battle_unit_status_bounce_step != 0);
    thread_start(NATIVE_THREAD_SLOT_TILE_INFO_PANEL, battle_menu_selected_tile_info_display_thread);
    thread_set_parameters(NATIVE_THREAD_SLOT_TILE_INFO_PANEL, 0, 0, 0);
    g_native_thread_contexts[NATIVE_THREAD_SLOT_TILE_INFO_PANEL].task_id = NATIVE_THREAD_TASK_TILE_INFO_PANEL;
    g_battle_menu_suspend_state = 0;
    battle_menu_leave_status_screen_selection();
    battle_menu_build_idle_action_menu(g_battle_menu_current_id);
    thread_exit_current();
}
