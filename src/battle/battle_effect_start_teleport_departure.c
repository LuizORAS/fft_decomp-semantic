#include "fft/battle_effect.h"
#include "fft/battle_gfx.h"
#include "fft/data.h"
#include "fft/main.h"

/* Start the departure effect of a teleport: Altima's two forms request their own ability effect
 * (ABILITY_ID_ALTIMA_HOLY_ANGEL_TELEPORT_DEPARTURE, ABILITY_ID_ALTIMA_ARCH_ANGEL_TELEPORT_DEPARTURE) and the
 * result is 1; any other unit gets the teleport secondary effect and MAIN_SFX_TELEPORT_OUT, and the result is
 * 0. battle_unit_animate_teleport_to_destination calls it. */
s32 battle_effect_start_teleport_departure(battle_unit_t* unit) {
    battle_effect_secondary_init_t teleport_data;

    battle_effect_init_teleport_data(unit, &teleport_data);
    switch (unit->spritesheet_id) {
    case BATTLE_SPRITESHEET_ID_ALTIMA_FIRST_FORM:
        battle_effect_request_ability_effect(0, ABILITY_ID_ALTIMA_HOLY_ANGEL_TELEPORT_DEPARTURE, &teleport_data);
        return 1;
    case BATTLE_SPRITESHEET_ID_ALTIMA_SECOND_FORM:
        battle_effect_request_ability_effect(0, ABILITY_ID_ALTIMA_ARCH_ANGEL_TELEPORT_DEPARTURE, &teleport_data);
        return 1;
    }
    battle_effect_set_secondary_teleport(unit);
    main_sound_play_sfx_find_channel(MAIN_SFX_TELEPORT_OUT);
    return 0;
}
