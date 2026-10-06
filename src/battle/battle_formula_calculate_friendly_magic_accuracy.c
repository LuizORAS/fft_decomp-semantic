#include "fft/battle.h"
#include "psx/types.h"

/* Hit chance for supporting spells, from the caller's XA and YA: Magic Attack Up and the zodiac
 * compatibility (no target defenses), then XA + YA scaled by both Faiths and rolled as a
 * percentage. Returns 1 on a miss. */
s32 battle_formula_calculate_friendly_magic_accuracy(void) {
    battle_formula_apply_magic_attack_up();
    battle_formula_apply_zodiac_compatibility();
    battle_formula_store_hit_chance();
    battle_formula_calculate_faith();
    battle_formula_roll_hit_chance();
    return g_battle_action_target_data->hit == 0;
}
