#include "fft/battle_unit.h"

/* Same as battle_unit_resolve_projectile_hit for two battle ids. No retail code calls it. */
void battle_unit_resolve_projectile_hit_by_battle_ids(u16 attacker_id, u16 target_id) {
    battle_unit_t* attacker = battle_unit_get_by_battle_id(attacker_id);
    battle_unit_t* target = battle_unit_get_by_battle_id(target_id);

    battle_unit_resolve_projectile_hit(attacker, target);
}
