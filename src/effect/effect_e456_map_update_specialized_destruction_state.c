#include "fft/battle_effect.h"
#include "fft/battle_map.h"
#include "fft/battle_script.h"
/* E456.BIN is the effect of effect id 0x203, one of the ids past the abilities. */
#include "fft/effect.h"

/* Effect callback of the map's specialized destruction: its init call releases
 * the hold on the GNS dispatch, then each update switches the map into its
 * specialized destroyed state. */
s32 effect_e456_map_update_specialized_destruction_state(
    s32 record_index, s32 callback_slot, s32 unused_emitter_index, s32 unused_frame) {
    s16 state_index = record_index;
    battle_effect_record_t* record = &g_battle_effect_state_records[state_index];

    switch (record->callback_phase[callback_slot]) {
    case EFFECT_CALLBACK_PHASE_IDLE:
        break;
    case EFFECT_CALLBACK_PHASE_INIT:
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_RELEASE_GNS_HOLD, 1, 1, 1);
        record->callback_phase[callback_slot] = EFFECT_CALLBACK_PHASE_UPDATE;
        break;
    case EFFECT_CALLBACK_PHASE_UPDATE:
        battle_script_set_specialized_map_destroyed();
        break;
    case EFFECT_CALLBACK_PHASE_DESTROY:
        record->callback_phase[callback_slot] = EFFECT_CALLBACK_PHASE_IDLE;
        break;
    }
}
