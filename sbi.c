#include "common.h"
#include "io.h"

#define SBI_DBCN_EID 0x4442434E  // 'DBCN'
#define SBI_DBCN_FID_CONSOLE_WRITE 0
#define SBI_DBCN_FID_CONSOLE_READ  1

#define SBI_SRST_EID 0x53525354 // 'SRST'
#define SBI_SRST_FID_SYSTEM_RESET 0

#define SBI_TIME_EID 0x54494D45 // 'TIME'
#define SBI_TIME_FID_SET_TIMER 0


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

sbiret_t sbi_debug_console_write(ui32 num_bytes, ui32 base_addr_lo, ui32 base_addr_hi)

{
    return sbi_ecall(
        num_bytes, (usize) base_addr_lo, base_addr_hi, 
        0, 0, 0,
        SBI_DBCN_FID_CONSOLE_WRITE,
        SBI_DBCN_EID);
}

sbiret_t sbi_shutdown()
{
        return sbi_ecall(
        0, 0, 0, 0, 0, 0,
        0, 0x08);
}

sbiret_t sbi_debug_console_read(ui32 num_bytes, ui32 base_addr_lo, ui32 base_addr_hi)
{
    return sbi_ecall(
        num_bytes, (usize) base_addr_lo, base_addr_hi, 
        0, 0, 0,
        SBI_DBCN_FID_CONSOLE_READ,
        SBI_DBCN_EID);
}

sbiret_t sbi_system_reset(ui32 reset_type, ui32 reset_reason)
{
    return sbi_ecall(
        reset_type, reset_reason,
        0, 0, 0, 0,
        SBI_SRST_FID_SYSTEM_RESET,
        SBI_SRST_EID);
}
