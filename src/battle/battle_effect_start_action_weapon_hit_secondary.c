#include "fft/battle.h"

struct battle_effect_secondary_init;

/* Start the weapon-hit secondary effect of the unit's action, in the element of its used weapon. is_bow is
 * 1 when battle_unit_resolve_projectile_hit calls it for a projectile and 0 from the unit's SEQ script
 * (battle_gfx_run_unit_seq_script); battle_effect_init_weapon_hit_secondary does not read it. */
void battle_effect_start_action_weapon_hit_secondary(battle_unit_misc_data_t* unit, s32 is_bow) {
    u8 secondary_init[0xc8];
    battle_effect_build_secondary_init_from_action(unit, (struct battle_effect_secondary_init*)secondary_init);
    battle_effect_init_weapon_hit_secondary(g_main_item_weapon_data[unit->used_item_or_weapon_id].element,
        (battle_effect_secondary_init_t*)secondary_init, is_bow);
}
