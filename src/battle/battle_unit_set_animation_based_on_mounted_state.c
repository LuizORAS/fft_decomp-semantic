#include "fft/battle.h"

void battle_unit_set_animation_based_on_mounted_state(battle_unit_misc_data_t* unit) {
    s32 anim;
    s32 s1v;
    s32 water_depth;
    battle_unit_misc_data_t* other;

    anim = BATTLE_UNIT_ANIMATION_RIDING;
    if (unit->mount_state != BATTLE_MISC_MOUNT_STATE_RIDER) {
        water_depth = battle_move_get_water_depth(unit) & 0xFF;
        if (water_depth < 0) {
            s1v = BATTLE_UNIT_ANIMATION_SUBMERGED_IDLE;
        } else if (water_depth < 2) {
            s1v = BATTLE_UNIT_ANIMATION_STAND;
            if (unit->mount_state == BATTLE_MISC_MOUNT_STATE_MOUNT) {
                other = battle_unit_get_misc_data_by_misc_id(unit->mount_partner_misc_id);
                if (other != 0) {
                    battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_RIDING, unit->facing, other);
                }
            }
        } else {
            s1v = BATTLE_UNIT_ANIMATION_SUBMERGED_IDLE;
        }
        if (s1v == 0) {
            return;
        }
        anim = s1v;
    }
    battle_unit_store_animation_facing(anim, unit->facing, unit);
}
