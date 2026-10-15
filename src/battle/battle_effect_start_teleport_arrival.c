#include "fft/battle.h"

/* Same as battle_effect_start_teleport_departure for the arrival: Altima's arrival abilities, else the
 * teleport secondary effect and the teleport-in sound. */
s32 battle_effect_start_teleport_arrival(battle_unit_misc_data_t* unit) {
    battle_effect_secondary_init_t teleport_data;

    battle_effect_init_teleport_data(unit, &teleport_data);
    switch (unit->spritesheet_id) {
    case BATTLE_SPRITESHEET_ID_ALTIMA_FIRST_FORM:
        battle_effect_set_ability_animation(0, ABILITY_ID_ALTIMA_HOLY_ANGEL_TELEPORT_ARRIVAL, &teleport_data);
        return 1;
    case BATTLE_SPRITESHEET_ID_ALTIMA_SECOND_FORM:
        battle_effect_set_ability_animation(0, ABILITY_ID_ALTIMA_ARCH_ANGEL_TELEPORT_ARRIVAL, &teleport_data);
        return 1;
    }
    battle_effect_set_secondary_teleport(unit);
    main_sound_play_sfx_find_channel(MAIN_SFX_TELEPORT_IN);
    return 0;
}
