#include "fft/battle.h"
#include "psx/types.h"

/* Return how many of the four tiles next to the unit hold a same-team Monster Skill supporter
 * (battle_menu_has_monster_skill_supporter_at_tile), or 0 for a unit not on the field; nonzero gives a
 * monster its fourth ability. */
s32 battle_menu_init_monster_skill_check(battle_stats_t* unit) {
    battle_unit_height_data_t height_data;
    s32 team;
    s32 x;
    s32 y;
    s32 height;
    s32 sum;

    if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE) {
        return 0;
    }
    battle_unit_calculate_height_data(&height_data, unit->misc_unit_id);
    team = unit->team_flags;
    x = unit->x;
    y = unit->position.bits.y;
    height = height_data.total_height;
    sum = battle_menu_has_monster_skill_supporter_at_tile(team, x - 1, y, height)
        + battle_menu_has_monster_skill_supporter_at_tile(team, x + 1, y, height);
    sum += battle_menu_has_monster_skill_supporter_at_tile(team, x, y - 1, height);
    return sum + battle_menu_has_monster_skill_supporter_at_tile(team, x, y + 1, height);
}
