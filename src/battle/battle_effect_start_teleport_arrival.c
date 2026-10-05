#include "fft/battle.h"

enum {
    EFFECT_ALTIMA_HOLY_ANGEL_TELEPORT_ARRIVAL_0B8 = 0xb8,
    EFFECT_ALTIMA_ARCH_ANGEL_TELEPORT_ARRIVAL_028 = 0x28,
};

/* Same as battle_effect_start_teleport_departure for the arrival: Altima's animations 0xb8 and 0x28,
 * else the generic effect and sound 0x6b. */
s32 battle_effect_start_teleport_arrival(battle_unit_misc_data_t* unit) {
    battle_effect_secondary_init_t teleport_data;
    /* Always 0; a literal argument lets GCC merge the two case tails. */
    s32 animation_type;
    s32 effect_id;

    battle_effect_init_teleport_data(unit, &teleport_data);
    switch (unit->spritesheet_id) {
    case BATTLE_SPRITESHEET_ID_ALTIMA_FIRST_FORM:
        animation_type = 0;
        effect_id = EFFECT_ALTIMA_HOLY_ANGEL_TELEPORT_ARRIVAL_0B8;
        break;
    case BATTLE_SPRITESHEET_ID_ALTIMA_SECOND_FORM:
        animation_type = 0;
        effect_id = EFFECT_ALTIMA_ARCH_ANGEL_TELEPORT_ARRIVAL_028;
        break;
    default:
        /* The target places the fallback after the animation tail. */
        goto fallback;
    }
    battle_effect_set_ability_animation(animation_type, effect_id, &teleport_data);
    return 1;

fallback:
    battle_effect_set_secondary_teleport(unit);
    main_sound_play_sfx_find_channel(0x6b);
    return 0;
}
