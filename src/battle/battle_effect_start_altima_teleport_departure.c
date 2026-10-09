#include "fft/battle.h"

enum {
    EFFECT_ALTIMA_HOLY_ANGEL_TELEPORT_DEPARTURE_0DB = 0xdb,
    EFFECT_ALTIMA_ARCH_ANGEL_TELEPORT_DEPARTURE_0DC = 0xdc,
};

s32 battle_effect_start_altima_teleport_departure(battle_unit_misc_data_t* unit) {
    battle_effect_secondary_init_t teleport_data;

    battle_effect_init_altima_teleport_data(unit, &teleport_data);
    switch (unit->spritesheet_id) {
    case BATTLE_SPRITESHEET_ID_ALTIMA_FIRST_FORM:
        battle_effect_set_ability_animation(0, EFFECT_ALTIMA_HOLY_ANGEL_TELEPORT_DEPARTURE_0DB, &teleport_data);
        return 1;
    case BATTLE_SPRITESHEET_ID_ALTIMA_SECOND_FORM:
        battle_effect_set_ability_animation(0, EFFECT_ALTIMA_ARCH_ANGEL_TELEPORT_DEPARTURE_0DC, &teleport_data);
        return 1;
    }
    battle_effect_set_secondary_teleport(unit);
    main_sound_play_sfx_find_channel(MAIN_SFX_TELEPORT_OUT);
    return 0;
}
