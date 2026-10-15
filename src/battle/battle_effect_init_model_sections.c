#include "fft/battle.h"
#include "psx/types.h"

/* Points the model globals at a projectile model's sections, rewinds its
 * command stream and returns its command count. */
s32 battle_effect_init_model_sections(battle_effect_resource_t* resource, s32 unused_1) {
    g_battle_effect_model_header = &resource->words[0];
    g_battle_effect_model_vertices = &resource->words[resource->words[BATTLE_EFFECT_MODEL_WORD_VERTICES_OFFSET] / 4];
    D_801B8A34 = &resource->words[resource->words[BATTLE_EFFECT_MODEL_WORD_SECOND_SECTION_OFFSET] / 4];
    g_battle_effect_model_commands = &resource->words[resource->words[BATTLE_EFFECT_MODEL_WORD_COMMANDS_OFFSET] / 4];
    g_battle_effect_model_command_index = 0;
    return resource->words[BATTLE_EFFECT_MODEL_WORD_COMMAND_COUNT];
}
