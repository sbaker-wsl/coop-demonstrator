#pragma once

#include "common.h"


#define SBI_SUCCESS 0
#define SBI_ERR_FAILED -1
#define SBI_ERR_NOT_SUPPORTED -2
#define SBI_ERR_INVALID_PARAM -3
#define SBI_ERR_DENIED -4
#define SBI_ERR_INVALID_ADDRESS -5
#define SBI_ERR_ALREADY_AVAILABLE -6
#define SBI_ERR_ALREADY_STARTED -7
#define SBI_ERR_ALREADY_STOPPED -8
#define SBI_ERR_NO_SHMEM -9

#define SBI_SRST_TYPE_SHUTDOWN 0
#define SBI_SRST_REASON_NONE 0

struct sbiret {
    size error;
    size value;    
};
typedef struct sbiret sbiret_t;

sbiret_t sbi_ecall(
        ui32 p0,
        ui32 p1,
        ui32 p2,
        ui32 p3,
        ui32 p4,
        ui32 p5,
        ui32 fid,
        ui32 eid);

sbiret_t sbi_debug_console_write(ui32 num_bytes, ui32 base_addr_lo, ui32 base_addr_hi);
sbiret_t sbi_debug_console_read(ui32 num_bytes, ui32 base_addr_lo, ui32 base_addr_hi);

sbiret_t sbi_system_reset(ui32 reset_type, ui32 reset_reason);

sbiret_t sbi_set_timer(ui64 stime_value);

sbiret_t sbi_shutdown();
