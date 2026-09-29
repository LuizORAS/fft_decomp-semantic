/* SCUS_942.21 0x8001a524..0x8001a9f3. */
#include "psx/libspu.h"

void _spu_setReverbAttr(psyq_spu_reverb_parameters_t* parameters) {
    u32 mask = parameters->mask;
    s32 all = mask == 0;
    if (all || (mask & 0x1)) {
        _spu_RXX->reverb_parameters[0] = parameters->values[0];
    }
    if (all || (mask & 0x2)) {
        _spu_RXX->reverb_parameters[1] = parameters->values[1];
    }
    if (all || (mask & 0x4)) {
        _spu_RXX->reverb_parameters[2] = parameters->values[2];
    }
    if (all || (mask & 0x8)) {
        _spu_RXX->reverb_parameters[3] = parameters->values[3];
    }
    if (all || (mask & 0x10)) {
        _spu_RXX->reverb_parameters[4] = parameters->values[4];
    }
    if (all || (mask & 0x20)) {
        _spu_RXX->reverb_parameters[5] = parameters->values[5];
    }
    if (all || (mask & 0x40)) {
        _spu_RXX->reverb_parameters[6] = parameters->values[6];
    }
    if (all || (mask & 0x80)) {
        _spu_RXX->reverb_parameters[7] = parameters->values[7];
    }
    if (all || (mask & 0x100)) {
        _spu_RXX->reverb_parameters[8] = parameters->values[8];
    }
    if (all || (mask & 0x200)) {
        _spu_RXX->reverb_parameters[9] = parameters->values[9];
    }
    if (all || (mask & 0x400)) {
        _spu_RXX->reverb_parameters[10] = parameters->values[10];
    }
    if (all || (mask & 0x800)) {
        _spu_RXX->reverb_parameters[11] = parameters->values[11];
    }
    if (all || (mask & 0x1000)) {
        _spu_RXX->reverb_parameters[12] = parameters->values[12];
    }
    if (all || (mask & 0x2000)) {
        _spu_RXX->reverb_parameters[13] = parameters->values[13];
    }
    if (all || (mask & 0x4000)) {
        _spu_RXX->reverb_parameters[14] = parameters->values[14];
    }
    if (all || (mask & 0x8000)) {
        _spu_RXX->reverb_parameters[15] = parameters->values[15];
    }
    if (all || (mask & 0x10000)) {
        _spu_RXX->reverb_parameters[16] = parameters->values[16];
    }
    if (all || (mask & 0x20000)) {
        _spu_RXX->reverb_parameters[17] = parameters->values[17];
    }
    if (all || (mask & 0x40000)) {
        _spu_RXX->reverb_parameters[18] = parameters->values[18];
    }
    if (all || (mask & 0x80000)) {
        _spu_RXX->reverb_parameters[19] = parameters->values[19];
    }
    if (all || (mask & 0x100000)) {
        _spu_RXX->reverb_parameters[20] = parameters->values[20];
    }
    if (all || (mask & 0x200000)) {
        _spu_RXX->reverb_parameters[21] = parameters->values[21];
    }
    if (all || (mask & 0x400000)) {
        _spu_RXX->reverb_parameters[22] = parameters->values[22];
    }
    if (all || (mask & 0x800000)) {
        _spu_RXX->reverb_parameters[23] = parameters->values[23];
    }
    if (all || (mask & 0x1000000)) {
        _spu_RXX->reverb_parameters[24] = parameters->values[24];
    }
    if (all || (mask & 0x2000000)) {
        _spu_RXX->reverb_parameters[25] = parameters->values[25];
    }
    if (all || (mask & 0x4000000)) {
        _spu_RXX->reverb_parameters[26] = parameters->values[26];
    }
    if (all || (mask & 0x8000000)) {
        _spu_RXX->reverb_parameters[27] = parameters->values[27];
    }
    if (all || (mask & 0x10000000)) {
        _spu_RXX->reverb_parameters[28] = parameters->values[28];
    }
    if (all || (mask & 0x20000000)) {
        _spu_RXX->reverb_parameters[29] = parameters->values[29];
    }
    if (all || (mask & 0x40000000)) {
        _spu_RXX->reverb_parameters[30] = parameters->values[30];
    }
    if (all || (mask & 0x80000000)) {
        _spu_RXX->reverb_parameters[31] = parameters->values[31];
    }
}
