#include "fft/battle.h"
#include "psx/types.h"

void battle_action_set_damage_display_type_based_on_ability(void) {
    battle_unit_misc_data_t* misc;
    u16 raw;
    s32 ability;
    s32 target;

    g_battle_state_vsync_interval = 1;
    misc = battle_unit_get_casting_misc_data();
    battle_action_resolve_ability_strike(misc->battle_data->misc_unit_id, (battle_strike_work_t*)&misc->action_18c);
    raw = misc->last_attack_id;
    target = misc->reaction_id_1a6;
    ability = raw & 0xffff;
    misc->used_ability_id = raw;
    battle_gfx_set_thrown_item_graphic_palette(misc->used_weapon_id, misc);
    if (((target != 0) && (ability != target)) || (ability == ABILITY_ID_KNOCKBACK) || (ability == 0)
        || (battle_effect_load_ability(ability) != 0)) {
        battle_action_set_target_coords_and_attacker_anim();
        return;
    }
    g_battle_game_state = BATTLE_GAME_STATE_START_EFFECT_FILE_OPEN;
    battle_effect_store_targets_and_display_types(ability, misc);
}
