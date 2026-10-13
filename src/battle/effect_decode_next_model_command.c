#include "fft/battle_effect.h"
#include "psx/libc.h"
#include "psx/types.h"

/* Decode the next effect command and advance the command index.
 *
 * Opcodes 0x31 and 0x39 carry three or four colors plus vertex indices; 0x35 and
 * 0x37 add five texture operands. Any other opcode is skipped by its length byte.
 *
 * The 0x35/0x37 stream uses variable-length commands, so its index is a 32-bit-element
 * offset rather than a psyq_tmd_packet_t array index. */
void effect_decode_next_model_command(effect_decoded_model_command_t* out) {
    psyq_tmd_packet_t* command;
    s32 index;
    effect_decoded_model_command_t* dst;
    u32 header;

    dst = out;
    index = g_effect_model_command_word_offset;
    command = (psyq_tmd_packet_t*)((u8*)g_effect_model_commands + index * sizeof(u32));
    header = command->id;
    memcpy(&out->header, &command->id, sizeof(out->header));
    /* Struct assignments use unaligned copies; memcpy preserves the 32-bit transfers.
     * The cached header preserves dispatch load/store order and the unsigned range test. */
    switch (header >> 24) {
    case EFFECT_MODEL_COMMAND_G4:
        memcpy(&out->colors[0], &command->g4.r0, sizeof(out->colors[0]));
        memcpy(&out->colors[1], &command->g4.r1, sizeof(out->colors[1]));
        memcpy(&out->colors[2], &command->g4.r2, sizeof(out->colors[2]));
        memcpy(&out->colors[3], &command->g4.r3, sizeof(out->colors[3]));
        memcpy(&out->vertices.pairs[0], &command->g4.v0, sizeof(out->vertices.pairs[0]));
        /* A 32-bit load preserves the native delay-slot store; memcpy reschedules it. */
        out->vertices.pairs[1] = *(u32*)&command->g4.v2;
        g_effect_model_command_word_offset = index + 7;
        break;
    case EFFECT_MODEL_COMMAND_G3:
        memcpy(&out->colors[0], &command->g3.r0, sizeof(out->colors[0]));
        memcpy(&out->colors[1], &command->g3.r1, sizeof(out->colors[1]));
        memcpy(&out->colors[2], &command->g3.r2, sizeof(out->colors[2]));
        memcpy(&out->vertices.pairs[0], &command->g3.v0, sizeof(out->vertices.pairs[0]));
        /* A 32-bit load preserves the native delay-slot store; memcpy reschedules it. */
        out->vertices.pairs[1] = *(u32*)&command->g3.v2;
        g_effect_model_command_word_offset = index + 6;
        break;
    case EFFECT_MODEL_COMMAND_GT3_35:
    case EFFECT_MODEL_COMMAND_GT3_37: {
        psyq_tmd_packet_t* extended;
        extended
            = (psyq_tmd_packet_t*)((u8*)g_effect_model_commands + g_effect_model_command_word_offset * sizeof(u32));
        memcpy(&dst->colors[0], &extended->gt3.r0, sizeof(dst->colors[0]));
        memcpy(&dst->colors[1], &extended->gt3.r1, sizeof(dst->colors[1]));
        memcpy(&dst->colors[2], &extended->gt3.r2, sizeof(dst->colors[2]));
        memcpy(&dst->vertices.pairs[0], &extended->gt3.v0, sizeof(dst->vertices.pairs[0]));
        memcpy(&dst->vertices.pairs[1], &extended->gt3.v2, sizeof(dst->vertices.pairs[1]));
        out->texture.tpage = extended->gt3.tpage;
        out->texture.clut = extended->gt3.clut;
        memcpy(&out->texture.uv[0], &extended->gt3.tu0, sizeof(out->texture.uv[0]));
        memcpy(&out->texture.uv[1], &extended->gt3.tu1, sizeof(out->texture.uv[1]));
        memcpy(&out->texture.uv[2], &extended->gt3.tu2, sizeof(out->texture.uv[2]));
        g_effect_model_command_word_offset += 9;
        break;
    }
    default:
        g_effect_model_command_word_offset += out->header.word_count + 1;
        break;
    }
}
