#pragma once

#include "common.h"

size str_nt_len(char *str_nt);
i32 str_nt_eq(char *a, char *b);

void write(char *str_nt);
i32 readln_poll(char *buf, ui32 len);

void ui64_2_str_0x(ui64 value, ui8 ui8x22[22]);
i32 i32_2_str_nt_0d(i32 value, char *buffer, usize len);
