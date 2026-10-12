/* E480.BIN is the effect of effect id 0x220, one of the ids past the abilities. */
#include "fft/effect.h"

/* Callback handler: freezes the map when its keyframe starts. */
s32 effect_e480_map_step_freeze_state(s32 record_index, s32 callback_slot, s32 unused_emitter_index, s32 unused_frame) {
    s16 state_index = record_index;
    effect_record_t* record;

    record = &g_battle_effect_state_records[state_index];

    switch (record->callback_phase[callback_slot]) {
    case EFFECT_PHASE_IDLE:
        break;
    case EFFECT_PHASE_INIT:
        battle_map_freeze();
        record->callback_phase[callback_slot] = EFFECT_PHASE_UPDATE;
        break;
    case EFFECT_PHASE_UPDATE:
        break;
    case EFFECT_PHASE_DESTROY:
        record->callback_phase[callback_slot] = EFFECT_PHASE_IDLE;
        break;
    }
}
