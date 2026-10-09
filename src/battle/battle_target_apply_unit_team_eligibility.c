#include "fft/battle.h"
#include "psx/types.h"

/* Filters unit-occupied targeting panels by team eligibility.
 *
 * The low byte of `raw_unit_id` selects the acting unit, which is treated as
 * same-team even if its initial and live team fields differ. `allow_allies`
 * and `allow_enemies` are raw nonzero flag values. Unless the ability AoE is
 * `0xff`, every nonempty panel left after filtering is marked selected. */
void battle_target_apply_unit_team_eligibility(s32 raw_unit_id, u8 allow_allies, u8 allow_enemies, u8 aoe_is_0xff) {
    s32 relation;
    u8 team_flags;
    s32 panel_index;
    battle_target_panel_t* panel;
    s32 i;
    s32 actor_id;

    raw_unit_id &= 0xff;
    team_flags = g_battle_unit_stats[raw_unit_id].team_flags;
    actor_id = raw_unit_id;
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        if (battle_target_is_unit_untargetable_and_store_tile(i, &panel_index) == 0) {
            relation = team_flags ^ g_battle_unit_stats[i].initial_team_flags;
            if (actor_id == i) {
                relation = 0;
            }
            panel = &g_battle_target_panels[panel_index];
            if (panel->remaining_range != 0) {
                if (relation & BATTLE_TEAM_MASK) {
                    if (allow_enemies == 0) {
                        panel->remaining_range = 0;
                    } else {
                        panel->mark = 1;
                    }
                } else if (allow_allies != 0) {
                    panel->mark = 1;
                } else {
                    panel->remaining_range = 0;
                }
            }
        }
    }
    if (!aoe_is_0xff) {
        for (i = 0; i < 0x200; i++) {
            panel = &g_battle_target_panels[i];
            if (panel->remaining_range != 0) {
                panel->mark = 1;
            }
        }
    }
}
