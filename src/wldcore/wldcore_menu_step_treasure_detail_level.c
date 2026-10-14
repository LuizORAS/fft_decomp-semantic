#include "fft/wldcore.h"
#include "psx/libgpu.h"
#include "psx/libgs.h"
#include "psx/pad.h"

/* Per-frame step of a treasure's detail (level TREASURE_DETAIL); the phases mirror
 * wldcore_menu_step_unexplored_land_detail_level.
 *
 * Phase 1 waits for the render record at saved_record_index to leave its 0x100 state, although the push
 * never sets that word (QUIRKS.md). Phase 2, once file loading finishes, uploads the picture's CLUT,
 * binds the level's render record to the VRAM image with its right edge at x -0x3e and centred on y
 * 0x10, starts the dissolve upload (wldcore_gfx_step_dissolve_image_upload) and hands the record index,
 * position and image rectangle to the text window (g_wldcore_scrollable_text_window extra_render_index,
 * extra_base, extra_bounds). Phase 3 advances the dissolve one step per frame. Then Triangle on a book
 * with a sound novel (g_wldcore_treasure_sound_novel_ids) closes the text, fading out unless the novel
 * has a saved place; once WORLD thread 14 has stopped, the detail's windows are released and the novel
 * opens (wldcore_menu_push_sound_novel_resume_level with a saved place, else
 * wldcore_menu_push_sound_novel_level). When the text window finishes instead, the Brave Story's windows
 * come back and the Treasure list opens again.
 *
 * The record index is staged in `saved` before the depth store: the target reloads the full word for the
 * halfword store instead of narrowing the load. The button path's byte uses its own local; sharing
 * `slot` changes the register allocation. */
void wldcore_menu_step_treasure_detail_level(wldcore_menu_detail_level_t* level) {
    GsIMAGE image;
    wldcore_window_render_bounds16_t bounds;
    wldcore_point32_t clut_position;
    wldcore_point32_t* base;
    s32 render;
    s32 slot;
    s32 depth;
    s32 saved;
    s32 entry;

    switch (level->phase) {
    case WLDCORE_DETAIL_PHASE_LOAD:
        if (!(g_wldcore_window_render_records[level->saved_record_index].flags & WLDCORE_DISPLAY_OBJECT_POP_IN)) {
            level->phase = WLDCORE_DETAIL_PHASE_BUILD;
        }
        break;
    case WLDCORE_DETAIL_PHASE_BUILD:
        if (g_main_file_still_loading != 0) {
            break;
        }
        GsGetTimInfo(g_wldcore_picture_buffer + 1, &image);
        LoadClut(image.clut, image.cx, image.cy);
        render = level->render_index;
        bounds.position.x = image.px;
        bounds.position.y = image.py;
        bounds.dimensions.x = image.pw;
        bounds.dimensions.y = image.ph;
        clut_position.x = image.cx;
        clut_position.y = image.cy;
        wldcore_window_init_vram_render_record(render, bounds, clut_position, WLDCORE_DISPLAY_OBJECT_8BIT_CLUT);
        g_wldcore_window_render_records[render].priority = 0xA;
        g_wldcore_window_render_records[render].flags
            &= ~(WLDCORE_DISPLAY_OBJECT_HIDDEN | WLDCORE_DISPLAY_OBJECT_POP_IN);
        base = &g_wldcore_window_render_records[render].base;
        base->x = -0x3E - image.pw;
        base->y = 0x10 - (image.ph >> 1);
        wldcore_gfx_step_dissolve_image_upload(&image, 0);
        g_wldcore_scrollable_text_window.extra_render_index = render;
        g_wldcore_scrollable_text_window.extra_base = *base;
        g_wldcore_scrollable_text_window.extra_bounds = g_wldcore_window_render_records[render].bounds;
        level->phase = level->phase + 1;
        break;
    case WLDCORE_DETAIL_PHASE_POP_IN:
        level->timer = level->timer + 1;
        GsGetTimInfo(g_wldcore_picture_buffer + 1, &image);
        if (wldcore_gfx_step_dissolve_image_upload(&image, level->timer) == 0) {
            level->phase = WLDCORE_DETAIL_PHASE_SHOWN;
        }
        break;
    default:
        if ((g_main_system_flags & (MAIN_SYSTEM_FLAG_INPUT_BLOCKED | MAIN_SYSTEM_FLAG_FADING))
                == MAIN_SYSTEM_FLAG_INPUT_BLOCKED
            && world_thread_is_running(WLDCORE_THREAD_SLOT_MESSAGE) == 0) {
            slot = level->saved_record_index;
            g_main_system_flags ^= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
            g_wldcore_window_record_count -= 4;
            g_wldcore_window_render_record_count -= 2;
            g_wldcore_window_render_object_count -= 6;
            saved = level->saved_record_index;
            g_wldcore_menu_stack_depth--;
            g_wldcore_sound_novel_index = saved;
            if (slot < 5 && g_main_sound_novel_records[slot].section != 0) {
                wldcore_menu_push_sound_novel_resume_level(level->value);
                break;
            }
            wldcore_menu_push_sound_novel_level(level->value);
            break;
        }
        if (wldcore_text_is_window_finished(&g_wldcore_scrollable_text_window) == 0) {
            depth = g_wldcore_menu_stack_depth;
            g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 2].list_window.main_window].flags
                ^= WLDCORE_DISPLAY_OBJECT_HIDDEN;
            g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 2].list_window.side_window].flags
                ^= WLDCORE_DISPLAY_OBJECT_HIDDEN;
            g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[depth - 2].list_window.frame_render].flags
                ^= WLDCORE_DISPLAY_OBJECT_HIDDEN;
            g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[depth - 2].list_window.frame_render].flags
                |= WLDCORE_DISPLAY_OBJECT_POP_IN;
            g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 2].list_window.upper_window].flags
                ^= WLDCORE_DISPLAY_OBJECT_HIDDEN;
            g_wldcore_window_render_record_count--;
            g_wldcore_window_render_object_count--;
            g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 2].list_window.lower_window].flags
                ^= WLDCORE_DISPLAY_OBJECT_HIDDEN;
            g_wldcore_menu_stack_depth = depth - 1;
            wldcore_list_open_treasures();
            break;
        }
        if (g_wldcore_new_button_presses & PSX_PAD_TRIANGLE) {
            entry = g_wldcore_treasure_sound_novel_ids[level->value];
            if (entry != 0) {
                level->saved_record_index = entry - 1;
                if (entry - 1 >= 5 || g_main_sound_novel_records[entry - 1].section == 0) {
                    wldcore_gfx_start_screen_fade(WLDCORE_SCREEN_FADE_OUT, 0x10);
                }
                g_main_system_flags |= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
                world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0, -1, 0);
                wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
            }
        }
        break;
    }
}
