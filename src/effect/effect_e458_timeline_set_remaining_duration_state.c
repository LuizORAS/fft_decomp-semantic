/* E458.BIN is the effect of effect id 0x201, one of the ids past the abilities. */
#include "fft/effect.h"

/* Callback handler: when its keyframe starts, gives map command 0x96 the
 * frames left of the record's child timeline. */
s32 effect_e458_timeline_set_remaining_duration_state(s16 record_index, s32 callback_slot) {
    effect_record_t* record = &g_battle_effect_state_records[record_index];

    switch (record->callback_phase[callback_slot]) {
    case EFFECT_PHASE_IDLE:
        break;
    case EFFECT_PHASE_INIT:
        battle_map_set_command_0x96_duration(0x96, g_battle_effect_timing_channels->duration - record->timeline_frame);
        record->callback_phase[callback_slot] = EFFECT_PHASE_UPDATE;
        break;
    case EFFECT_PHASE_UPDATE:
        break;
    case EFFECT_PHASE_DESTROY:
        record->callback_phase[callback_slot] = EFFECT_PHASE_IDLE;
        break;
    }
}
