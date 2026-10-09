#include "fft/battle.h"

void battle_unit_decide_facing_direction(battle_unit_misc_data_t* unit, s32 facing) {
    s16* cur = (s16*)&unit->facing;

    switch (facing) {
    case BATTLE_FACING_NORTH:
        if (*cur == facing) {
            return;
        }
        *cur = BATTLE_FACING_NORTH;
        break;
    case BATTLE_FACING_SOUTH:
        if (*cur == BATTLE_FACING_SOUTH) {
            return;
        }
        *cur = BATTLE_FACING_SOUTH;
        break;
    case BATTLE_FACING_WEST:
        if (*cur == facing) {
            return;
        }
        *cur = BATTLE_FACING_WEST;
        break;
    case BATTLE_FACING_EAST:
        if (*cur == facing) {
            return;
        }
        *cur = BATTLE_FACING_EAST;
        break;
    case 0x200:
    case 0x600:
    case 0xA00:
    default:
        return;
    }
    battle_unit_set_animation_based_on_status(unit);
}
