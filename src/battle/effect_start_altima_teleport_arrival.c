#include "fft/battle_effect.h"
#include "fft/battle_gfx.h"
#include "fft/data.h"
#include "fft/main.h"

/* Start the arrival effect of a teleport. Altima's two forms request their own
 * ability effect and the result is 1; any other unit gets the teleport secondary
 * effect and the teleport-in sound, and the result is 0. */
s32 effect_start_altima_teleport_arrival(battle_unit_misc_data_t* unit) {
    effect_secondary_init_t teleport_data;

    effect_init_altima_teleport_data(unit, &teleport_data);
    switch (unit->spritesheet_id) {
    case BATTLE_SPRITESHEET_ID_ALTIMA_FIRST_FORM:
        effect_set_ability_animation(0, ABILITY_ID_ALTIMA_HOLY_ANGEL_TELEPORT_ARRIVAL, &teleport_data);
        return 1;
    case BATTLE_SPRITESHEET_ID_ALTIMA_SECOND_FORM:
        effect_set_ability_animation(0, ABILITY_ID_ALTIMA_ARCH_ANGEL_TELEPORT_ARRIVAL, &teleport_data);
        return 1;
    }
    effect_set_secondary_teleport(unit);
    main_sound_play_sfx_find_channel(MAIN_SFX_TELEPORT_IN);
    return 0;
}
