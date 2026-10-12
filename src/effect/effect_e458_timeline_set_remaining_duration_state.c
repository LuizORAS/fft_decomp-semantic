#include "fft/battle_effect.h"
#include "fft/battle_map.h"
#include "fft/effect.h"

/* E458.BIN is the effect of effect id 0x201, one of the ids past the abilities. */
#include "fft/effect.h"

/* Callback handler: when its keyframe starts, gives map command 0x96 the
 * frames left of the record's child timeline. */
s32 effect_e458_timeline_set_remaining_duration_state(
    s32 record_index, s32 callback_slot, s32 unused_emitter_index, s32 unused_frame) {
    s16 state_index = record_index;
    effect_record_t* record = &g_battle_effect_state_records[state_index];

    switch (record->callback_phase[callback_slot]) {
    case EFFECT_PHASE_IDLE:
        break;
    case EFFECT_PHASE_INIT:
        battle_map_set_command_0x96_duration(BATTLE_MAP_WEATHER_EFFECT_TIMED_PARTICLE_TRANSFORM,
            g_battle_effect_child_timeline->duration - record->timeline.frame);
        record->callback_phase[callback_slot] = EFFECT_PHASE_UPDATE;
        break;
    case EFFECT_PHASE_UPDATE:
        break;
    case EFFECT_PHASE_DESTROY:
        record->callback_phase[callback_slot] = EFFECT_PHASE_IDLE;
        break;
    }
}
