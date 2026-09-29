/* SCUS_942.21 0x800212ec..0x80021c5b. */
#include "psx/libcd.h"
/* These source-local views preserve original reloads after descriptor publication. */
extern volatile s32 stream_emulation_base __asm__("g_psyq_cd_stream_emulation_base");
extern psyq_cd_ring_record_t* volatile current_header_address __asm__("g_psyq_cd_stream_current_header");

/* DMA writes the sector header; the original rereads individual halfwords. */
#define CURRENT_HEADER (*(volatile psyq_cd_ring_record_t*)g_psyq_cd_stream_current_header)
/* Retail callers pass an ignored fourth argument to this three-argument word copy. */
typedef void (*cd_stream_copy_t)(u32*, u32*, u32, s32);
void StCdInterrupt(void) {
    /* Volatile keeps the original extra response-halfword store and reload. */
    volatile u16 response_status[3];
    CdlLOC sector_position;
    u8 response[8];
    u8* position_byte;
    u32 index;
    u32 dma_control;
    s32 emulation_base;
    if (g_psyq_cd_stream_frame_dma_pending == 1)
        return;
    if (g_psyq_cd_stream_mode && (*g_psyq_cd_stream_mdec_out_dma_control_register & 0x01000000)) {
        g_psyq_cd_stream_interrupt_pending = 1;
        if (stream_emulation_base)
            g_psyq_cd_stream_emulation_sector_index++;
        g_psyq_cd_stream_diagnostic_code = 1;
        return;
    }
    if (CdReady(1, response) == CdlDiskError)
        return;
    response_status[1] = response[0];
    response_status[2] = response[1];
    if (response_status[1] & 4) {
        g_psyq_cd_stream_diagnostic_code = 3;
        return;
    }
    g_psyq_cd_stream_current_header = &g_psyq_cd_stream_ring[g_psyq_cd_stream_write_index];
    /* Publish the original current-header address before inspecting the descriptor. */
    __asm__ volatile("");
    if (CURRENT_HEADER.status) {
        if (stream_emulation_base)
            g_psyq_cd_stream_emulation_sector_index++;
        g_psyq_cd_stream_diagnostic_code = 4;
        return;
    }
    *g_psyq_cd_stream_index_register = 0;
    *g_psyq_cd_stream_request_register = 0;
    *g_psyq_cd_stream_index_register = 0;
    *g_psyq_cd_stream_request_register = 0x80;
    *g_psyq_cd_stream_access_delay_register = 0x20943;
    *g_psyq_cd_stream_common_delay_register = 0x1323;
    if (!g_psyq_cd_stream_skip_sector_position) {
        position_byte = (u8*)&sector_position;
        do {
            *position_byte++ = *g_psyq_cd_stream_data_register;
        } while (position_byte < (u8*)(&sector_position + 1));
        for (index = 0; index < PSYQ_CD_STREAM_HEADER_WORDS; index++)
            *g_psyq_cd_stream_data_register;
    }
    dma_control = 0x11000000;
    if ((emulation_base = stream_emulation_base) != 0)
        ((cd_stream_copy_t)mem2mem)((u32*)g_psyq_cd_stream_current_header,
            (u32*)((g_psyq_cd_stream_emulation_sector_index << 11) + emulation_base), PSYQ_CD_STREAM_HEADER_WORDS, 0);
    else
        dma_execute(PSYQ_CD_DMA_CHANNEL, (u32*)g_psyq_cd_stream_current_header, 0, PSYQ_CD_STREAM_HEADER_WORDS,
            dma_control, 0, 0);
    while (*g_psyq_cd_stream_dma_control_register & 0x01000000) { }
    g_psyq_cd_stream_current_header->position = sector_position;
    *g_psyq_cd_stream_access_delay_register = 0x20843;
    *g_psyq_cd_stream_common_delay_register = 0x1325;
    if (g_psyq_cd_stream_start_mask == 1) {
        s32 required_frame = g_psyq_cd_stream_start_frame;
        if (required_frame) {
            /* The original comparison reads the low halfword of the frame word. */
            if (required_frame != *(volatile u16*)&CURRENT_HEADER.frame_count) {
                CURRENT_HEADER.status = StFREE;
                if (stream_emulation_base)
                    g_psyq_cd_stream_emulation_sector_index++;
                return;
            }
            g_psyq_cd_stream_start_mask = 0;
        }
    }
    if (CURRENT_HEADER.status != PSYQ_CD_STREAM_HEADER_MAGIC
        || ((CURRENT_HEADER.type >> 10) & 0x1f) != g_psyq_cd_stream_current_channel) {
        if (stream_emulation_base)
            g_psyq_cd_stream_emulation_sector_index = 0;
        else
            *(volatile u16*)&CURRENT_HEADER.status;
        g_psyq_cd_stream_diagnostic_code = 5;
        CURRENT_HEADER.status = StFREE;
        return;
    }
    if (g_psyq_cd_stream_expected_sector_index != CURRENT_HEADER.sector_index
        || (g_psyq_cd_stream_frame_count
            && g_psyq_cd_stream_frame_count != *(volatile u16*)&CURRENT_HEADER.frame_count)) {
        g_psyq_cd_stream_frame_count = 0;
        g_psyq_cd_stream_expected_sector_index = 0;
        init_ring_status(
            g_psyq_cd_stream_frame_start_index, g_psyq_cd_stream_write_index - g_psyq_cd_stream_frame_start_index);
        {
            u32 next = g_psyq_cd_stream_frame_start_index;
            volatile psyq_cd_ring_record_t* header;
            /* Keep the rollback index load before the current-header load. */
            __asm__("" : "=r"(next) : "0"(next));
            header = g_psyq_cd_stream_current_header;
            /* Keep the header load before restoring the producer index. */
            __asm__("" : "=r"(header) : "0"(header));
            g_psyq_cd_stream_write_index = next;
            /* Keep producer publication before clearing the descriptor. */
            __asm__ volatile("");
            header->status = StFREE;
        }
        if (stream_emulation_base)
            g_psyq_cd_stream_emulation_sector_index++;
        g_psyq_cd_stream_diagnostic_code = 6;
        return;
    }
    if (CURRENT_HEADER.sector_index == 0) {
        u16 frame = *(volatile u16*)&CURRENT_HEADER.frame_count;
        g_psyq_cd_stream_expected_sector_index = 0;
        g_psyq_cd_stream_frame_count = frame;
        if (g_psyq_cd_stream_end_frame && (u32)g_psyq_cd_stream_frame_count >= (u32)g_psyq_cd_stream_end_frame) {
            g_psyq_cd_stream_frame_count = 0;
            g_psyq_cd_stream_expected_sector_index = 0;
            init_ring_status(
                g_psyq_cd_stream_frame_start_index, g_psyq_cd_stream_write_index - g_psyq_cd_stream_frame_start_index);
            {
                u32 next = g_psyq_cd_stream_frame_start_index;
                volatile psyq_cd_ring_record_t* header;
                /* Keep the rollback index load before the current-header load. */
                __asm__("" : "=r"(next) : "0"(next));
                header = g_psyq_cd_stream_current_header;
                /* Keep the header load before restoring the producer index. */
                __asm__("" : "=r"(header) : "0"(header));
                g_psyq_cd_stream_write_index = next;
                /* Keep producer publication before clearing the descriptor. */
                __asm__ volatile("");
                header->status = StFREE;
            }
            g_psyq_cd_stream_start_mask = 1;
            if (g_psyq_cd_stream_end_callback)
                g_psyq_cd_stream_end_callback();
            if (stream_emulation_base)
                g_psyq_cd_stream_emulation_sector_index++;
            g_psyq_cd_stream_diagnostic_code = 7;
            return;
        }
        if ((u32)(g_psyq_cd_stream_ring_sectors - g_psyq_cd_stream_write_index - 1) < CURRENT_HEADER.sector_count) {
            if (!g_psyq_cd_stream_end_frame) {
                CURRENT_HEADER.status = StREWIND;
                /* Keep the rewind descriptor store before the mask update. */
                __asm__ volatile("");
                g_psyq_cd_stream_start_mask = 1;
                if (g_psyq_cd_stream_end_callback)
                    g_psyq_cd_stream_end_callback();
                if (stream_emulation_base)
                    g_psyq_cd_stream_emulation_sector_index++;
                g_psyq_cd_stream_diagnostic_code = 8;
                return;
            }
            /* The signed lvalue retains the original LH rather than LHU. */
            if (*(s16*)&g_psyq_cd_stream_ring[0].status) {
                CURRENT_HEADER.status = StFREE;
                if (stream_emulation_base)
                    g_psyq_cd_stream_emulation_sector_index++;
                g_psyq_cd_stream_diagnostic_code = 9;
                return;
            }
            CURRENT_HEADER.status = StREWIND;
            {
                u32* destination = (u32*)g_psyq_cd_stream_ring;
                u32* source = (u32*)current_header_address;
                g_psyq_cd_stream_write_index = 0;
                for (index = 0; index < PSYQ_CD_STREAM_HEADER_WORDS; index++)
                    *destination++ = *source++;
            }
            g_psyq_cd_stream_current_header = g_psyq_cd_stream_ring;
        }
        g_psyq_cd_stream_frame_start_index = g_psyq_cd_stream_write_index;
    }
    g_psyq_cd_stream_diagnostic_code = 10;
    g_psyq_cd_stream_expected_sector_index++;
    g_psyq_cd_stream_payload = (u32*)((u8*)(g_psyq_cd_stream_ring + g_psyq_cd_stream_ring_sectors)
        + g_psyq_cd_stream_write_index * PSYQ_CD_STREAM_PAYLOAD_BYTES);
    dma_control = 0x11000000;
    if (g_psyq_cd_stream_mode) {
        *g_psyq_cd_stream_access_delay_register = 0x20943;
        *g_psyq_cd_stream_common_delay_register = 0x1323;
    } else {
        *g_psyq_cd_stream_access_delay_register = 0x21020843;
        dma_control = 0x11400100;
    }
    {
        u32 frame_complete;
        if (CURRENT_HEADER.sector_count - 1 == CURRENT_HEADER.sector_index) {
            frame_complete = 1;
            /* Keep the known completion constant in the original branch delay slot. */
            __asm__("" : : "r"(frame_complete));
            g_psyq_cd_stream_frame_dma_pending = frame_complete;
            if ((emulation_base = stream_emulation_base) != 0) {
                ((cd_stream_copy_t)mem2mem)(g_psyq_cd_stream_payload,
                    (u32*)((g_psyq_cd_stream_emulation_sector_index << 11) + emulation_base + 32),
                    PSYQ_CD_STREAM_PAYLOAD_WORDS, 1);
                g_psyq_cd_stream_emulation_sector_index++;
            } else {
                dma_execute(PSYQ_CD_DMA_CHANNEL, g_psyq_cd_stream_payload, 0, PSYQ_CD_STREAM_PAYLOAD_WORDS, dma_control,
                    frame_complete, 0);
            }
            g_psyq_cd_stream_expected_sector_index = 0;
            g_psyq_cd_stream_frame_count = 0;
            g_psyq_cd_stream_current_channel = g_psyq_cd_stream_requested_channel;
        } else {
            if ((emulation_base = stream_emulation_base) != 0) {
                ((cd_stream_copy_t)mem2mem)(g_psyq_cd_stream_payload,
                    (u32*)((g_psyq_cd_stream_emulation_sector_index << 11) + emulation_base + 32),
                    PSYQ_CD_STREAM_PAYLOAD_WORDS, 0);
                g_psyq_cd_stream_emulation_sector_index++;
            } else {
                dma_execute(
                    PSYQ_CD_DMA_CHANNEL, g_psyq_cd_stream_payload, 0, PSYQ_CD_STREAM_PAYLOAD_WORDS, dma_control, 0, 0);
            }
        }
    }
    *g_psyq_cd_stream_common_delay_register = 0x1325;
    CURRENT_HEADER.status = StBUSY;
    g_psyq_cd_stream_write_index++;
    if (stream_emulation_base && g_psyq_cd_stream_frame_dma_pending)
        data_ready_callback();
}
