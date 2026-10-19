#include "fft/battle_status.h"
#include "fft/battle_unit.h"
#include "fft/main.h"
#include "fft/script.h"
#include "fft/sound.h"
#include "fft/thread.h"
#include "fft/unit.h"

/* The event's Inflict Status command as a thread (task THREAD_TASK_INFLICT_STATUS). Its
 * parameter points at the operands: ENTD unit id 16-bit value, mode byte, wait-frame 16-bit value. The unit
 * is staged first (battle_unit_update_staged_status_data). Mode 0 revives a dead unit at 1 HP with
 * Critical (sound 0x41), or refreshes the mounted animation of a unit neither dead nor critical;
 * mode 1 removes Dead and Critical and adds Crystal; mode 2 adds Poison with animation 0x16. Each
 * records the exit mode (1, 2 or 3) that battle_unit_apply_staged_status_data applies when the event
 * ends. Then it waits the frames and exits. The WORLD twin is world_script_inflict_status_thread. */
void script_inflict_status_thread(void) {
    event_inflict_status_parameters_t* parameters;
    s32 unit_id;
    s32 mode;
    s32 wait_frames;
    s32 battle_unit_index;
    s32 runtime_id;
    battle_stats_t* unit;
    s32 state;

    thread_set_current_task_id(THREAD_TASK_INFLICT_STATUS);
    parameters = (event_inflict_status_parameters_t*)battle_thread_get_current_parameter_1();
    unit_id = script_load_halfword(parameters->unit_id_le);
    mode = parameters->mode;
    wait_frames = script_load_halfword(parameters->wait_frames_le);
    battle_unit_index = battle_unit_get_battle_id_by_entd_unit_id(unit_id);
    runtime_id = script_resolve_runtime_id(unit_id);
    if (runtime_id == EVENT_RUNTIME_ID_NONE) {
        thread_exit_current();
    }
    battle_unit_update_staged_status_data(unit_id, 0);
    unit = battle_unit_get_stats_from_battle_id(battle_unit_index);
    if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEAD)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD)) {
        state = 0;
    } else {
        state = 2;
        if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CRITICAL)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRITICAL)) {
            state = 1;
        }
    }
    if (mode == EVENT_INFLICT_STATUS_POISON) {
        status_inflict_by_entd_unit_id(unit_id, BATTLE_STATUS_LSB_INDEX(BATTLE_STATUS_ID_POISON), 1, 1);
        g_battle_unit_status_staging_data->inflict_status_mode[battle_unit_index] = UNIT_INFLICT_STATUS_MODE_POISON;
        g_thread_call_target = (void (*)(void))battle_unit_update_graphics_by_runtime_id;
        thread_call_on_main_stack(runtime_id);
        unit_set_animation(runtime_id, BATTLE_UNIT_ANIMATION_KNEEL);
    } else if (mode == EVENT_INFLICT_STATUS_CRYSTAL) {
        status_inflict_by_entd_unit_id(unit_id, BATTLE_STATUS_LSB_INDEX(BATTLE_STATUS_ID_DEAD), 0, 1);
        status_inflict_by_entd_unit_id(unit_id, BATTLE_STATUS_LSB_INDEX(BATTLE_STATUS_ID_CRITICAL), 0, 1);
        status_inflict_by_entd_unit_id(unit_id, BATTLE_STATUS_LSB_INDEX(BATTLE_STATUS_ID_CRYSTAL), 1, 1);
        g_battle_unit_status_staging_data->inflict_status_mode[battle_unit_index] = UNIT_INFLICT_STATUS_MODE_CRYSTAL;
        g_thread_call_target = (void (*)(void))battle_unit_update_graphics_by_runtime_id;
        thread_call_on_main_stack(runtime_id);
    } else if (mode == EVENT_INFLICT_STATUS_REVIVE) {
        if (state == 0) {
            g_battle_unit_status_staging_data->inflict_status_mode[battle_unit_index] = UNIT_INFLICT_STATUS_MODE_REVIVE;
            status_inflict_by_entd_unit_id(unit_id, BATTLE_STATUS_LSB_INDEX(BATTLE_STATUS_ID_DEAD), 0, 1);
            unit->hp = 1;
            status_inflict_by_entd_unit_id(unit_id, BATTLE_STATUS_LSB_INDEX(BATTLE_STATUS_ID_CRITICAL), 1, 1);
            g_thread_call_target = (void (*)(void))battle_unit_update_graphics_by_runtime_id;
            thread_call_on_main_stack(runtime_id);
            g_sound_pending_sfx_id = MAIN_SFX_EVENT_REVIVE;
        } else if (state == 2) {
            g_thread_call_target = (void (*)(void))battle_unit_set_mounted_animation_by_runtime_id;
            thread_call_on_main_stack(runtime_id);
        }
    }
    if (wait_frames != 0) {
        thread_wait_frames(wait_frames);
    }
    thread_exit_current();
}
