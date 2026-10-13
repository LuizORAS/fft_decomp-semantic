#include "fft/event_require.h"
#include "psx/types.h"

/*
 * Thread body of the REQUIRE overlay: runs one display condition.
 *
 * Parameter 1 is the condition id and parameter 2 its argument. The general
 * conditions draw their banner inline (the argument is the hold time). The
 * others each run their routine in the first idle thread slot and wait for
 * it: the special-case banner (held for the argument's frames or until
 * skipped), "ready", the battle congratulations, the gil reward, the war
 * trophies, the permanent Brave/Faith changes and the low-Brave/high-Faith
 * desertions. The post-battle unit changes run inline.
 */
void require_condition_dispatch(void) {
    s32 condition;
    s32 parameter;
    s32 thread;

    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_CONDITION_DISPATCH);
    condition = battle_thread_get_current_parameter_1();
    parameter = battle_thread_get_current_parameter_2();
    if (condition >= EVENT_DISPLAY_CONDITION_GENERAL_FIRST)
        require_condition_show_bonus_banner(parameter);
    thread = battle_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);

    if (condition == EVENT_DISPLAY_CONDITION_SPECIAL_CASES) {
        battle_thread_start(thread, require_condition_show_special_banner_thread);
        battle_thread_set_parameters(thread, 0, 0, 0);
        require_input_wait_frames_or_skip(parameter);
        battle_thread_set_parameters(thread, 0, 0, 2);
        battle_thread_wait_until_inactive(thread);
    } else if (condition == EVENT_DISPLAY_CONDITION_HOVERED_UNIT_STATS) {
        battle_thread_start(thread, require_condition_show_hovered_unit_stats);
        battle_thread_set_parameters(thread, 0, 0, 0);
        battle_thread_wait_until_inactive(thread);
    } else if (condition == EVENT_DISPLAY_CONDITION_BATTLE_CONGRATULATIONS) {
        battle_thread_start(thread, require_text_show_battle_congratulations);
        battle_thread_set_parameters(thread, 0, 0, 0);
        battle_thread_wait_until_inactive(thread);
    } else if (condition == EVENT_DISPLAY_CONDITION_GIL_REWARD) {
        battle_thread_start(thread, require_reward_render_gil_ticker);
        battle_thread_set_parameters(thread, 0, 0, 0);
        battle_thread_wait_until_inactive(thread);
    } else if (condition == EVENT_DISPLAY_CONDITION_WAR_TROPHIES) {
        battle_thread_start(thread, require_reward_render_war_trophies);
        battle_thread_set_parameters(thread, 0, 0, 0);
        battle_thread_wait_until_inactive(thread);
    } else if (condition == EVENT_DISPLAY_CONDITION_APPLY_PERMANENT_BRAVE_FAITH_CHANGES) {
        battle_thread_start(thread, require_party_apply_permanent_brave_faith_changes);
        battle_thread_set_parameters(thread, 0, 0, 0);
        battle_thread_wait_until_inactive(thread);
    } else if (condition == EVENT_DISPLAY_CONDITION_REMOVE_LOW_BRAVE_HIGH_FAITH_UNITS) {
        battle_thread_start(thread, require_party_remove_low_brave_high_faith_units);
        battle_thread_set_parameters(thread, 0, 0, 0);
        battle_thread_wait_until_inactive(thread);
    } else if (condition == EVENT_DISPLAY_CONDITION_APPLY_POST_BATTLE_UNIT_CHANGES) {
        require_party_apply_post_battle_unit_changes();
    }
    battle_thread_exit_current();
}
