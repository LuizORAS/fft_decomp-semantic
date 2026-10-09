#include "fft/battle.h"

enum {
    LEVEL_UP_ANIMATION_WALKING = 0x1c,
    LEVEL_UP_ANIMATION_SMALL_HOPS = 0x1d,
};

void battle_unit_set_level_up_animation(battle_unit_misc_data_t* unit) {
    if (unit->mount_state == BATTLE_MISC_MOUNT_STATE_RIDER) {
        battle_unit_store_animation_facing(LEVEL_UP_ANIMATION_SMALL_HOPS, unit->facing, unit);
        return;
    }
    switch (battle_move_get_water_depth(unit)) {
    case 0:
        battle_unit_store_animation_facing(LEVEL_UP_ANIMATION_WALKING, unit->facing, unit);
        break;
    case 1:
        battle_unit_store_animation_facing(LEVEL_UP_ANIMATION_SMALL_HOPS, unit->facing, unit);
        break;
    case 2:
        battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_SUBMERGED_IDLE, unit->facing, unit);
        break;
    }
}
