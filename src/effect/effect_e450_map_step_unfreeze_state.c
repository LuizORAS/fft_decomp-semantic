/* E450.BIN is the effect of effect id 0x218, one of the ids past the abilities. */
#include "fft/effect.h"

/* Callback handler: unfreezes the map when its keyframe starts. */
s32 effect_e450_map_step_unfreeze_state(s16 record_index, s32 callback_slot) {
    effect_record_t* record;

    record = &g_effect_state_records[record_index];

    switch (record->callback_phase[callback_slot]) {
    case EFFECT_PHASE_IDLE:
        break;
    case EFFECT_PHASE_INIT:
        battle_map_unfreeze();
        record->callback_phase[callback_slot] = EFFECT_PHASE_UPDATE;
        break;
    case EFFECT_PHASE_UPDATE:
        break;
    case EFFECT_PHASE_DESTROY:
        record->callback_phase[callback_slot] = EFFECT_PHASE_IDLE;
        break;
    }
}
