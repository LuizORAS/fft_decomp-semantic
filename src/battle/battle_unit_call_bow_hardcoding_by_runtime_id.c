#include "fft/battle_unit.h"

/* Same as battle_unit_resolve_projectile_hit for two runtime ids: the arrow-arc and linear projectile
 * effects call it for the unit the projectile hits. */
void battle_unit_call_bow_hardcoding_by_runtime_id(u16 attacker_id, u16 target_id) {
    battle_unit_t* attacker = battle_unit_get_by_runtime_id(attacker_id);
    battle_unit_t* target = battle_unit_get_by_runtime_id(target_id);

    battle_unit_resolve_projectile_hit(attacker, target);
}
