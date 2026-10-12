#include "fft/wldcore.h"

/* Build the proposition-result panel, total its gil reward, start the reporting
 * unit's message, and push the result level.
 *
 * The first-column local stays live across the text calls. Reading the result
 * through the shared record keeps its load after the level-record stores. */
void wldcore_proposition_push_result_level(s32 proposition) {
    RECT bounds;
    point32_t point;
    s16 text_width;
    s16 text_rows;
    s32 index;
    s32 i;
    s32 message;
    s32 count;
    s32 depth;
    u16* panel_text;
    s32* party;
    point32_t* base;
    s32 rows;

    count = g_main_active_propositions[proposition].participant_count;
    g_wldcore_proposition_gil_amount = 0;
    for (i = 0; i < count; i++) {
        g_wldcore_proposition_gil_amount += g_wldcore_job_selection.participant_gil[i];
    }
    g_wldcore_proposition_gil_amount += g_wldcore_job_selection.reward_value;
    g_world_text_substitution_values[1] = g_wldcore_proposition_gil_amount;
    switch (g_wldcore_job_selection.reward_type) {
    case WLDCORE_PROPOSITION_REWARD_TREASURE:
        g_world_text_substitution_values[0] = g_wldcore_job_selection.reward_index + 0x782E;
        break;
    case WLDCORE_PROPOSITION_REWARD_UNEXPLORED_LAND:
        g_world_text_substitution_values[0] = g_wldcore_job_selection.reward_index + 0x781B;
        break;
    case WLDCORE_PROPOSITION_REWARD_RANDOM_BONUS:
        g_world_text_substitution_values[0]
            = g_wldcore_job_selection.reward_index + TEXT_ID_PROPOSITION_REWARD_NAME_BASE;
        break;
    default:
        g_world_text_substitution_values[0] = 0x7806;
        break;
    }
    index = wldcore_window_append_render_record_and_reset_color(
        g_wldcore_window_render_object_queue, &g_wldcore_window_render_object_count);
    panel_text = &g_wldcore_selected_proposition_row[0].name_text;
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_result.render_index = index;
    world_text_measure_pixels(
        &text_width, &text_rows, world_text_find_entry(*panel_text + (TEXT_ID_PROPOSITION_JOB_CONTENTS_BASE - 1)));
    bounds.x = 0;
    bounds.y = 0x60;
    bounds.w = 0xF4;
    rows = text_rows + 2;
    bounds.h = (count + rows) * 0x10 + 0x10;
    wldcore_window_build_render_record_image(index, bounds, 0, g_wldcore_window_image_buffer);
    point.x = 8;
    point.y = text_rows * 0x10 + 8;
    wldcore_menu_display_text_entry(index, WLDCORE_TEXT_ID_JOB_RESULT_LABELS, point, g_wldcore_window_image_buffer);
    point.x = 8;
    point.y = 8;
    wldcore_menu_display_text_entry(index, WLDCORE_TEXT_ID_JOB_CONTENTS_LABEL, point, g_wldcore_window_image_buffer);
    point.x = 0x44;
    point.y = 8;
    wldcore_menu_display_text_entry(
        index, *panel_text + (TEXT_ID_PROPOSITION_JOB_CONTENTS_BASE - 1), point, g_wldcore_window_image_buffer);
    for (i = 0; i < count; i++) {
        s32 row = i + 2;
        s32 x = 0x38;

        point.y = (text_rows + row) * 0x10 + 8;
        point.x = x;
        wldcore_menu_display_text_entry(index,
            g_main_active_propositions[proposition].participant_indices[i] | TEXT_ID_UNIT_NAME_BASE, point,
            g_wldcore_window_image_buffer);
    }
    for (i = 0; i < count; i++) {
        s32 row = i + 2;

        point.x = 0xA0;
        point.y = (text_rows + row) * 0x10 + 8;
        g_world_text_substitution_values[0] = g_wldcore_job_selection.participant_jp[i];
        wldcore_menu_display_text_entry(index, WLDCORE_TEXT_ID_FOUR_DIGIT_VALUE, point, g_wldcore_window_image_buffer);
    }
    wldcore_window_load_image_record_to_vram(index, g_wldcore_window_image_buffer);
    g_wldcore_window_render_records[index].priority = 9;
    g_wldcore_window_render_records[index].flags |= WORLD_RENDER_RECORD_POP_IN;
    base = &g_wldcore_window_render_records[index].base;
    base->x = -0x7A;
    base->y = -0x2A;
    if (world_thread_is_running(WLDCORE_THREAD_SLOT_MESSAGE) == 0) {
        world_thread_start(WLDCORE_THREAD_SLOT_MESSAGE, world_text_message_box_thread);
    }
    party = &g_wldcore_job_selection.excluded_party_index;
    g_world_text_substitution_values[0] = *party + TEXT_ID_UNIT_NAME_BASE;
    message = 0x9C;
    world_script_set_variable(EVENT_SCRIPT_VAR_DIALOG_PORTRAIT, world_get_party_unit_formation_sprite(*party));
    if (g_wldcore_job_selection.recalled != 0) {
        message = 0xA2;
    }
    if (world_thread_is_running(WLDCORE_THREAD_SLOT_MESSAGE) == 0) {
        world_thread_start(WLDCORE_THREAD_SLOT_MESSAGE, world_text_message_box_thread);
    }
    world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19,
        wldcore_proposition_adjust_message_index(message, *party) + TEXT_ID_WORLD_MESSAGE_BASE, 0);
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_result.proposition = proposition;
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_result.delay = 0;
    switch (g_wldcore_job_selection.result) {
    case WLDCORE_PROPOSITION_RESULT_SUCCESS:
        suzuki_sound_play_2_sfx(MAIN_SFX_PROPOSITION_SUCCESS_A, MAIN_SFX_PROPOSITION_SUCCESS_B);
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].proposition_result.delay = 0x78;
        break;
    case WLDCORE_PROPOSITION_RESULT_FAILURE:
        suzuki_sound_play_2_sfx(MAIN_SFX_PROPOSITION_PARTIAL_A, MAIN_SFX_PROPOSITION_PARTIAL_B);
        break;
    case WLDCORE_PROPOSITION_RESULT_BAD_FAILURE:
        wldcore_sound_play_effect(MAIN_SFX_PROPOSITION_FAILURE);
        break;
    }
    depth = g_wldcore_menu_stack_depth;
    g_wldcore_menu_stack_records_next[depth].proposition_result.phase = WLDCORE_PROPOSITION_RESULT_PHASE_INPUT;
    g_wldcore_menu_stack_types[depth + 1] = WLDCORE_MENU_LEVEL_PROPOSITION_RESULT;
    g_wldcore_menu_stack_depth = depth + 1;
}
