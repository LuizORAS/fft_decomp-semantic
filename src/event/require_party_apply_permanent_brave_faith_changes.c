#include "fft/event_require.h"
#include "psx/types.h"

/*
 * Thread body of the post-battle Brave/Faith report.
 *
 * Exits at once in the world debug battle style, or when
 * require_party_apply_permanent_brave_faith_changes_and_collect_warnings reports
 * nobody. Otherwise it binds the Brave/Faith text sections, shows banner 5
 * and plays each affected unit's warning line in turn before fading the
 * banner out.
 */
void require_party_apply_permanent_brave_faith_changes(void) {
    s32 i;
    s32 main_thread;

    if (battle_script_get_variable(EVENT_SCRIPT_VAR_WORLD_DEBUG_BATTLE_STYLE) != 0)
        battle_thread_exit_current();
    if (require_party_apply_permanent_brave_faith_changes_and_collect_warnings() == 0)
        battle_thread_exit_current();
    g_battle_text_section_pointers[REQUIRE_TEXT_SECTION_BRAVE_FAITH_WARNINGS]
        = g_require_text_data + g_require_text_section_offsets[1];
    g_battle_text_section_pointers[REQUIRE_TEXT_SECTION_BRAVE_FAITH_DEPARTURES]
        = g_require_text_data + g_require_text_section_offsets[2];
    g_battle_text_section_pointers[REQUIRE_TEXT_SECTION_DISMISS_UNIT_REPLIES]
        = g_require_text_data + g_require_text_section_offsets[3];
    g_battle_text_section_pointers[TEXT_SECTION_JOB_NAMES] = g_require_text_data + g_require_text_section_offsets[4];
    main_thread = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
    battle_thread_start(main_thread, require_condition_show_special_banner_thread);
    battle_thread_set_parameters(main_thread, 5, 0, 0);
    require_gfx_fade_rgb_31_frames(0xd, 0xd, 2);
    for (i = 0; i < g_require_party_affected_unit_count; i++) {
        s32 unit_thread = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
        g_battle_text_substitution_values[0] = g_require_party_affected_battle_unit_indices[i];
        battle_thread_start(unit_thread, battle_text_character_handling_thread);
        battle_thread_set_parameters(unit_thread, 0x1b, g_require_party_affected_unit_message_ids[i],
            g_require_party_affected_unit_message_args[i]);
        battle_thread_wait_until_inactive(unit_thread);
    }
    battle_thread_set_parameters(main_thread, 0, 0, 2);
    battle_thread_wait_until_inactive(main_thread);
    require_gfx_fade_rgb_31_frames(6, 5, 2);
    battle_thread_exit_current();
}
