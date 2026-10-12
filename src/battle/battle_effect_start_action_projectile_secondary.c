#include "fft/battle_effect.h"
#include "fft/data.h"

/* Starts the projectile secondary effect of the unit's action: the stone for
 * Throw Stone, the thrown item for the Item and Throw abilities, and for
 * anything else the animation of the equipped weapon's type (an arrow for
 * bows and crossbows, none for the rest). The target retains a redundant
 * Shuriken test before checking the complete Item/Throw span. */
void battle_effect_start_action_projectile_secondary(battle_unit_misc_data_t* unit) {
    battle_effect_secondary_init_t secondary_init;
    u16 ability_id;
    u8 animation_id;
    battle_effect_build_secondary_init_from_action(unit, &secondary_init);
    ability_id = unit->used_ability_id;
    if (ability_id == ABILITY_ID_BASIC_SKILL_THROW_STONE) {
        animation_id = BATTLE_EFFECT_SECONDARY_ANIMATION_THROWN_STONE;
    } else {
        animation_id = BATTLE_EFFECT_SECONDARY_ANIMATION_THROWN_ITEM;
        if ((ability_id != ABILITY_ID_THROW_SHURIKEN)
            && ((u32)(unit->used_ability_id - ABILITY_ID_ITEM_FIRST)
                >= (ABILITY_ID_JUMP_FIRST - ABILITY_ID_ITEM_FIRST))) {
            animation_id = g_battle_effect_weapon_type_extra_flags[unit->equipped_weapon_type];
        }
    }
    battle_effect_init_projectile_secondary(animation_id, &secondary_init);
}
