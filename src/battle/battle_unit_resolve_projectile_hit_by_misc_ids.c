#include "fft/battle.h"

/* Same as battle_unit_resolve_projectile_hit for two misc ids: the arrow-arc and linear projectile
 * effects call it for the unit the projectile hits. */
void battle_unit_resolve_projectile_hit_by_misc_ids(u16 attacker_id, u16 target_id) {
    battle_unit_misc_data_t* attacker = battle_unit_get_misc_data_by_misc_id(attacker_id);
    battle_unit_misc_data_t* target = battle_unit_get_misc_data_by_misc_id(target_id);

    battle_unit_resolve_projectile_hit(attacker, target);
}
