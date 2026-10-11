#include "fft/battle.h"
#include "psx/types.h"

/* Unaligned SEQ record: every field is a little-endian byte sequence. The
 * layout is that of battle_gfx_packed_offset_table_t (0x800872ec) with the
 * first four bytes read as two halfwords. */
typedef struct battle_gfx_packed_seq_record {
    u8 header[2][2];    /* 0x000 */
    u8 entry[0x100][4]; /* 0x004 */
    u8 payload_size[2]; /* 0x404 */
    u8 payload[1];      /* 0x406 */
} battle_gfx_packed_seq_record_t;

/* Unpack a unit SEQ file: its two header halfwords (the first sequence of
 * the attack set and the first of the SP2 set) become words and its 256
 * sequence offsets pointers into the sequence data, which is appended at
 * g_battle_gfx_seq_data_cursor. An offset of -1 resolves to the start of
 * that data, the first sequence.
 *
 * Near twin of battle_gfx_resolve_packed_offset_table, which resolves the
 * same record without the two header words. As there, the target keeps a
 * second copy of the record pointer for the payload size (`move t2,a1`). */
void battle_gfx_unpack_unit_seq_data(battle_gfx_native_unit_seq_table_t* destination, const u8* source) {
    battle_gfx_native_unit_seq_table_t* table = destination;
    battle_gfx_packed_seq_record_t* record = (battle_gfx_packed_seq_record_t*)source;
    battle_gfx_packed_seq_record_t* header;
    u8(*entry)[4];
    u32 cursor;
    u32 index;
    u32 payload_size;
    s32 offset;

    header = record;
    cursor = (u32)g_battle_gfx_seq_data_cursor;
    table->header.first_attack_sequence = record->header[0][0] + (record->header[0][1] << 8);
    table->header.first_sp2_sequence = record->header[1][0] + (record->header[1][1] << 8);
    entry = record->entry;
    for (index = 0; index < 0x100; index++) {
        offset = entry[index][0] + (entry[index][1] << 8) + (entry[index][2] << 16) + (entry[index][3] << 24);
        if (offset == -1) {
            offset = 0;
        }
        table->sequences[index] = (u8*)(offset + cursor);
    }

    payload_size = header->payload_size[0] + (header->payload_size[1] << 8);
    for (index = 0; index < payload_size; index++) {
        u8* payload = record->payload;

        g_battle_gfx_seq_data_cursor[index] = payload[index];
    }
    g_battle_gfx_seq_data_cursor += index;
}
