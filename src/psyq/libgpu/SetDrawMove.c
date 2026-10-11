/* LIBGPU 80023ea0-80023ebc. */
#include "psx/libgpu.h"

void SetDrawMove(DR_MOVE* primitive) {
    ((P_TAG*)primitive)->len = PSYQ_GPU_PACKET_WORDS(DR_MOVE);
    ((P_TAG*)primitive)->code = PSYQ_GPU_CODE_CLEAR_CACHE;
    primitive->code[1] = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_MOVE_IMAGE);
}
