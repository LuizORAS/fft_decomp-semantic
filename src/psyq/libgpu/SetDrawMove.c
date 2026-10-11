/* LIBGPU 80023ea0-80023ebc. */
#include "psx/libgpu.h"

void SetDrawMove(DR_MOVE* primitive) {
    setlen(primitive, PSYQ_GPU_PACKET_WORDS(DR_MOVE));
    setcode(primitive, PSYQ_GPU_CODE_CLEAR_CACHE);
    primitive->code2 = PSYQ_GPU_COMMAND_WORD(PSYQ_GPU_CODE_MOVE_IMAGE);
}
