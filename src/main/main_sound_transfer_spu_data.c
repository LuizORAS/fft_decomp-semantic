#include "fft/main.h"
#include "psx/libspu.h"
#include "psx/types.h"

/* Starts an SPU RAM transfer at spu_address once the previous one is done.
 *
 * The low nibble of mode selects it: 1 writes size bytes from data, 2 reads
 * them into data, 3 and 4 read the decoded CD/voice buffers (all, or CD only)
 * with the result in g_main_sound_decoded_data_result. The status bits 0x10-0x40 stay set until
 * the transfer callback clears them. The empty case 5 is part of the
 * target's five-entry jump table. */
void main_sound_transfer_spu_data(u32 spu_address, void* data, u32 size, s32 mode) {
    main_sound_wait_for_spu_transfer(MAIN_SOUND_SPU_TRANSFER_WAIT);
    g_main_sound_driver_flags |= MAIN_SOUND_DRIVER_SPU_TRANSFER;
    SpuSetTransferMode(SPU_TRANSFER_BY_DMA);
    SpuSetTransferStartAddr(spu_address);
    switch (mode & 0xf) {
    case MAIN_SOUND_SPU_TRANSFER_WRITE:
        g_main_sound_driver_flags |= MAIN_SOUND_DRIVER_SPU_WRITE | MAIN_SOUND_DRIVER_SPU_TRANSFER;
        SpuWrite(data, size);
        break;
    case MAIN_SOUND_SPU_TRANSFER_READ:
        g_main_sound_driver_flags |= MAIN_SOUND_DRIVER_SPU_READ | MAIN_SOUND_DRIVER_SPU_TRANSFER;
        SpuRead(data, size);
        break;
    case MAIN_SOUND_SPU_TRANSFER_READ_DECODED:
        g_main_sound_driver_flags |= MAIN_SOUND_DRIVER_SPU_READ | MAIN_SOUND_DRIVER_SPU_TRANSFER;
        g_main_sound_decoded_data_result = SpuReadDecodedData(data, SPU_ALL);
        break;
    case MAIN_SOUND_SPU_TRANSFER_READ_DECODED_CD:
        g_main_sound_driver_flags |= MAIN_SOUND_DRIVER_SPU_READ | MAIN_SOUND_DRIVER_SPU_TRANSFER;
        g_main_sound_decoded_data_result = SpuReadDecodedData(data, SPU_CDONLY);
        break;
    case MAIN_SOUND_SPU_TRANSFER_NONE:
        break;
    }
}
