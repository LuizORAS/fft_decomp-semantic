/* E453.BIN is the effect of effect id 0x221, one of the ids past the abilities. */
#include "fft/effect.h"

/* Callback handler: when its keyframe starts, splits the map's mesh parts and
 * sets 3D objects 1-8 to state 3. */
s32 effect_e453_map_split_mesh_and_set_3d_objects_to_state_3_state(s16 record_index, s32 callback_slot) {
    effect_record_t* record = &g_effect_state_records[record_index];

    switch (record->callback_phase[callback_slot]) {
    case EFFECT_PHASE_IDLE:
        break;
    case EFFECT_PHASE_INIT:
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SPLIT_MESH_PARTS, 1, 1, 1);
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 1, 3, 1);
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 2, 3, 1);
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 3, 3, 1);
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 4, 3, 1);
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 5, 3, 1);
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 6, 3, 1);
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 7, 3, 1);
        battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, 8, 3, 1);
        record->callback_phase[callback_slot] = EFFECT_PHASE_UPDATE;
        break;
    case EFFECT_PHASE_UPDATE:
        break;
    case EFFECT_PHASE_DESTROY:
        record->callback_phase[callback_slot] = EFFECT_PHASE_IDLE;
        break;
    }
}
