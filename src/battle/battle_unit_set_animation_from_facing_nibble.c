#include "fft/battle_unit.h"

/* Faces a unit in a direction given in sixteenths of a turn and restarts its
 * current animation with that facing; an animation below idle becomes standing.
 * Returns the misc id, or -1 when no record has it. */
s32 battle_unit_set_animation_from_facing_nibble(u32 misc_id, s32 facing_nibble) {
    battle_unit_t* unit;
    s32 animation;
    s16 facing;

    unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);
    if (unit != 0) {
        animation = unit->animation.sequence_id >> 1;
        facing = facing_nibble << 8;
        unit->facing = facing;
        if (animation < BATTLE_UNIT_ANIMATION_IDLE) {
            animation = BATTLE_UNIT_ANIMATION_STAND;
        }
        battle_unit_store_animation_facing_movement_data(animation, facing, unit);
        return misc_id;
    }
    return -1;
}
