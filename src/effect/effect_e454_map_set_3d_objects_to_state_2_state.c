/* E454.BIN is the effect of effect id 0x219, one of the ids past the abilities. */
#include "fft/effect.h"

/* Callback handler: when its keyframe starts, sets the map's 3D objects 1-6 to
 * state 2. */
s32 effect_e454_map_set_3d_objects_to_state_2_state(
    s32 record_index, s32 callback_slot, s32 unused_emitter_index, s32 unused_frame) {
    s16 state_index = record_index;
    effect_record_t* record = &g_battle_effect_state_records[state_index];

    switch (record->callback_phase[callback_slot]) {
    case EFFECT_PHASE_IDLE:
        break;
    case EFFECT_PHASE_INIT:
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 1, 2, 1);
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 2, 2, 1);
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 3, 2, 1);
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 4, 2, 1);
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 5, 2, 1);
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 6, 2, 1);
        record->callback_phase[callback_slot] = EFFECT_PHASE_UPDATE;
        break;
    case EFFECT_PHASE_UPDATE:
        break;
    case EFFECT_PHASE_DESTROY:
        record->callback_phase[callback_slot] = EFFECT_PHASE_IDLE;
        break;
    }
}
