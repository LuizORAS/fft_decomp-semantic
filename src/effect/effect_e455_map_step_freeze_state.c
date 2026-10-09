#include "fft/effect.h"

s32 effect_e455_map_step_freeze_state(s16 record_index, s32 byte_offset) {
    effect_record_t* record = &g_effect_state_records[record_index];

    switch (record->callback_phase[byte_offset]) {
    case EFFECT_PHASE_IDLE:
        break;
    case EFFECT_PHASE_INIT:
        battle_map_freeze();
        record->callback_phase[byte_offset] = EFFECT_PHASE_UPDATE;
        break;
    case EFFECT_PHASE_UPDATE:
        break;
    case EFFECT_PHASE_DESTROY:
        record->callback_phase[byte_offset] = EFFECT_PHASE_IDLE;
        break;
    }
}
