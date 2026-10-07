#include "fft/battle.h"
#include "psx/types.h"

/* The event's Inflict Status command as a thread (task NATIVE_THREAD_TASK_INFLICT_STATUS). Its
 * parameter points at the operands: ENTD unit id halfword, mode byte, wait-frame halfword. The unit
 * is staged first (battle_unit_update_staged_status_data). Mode 0 revives a dead unit at 1 HP with
 * Critical (sound 0x41), or refreshes the mounted animation of a unit neither dead nor critical;
 * mode 1 removes Dead and Critical and adds Crystal; mode 2 adds Poison with animation 0x16. Each
 * records the exit mode (1, 2 or 3) that battle_unit_apply_staged_status_data applies when the event
 * ends. Then it waits the frames and exits. The WORLD twin is world_script_inflict_status_thread. */
void battle_script_inflict_status_thread(void) {
    u8* parameters;
    s32 unit_id;
    s32 mode;
    s32 wait_frames;
    s32 battle_unit_index;
    s32 misc_id;
    battle_stats_t* unit;
    s32 state;

    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_INFLICT_STATUS);
    parameters = (u8*)battle_thread_get_current_parameter_1();
    unit_id = battle_script_load_halfword(parameters);
    mode = parameters[2];
    wait_frames = battle_script_load_halfword(parameters + 3);
    battle_unit_index = battle_unit_get_battle_index_by_entd_unit_id(unit_id);
    misc_id = battle_get_misc_id(unit_id);
    if (misc_id == EVENT_MISC_ID_NONE) {
        battle_thread_exit_current();
    }
    battle_unit_update_staged_status_data(unit_id, 0);
    unit = battle_unit_get_stats_from_battle_id(battle_unit_index);
    if (unit->status_sets.current[0] & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD)) {
        state = 0;
    } else {
        state = 2;
        if (unit->status_sets.current[2] & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRITICAL)) {
            state = 1;
        }
    }
    if (mode == 2) {
        battle_status_inflict_by_entd_unit_id(unit_id, BATTLE_STATUS_LSB_INDEX(BATTLE_STATUS_ID_POISON), 1, 1);
        g_battle_unit_status_staging_data->exit_mode[battle_unit_index] = 3;
        g_battle_thread_call_target = (void (*)(void))battle_unit_update_graphics_by_misc_id_wrapper;
        battle_thread_call_on_main_stack(misc_id);
        battle_unit_set_specific_animation_value_on_battle_init(misc_id, 0x16);
    } else if (mode == 1) {
        battle_status_inflict_by_entd_unit_id(unit_id, BATTLE_STATUS_LSB_INDEX(BATTLE_STATUS_ID_DEAD), 0, 1);
        battle_status_inflict_by_entd_unit_id(unit_id, BATTLE_STATUS_LSB_INDEX(BATTLE_STATUS_ID_CRITICAL), 0, 1);
        battle_status_inflict_by_entd_unit_id(unit_id, BATTLE_STATUS_LSB_INDEX(BATTLE_STATUS_ID_CRYSTAL), 1, 1);
        g_battle_unit_status_staging_data->exit_mode[battle_unit_index] = 2;
        g_battle_thread_call_target = (void (*)(void))battle_unit_update_graphics_by_misc_id_wrapper;
        battle_thread_call_on_main_stack(misc_id);
    } else if (mode == 0) {
        if (state == 0) {
            g_battle_unit_status_staging_data->exit_mode[battle_unit_index] = 1;
            battle_status_inflict_by_entd_unit_id(unit_id, BATTLE_STATUS_LSB_INDEX(BATTLE_STATUS_ID_DEAD), 0, 1);
            unit->hp = 1;
            battle_status_inflict_by_entd_unit_id(unit_id, BATTLE_STATUS_LSB_INDEX(BATTLE_STATUS_ID_CRITICAL), 1, 1);
            g_battle_thread_call_target = (void (*)(void))battle_unit_update_graphics_by_misc_id_wrapper;
            battle_thread_call_on_main_stack(misc_id);
            g_sound_effect_id_to_play = 0x41;
        } else if (state == 2) {
            g_battle_thread_call_target = (void (*)(void))battle_unit_set_mounted_animation_by_misc_id;
            battle_thread_call_on_main_stack(misc_id);
        }
    }
    if (wait_frames != 0) {
        battle_thread_wait_frames(wait_frames);
    }
    battle_thread_exit_current();
}
