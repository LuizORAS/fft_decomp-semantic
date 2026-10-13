#include "fft/battle_script.h"
#include "fft/battle_sound.h"
#include "fft/battle_status.h"
#include "fft/battle_thread.h"
#include "fft/battle_unit.h"
#include "fft/main.h"
#include "fft/script.h"
#include "fft/thread.h"
#include "fft/unit.h"
#include "psx/types.h"

/* Battle twin of world_script_inflict_status_thread: Inflict Status event command thread.
 *
 * Parameters: ENTD unit id 16-bit value, mode byte, then a wait-frame 16-bit value.
 * Mode 0 revives a dead unit at 1 HP with Critical (or refreshes its mounted
 * animation when it is neither dead nor critical). Mode 1 clears Dead/Critical
 * and inflicts Crystal. Mode 2 inflicts Poison with the kneeling animation. Each mode
 * records the matching staged exit mode for the later status-application pass. */
void script_inflict_status_thread(void) {
    event_inflict_status_parameters_t* parameters;
    s32 unit_id;
    s32 mode;
    s32 wait_frames;
    s32 battle_unit_index;
    s32 runtime_id;
    battle_stats_t* unit;
    s32 state;

    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_INFLICT_STATUS);
    parameters = (event_inflict_status_parameters_t*)battle_thread_get_current_parameter_1();
    unit_id = script_read_s16(parameters->unit_id_le);
    mode = parameters->mode;
    wait_frames = script_read_s16(parameters->wait_frames_le);
    battle_unit_index = battle_unit_get_battle_index_by_entd_unit_id(unit_id);
    runtime_id = script_resolve_runtime_id(unit_id);
    if (runtime_id == EVENT_RUNTIME_ID_NONE) {
        battle_thread_exit_current();
    }
    battle_unit_update_staged_status_data(unit_id, 0);
    unit = battle_unit_get_stats_from_battle_id(battle_unit_index);
    if (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_DEAD)] & STATUS_BYTE_MASK(STATUS_ID_DEAD)) {
        state = 0;
    } else {
        state = 2;
        if (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_CRITICAL)] & STATUS_BYTE_MASK(STATUS_ID_CRITICAL)) {
            state = 1;
        }
    }
    if (mode == EVENT_INFLICT_STATUS_POISON) {
        status_inflict_by_entd_unit_id(unit_id, STATUS_LSB_INDEX(STATUS_ID_POISON), 1, 1);
        g_battle_unit_status_staging_data->inflict_status_mode[battle_unit_index] = UNIT_INFLICT_STATUS_MODE_POISON;
        g_thread_call_target = (void (*)(void))battle_unit_update_graphics_by_runtime_id;
        thread_call_on_main_stack(runtime_id);
        battle_unit_set_animation(runtime_id, BATTLE_UNIT_ANIMATION_KNEEL);
    } else if (mode == EVENT_INFLICT_STATUS_CRYSTAL) {
        status_inflict_by_entd_unit_id(unit_id, STATUS_LSB_INDEX(STATUS_ID_DEAD), 0, 1);
        status_inflict_by_entd_unit_id(unit_id, STATUS_LSB_INDEX(STATUS_ID_CRITICAL), 0, 1);
        status_inflict_by_entd_unit_id(unit_id, STATUS_LSB_INDEX(STATUS_ID_CRYSTAL), 1, 1);
        g_battle_unit_status_staging_data->inflict_status_mode[battle_unit_index] = UNIT_INFLICT_STATUS_MODE_CRYSTAL;
        g_thread_call_target = (void (*)(void))battle_unit_update_graphics_by_runtime_id;
        thread_call_on_main_stack(runtime_id);
    } else if (mode == EVENT_INFLICT_STATUS_REVIVE) {
        if (state == 0) {
            g_battle_unit_status_staging_data->inflict_status_mode[battle_unit_index] = UNIT_INFLICT_STATUS_MODE_REVIVE;
            status_inflict_by_entd_unit_id(unit_id, STATUS_LSB_INDEX(STATUS_ID_DEAD), 0, 1);
            unit->hp = 1;
            status_inflict_by_entd_unit_id(unit_id, STATUS_LSB_INDEX(STATUS_ID_CRITICAL), 1, 1);
            g_thread_call_target = (void (*)(void))battle_unit_update_graphics_by_runtime_id;
            thread_call_on_main_stack(runtime_id);
            g_sound_pending_sfx_id = MAIN_SFX_EVENT_REVIVE;
        } else if (state == 2) {
            g_thread_call_target = (void (*)(void))battle_unit_set_mounted_animation_by_runtime_id;
            thread_call_on_main_stack(runtime_id);
        }
    }
    if (wait_frames != 0) {
        battle_thread_wait_frames(wait_frames);
    }
    battle_thread_exit_current();
}
