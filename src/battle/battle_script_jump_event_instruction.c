#include "fft/battle.h"
#include "psx/types.h"

void battle_script_jump_event_instruction(battle_script_jump_instruction_t* instr) {
    SVECTOR vec;
    s32 misc_id;
    SVECTOR* coords;
    battle_stats_t* stats;
    u8 saved;

    misc_id = battle_script_get_misc_id(battle_script_load_halfword((const u8*)instr));
    if (misc_id != EVENT_MISC_ID_NONE) {
        coords = (SVECTOR*)battle_unit_get_event_offset_ptr_by_misc_id(misc_id);
        vec.vx = -coords->vx;
        vec.vy = -coords->vy;
        vec.vz = -coords->vz;
        battle_unit_add_event_offset_by_misc_id(misc_id, &vec);
        stats = battle_unit_get_stats_from_battle_id(battle_unit_get_battle_index_by_misc_id(misc_id));
        saved = stats->jump;
        stats->jump = 6;
        battle_unit_prepare_rider_dismount(misc_id, instr->param1, instr->param0 - 1);
        stats->jump = saved;
    }
}
