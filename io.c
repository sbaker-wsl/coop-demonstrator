#define RV32
#include "types.h"
#include "io.h"
#include "sbi.h"

size str_nt_len(char *str_nt) {
    size len = 0;	
    while (str_nt[len] != '\0') { 
        len += 1;
    }
    return len;
}

i32 str_nt_eq(char *a, char *b) {
    ui32 end;

    while (a[0] != '\0' || b[0] != '\0') {
        if (a[0] == '\0' || b[0] == '\0') {
            return 0;
        }
        if (a[0] != b[0]) {
            return 0;
        }
        a += 1;
        b += 1;
    }
    return 1;
}


void write(char *str_nt) {
    size len = str_nt_len(str_nt);
    sbi_debug_console_write(len, (ui32) str_nt, 0);
}

void write_char(char c) {
    char char_nt[2] = { c, '\0' };
    write(char_nt);
}

i32 readch_poll(char *ptr_c) {
    sbiret_t ret;
    do {
        ret = sbi_debug_console_read(1, (ui32) (ptr_c), 0);
	if (ret.error != SBI_SUCCESS) {
	    return ret.error;
	}
    } while (ret.value <= 0);
    return SBI_SUCCESS;
}

#define ANSI_LEFT  "\x1B[D"
#define ANSI_RIGHT "\x1B[C"
#define ANSI_UP    "\x1B[A"
#define ANSI_DOWN  "\x1B[B"

#define KEY_ESC   0x0027
#define KEY_LEFT  0x2190
#define KEY_RIGHT 0x2192
#define KEY_UP    0x2191
#define KEY_DOWN  0x2193

i32 parse_esc(ui32 *code) {
    char c;
    char state = 'E';
    sbiret_t ret;

    ret = sbi_debug_console_read(1, (ui32) &c, 0);
    if (ret.error != SBI_SUCCESS) {
        return ret.error;
    }
    if (ret.value <= 0) {
        *code = KEY_ESC;
	return 0;
    }

    while (1) {
        switch (state) {
        case 'E': 
	    switch (c) {
	    case '[': state = '['; break;
	    default: return -1;
	    } 
	    break;
        case '[':
	    switch (c) {
	    case 'D': *code = KEY_LEFT; return 0;
	    case 'C': *code = KEY_RIGHT; return 0;
	    case 'A': *code = KEY_UP; return 0;
	    case 'B': *code = KEY_DOWN; return 0;
	    default: return -1; // could not parse
	    } 
	    break;
        default:
            return -1;
        }
        ret.error = readch_poll(&c);
        if (ret.error != SBI_SUCCESS) {
            return ret.error;
        }
    }
}

i32 readln_poll(char *line, ui32 len) {
    if (len < 2) {
        return -1;
    }
    i32 error;
    ui32 pos = 0;
    char c;
    while (1) {
        error = readch_poll(&c);
        if (error != SBI_SUCCESS) {
	    return error;
	}

		
	// if 'carriage return' (13, '\r')
	if (c == '\r') {
	    line[pos] = '\0';
	    write("\r\n");
	    return 0; 
	} else {
	    // if ESC (27) code sequence
	    if (c == 0x1B) {
	        // parse ESC
		ui32 key;
		if (!parse_esc(&key)) {
	            switch(key) {
			case KEY_LEFT: // write(ANSI_LEFT); break;
			case KEY_RIGHT: // write(ANSI_RIGHT); break;
			case KEY_UP: break;
			case KEY_DOWN: break;
			case KEY_ESC: write("\x07"); break;
		    }
		}
		// write("ESC:");
		continue;
	    }
	    // else if 'delete' (127, 0x7F)
	    else if (c == 0x7F) {
		// if not at start of line
		if (pos>0) {
		    // write ANSI sequence "ESC [ 1 P" (delete 1 character)
		    write("\b\x1B[1P");
		    pos -= 1;
		}
	    }
	    // else if buffer not full
	    else if (pos < len-1) {
	        // write character
	        write_char(c);  // to screen
                line[pos] = c;  // to line buffer
	        pos += 1;
	    }

	}
	
	/*
	char digits[32]; 
	i32_2_str_nt_0d((i32) c, digits, 32);
        write(digits);
	write(".");
	*/
    }
}


char syms_hex[16] = {
	'0','1','2','3',
	'4','5','6','7',
	'8','9','A','B',
	'C','D','E','F',
};


i32 i32_2_str_nt_0d(i32 value, char *buffer, usize len) {
    if (len < 2) {
        return -1;
    }
    if (value == 0) {
	buffer[0] = '0';
	buffer[1] = '\0';
    } else {
	if (value < 0) {
	    buffer[0] = '-';
	    buffer += 1;
	    len -= 1;
	}
	i32 i = 0;
        while (value > 0) {
	    if (i >= len-1) {
		buffer[i] = '\0';
		return -1;
	    }
	    char digit = '0' + (ui8) (value%10);
	    // write_char(digit); write("|");
	    value = value / 10;
	    buffer[i] = digit;
	    i += 1;
	}
	buffer[i] = '\0';
    }
    return 0;
}


i32 ui32_2_str_nt_0x(ui32 value, char *buffer, usize len) {
    if (len < 11) {
        return -1;
    }
    buffer[0] = '0';
    buffer[1] = 'x';
    buffer = buffer + 2;
    for (size i=0; i<8; ++i) {
        buffer[7-i] = syms_hex[value % 16U];
        value /= 16U;
    }
    buffer[8] = '\0';
    return 0;
}

void ui64_2_str_0x(ui64 value, ui8 ui8x22[22]) {
    ui8x22[0] = '0';
    ui8x22[1] = 'x';
    size i = 19;
    while (i>=0) {
        ui8x22[i+2] = syms_hex[(ui8) (value & 0xF)]; value >>=4;
	i -= 1;
	if (i%5==0) {
	    ui8x22[i+2] = '_';
	    i -= 1;
	}
    }
}

void write_ui32_hex(ui32 num) {
    char buffer[16];
    ui32_2_str_nt_0x(num, buffer, 15);
    write(buffer);
}


