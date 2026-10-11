#include "fft/battle.h"
#include "psx/libc.h"
#include "psx/types.h"

/* One draw command of a projectile model's command section
 * (g_battle_effect_model_commands, see battle_effect_init_resource_sections). The top header
 * byte is the opcode and header byte 1 the word count after the header. */
typedef union battle_effect_model_command {
    u32 header;
    struct {
        u32 header;
        CVECTOR colors[3];   /* 0x04 */
        u32 vertex_pairs[2]; /* 0x10; packed vertex-index pairs */
    } g3;
    struct {
        u32 header;
        CVECTOR colors[4];   /* 0x04 */
        u32 vertex_pairs[2]; /* 0x14 */
    } g4;
    struct {
        u32 header;
        u16 texture[5];      /* 0x04 */
        u16 _padding_0e;     /* aligns colors */
        CVECTOR colors[3];   /* 0x10 */
        u32 vertex_pairs[2]; /* 0x1c; packed vertex-index pairs */
    } gt3;
} battle_effect_model_command_t;

/* Decode the next effect command and advance the command index.
 *
 * Opcodes 0x31 and 0x39 carry three or four colors plus vertex indices; 0x35 and
 * 0x37 add five texture operands. Any other opcode is skipped by its length byte.
 *
 * The 0x35/0x37 stream uses variable-length commands, so its index is a word
 * offset rather than a battle_effect_model_command_t array index. Its texture operands are
 * written through a walking pointer, which keeps the last of them ahead of
 * the command-index update. */
void battle_effect_decode_next_model_command(battle_effect_decoded_model_command_t* out) {
    battle_effect_model_command_t* command;
    s32 index;
    battle_effect_decoded_model_command_t* dst;
    u32 header;

    dst = out;
    index = g_battle_effect_model_command_index;
    command = (battle_effect_model_command_t*)&g_battle_effect_model_commands[index];
    header = command->header;
    out->header.word = header;
    /* Struct assignments use unaligned copies; memcpy preserves the word transfers. */
    switch (header >> 24) {
    case BATTLE_EFFECT_MODEL_COMMAND_G4:
        memcpy(&out->colors[0], &command->g4.colors[0], sizeof(out->colors[0]));
        memcpy(&out->colors[1], &command->g4.colors[1], sizeof(out->colors[1]));
        memcpy(&out->colors[2], &command->g4.colors[2], sizeof(out->colors[2]));
        memcpy(&out->colors[3], &command->g4.colors[3], sizeof(out->colors[3]));
        out->vertices.pairs[0] = command->g4.vertex_pairs[0];
        out->vertices.pairs[1] = command->g4.vertex_pairs[1];
        g_battle_effect_model_command_index = index + 7;
        break;
    case BATTLE_EFFECT_MODEL_COMMAND_G3:
        memcpy(&out->colors[0], &command->g3.colors[0], sizeof(out->colors[0]));
        memcpy(&out->colors[1], &command->g3.colors[1], sizeof(out->colors[1]));
        memcpy(&out->colors[2], &command->g3.colors[2], sizeof(out->colors[2]));
        out->vertices.pairs[0] = command->g3.vertex_pairs[0];
        out->vertices.pairs[1] = command->g3.vertex_pairs[1];
        g_battle_effect_model_command_index = index + 6;
        break;
    case BATTLE_EFFECT_MODEL_COMMAND_GT3_35:
    case BATTLE_EFFECT_MODEL_COMMAND_GT3_37: {
        battle_effect_model_command_t* extended;
        /* Named texture fields reschedule the final store past the index update. */
        u16* texture;

        extended = (battle_effect_model_command_t*)&g_battle_effect_model_commands[g_battle_effect_model_command_index];
        memcpy(&dst->colors[0], &extended->gt3.colors[0], sizeof(dst->colors[0]));
        memcpy(&dst->colors[1], &extended->gt3.colors[1], sizeof(dst->colors[1]));
        memcpy(&dst->colors[2], &extended->gt3.colors[2], sizeof(dst->colors[2]));
        dst->vertices.pairs[0] = extended->gt3.vertex_pairs[0];
        dst->vertices.pairs[1] = extended->gt3.vertex_pairs[1];
        texture = out->texture;
        *texture++ = extended->gt3.texture[3];
        *texture++ = extended->gt3.texture[1];
        *texture++ = extended->gt3.texture[0];
        *texture++ = extended->gt3.texture[2];
        *texture++ = extended->gt3.texture[4];
        g_battle_effect_model_command_index += 9;
        break;
    }
    default:
        g_battle_effect_model_command_index += out->header.fields.word_count + 1;
        break;
    }
}
