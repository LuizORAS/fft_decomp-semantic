#include "fft/battle.h"
#include "psx/types.h"

/* Start the charging variant of the ability's charge animation set, the pose held while the unit
 * charges or performs, or the submerged stance (9) in deep water; nothing when the set has none.
 * Committing a command, the start of a charged attack and the Charging and Performing stances use
 * it. */
void battle_unit_start_ability_charging_animation(battle_unit_misc_data_t* unit) {
    u8 animation;
    u8 movement_type;
    s32 selected_animation;

    animation = g_battle_ability_charge_animation_sets[g_battle_ability_animation_data[unit->used_ability_id]
            .charge_animation_set_id][BATTLE_CHARGE_ANIMATION_CHARGING];
    if (animation != 0) {
        movement_type = battle_move_get_water_animation_mode(unit);
        selected_animation = 9;
        if (movement_type < 2) {
            selected_animation = animation;
        }
        battle_unit_store_animation_facing(selected_animation, unit->facing, unit);
    }
}
