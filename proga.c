#include "io.h"
#include "common.h"
#include <stddef.h>

// proga.c
// Write 2 little C programs (A and B) that are computation bound. To
// simplify, the programs should not use any global variables and should
// not perform input. If data is needed, it can be placed on the stack in
// the main function and then shared via a pointer. Every few seconds, each
// program should write some message output. Use one *.c file per program

// function defined in boot.s, calling basically works as a
// "cooperative interrupt" which when called switches the running
// process from proga code to progb code


void output_mem();
void switch_b();
void write_ui32_hex();
typedef struct mem {
        int id;
        float value;
        char name[8];
} mem_t;

void proga_main() {
        ui64 x = 0;
        while (x<10000000) {
                x++;
		ui64 y = 0;
		while (y < 10) {
			y++;
		}
		if (x == 1000) {
			write("running a...\n");
		}
		if (x == 5000000) {
			write("running a...\n");
		}
        }
	// lets print some memory byte by byte
	write("lets try to print some memory!\n");
	mem_t t = { 42, 3.14, "example" };
	output_mem(&t, sizeof(mem_t));
	write("cooperative switch from proga\n");
	// cooperative, proga gives up its running state to
	// progb
	switch_b();
}

void output_mem(void* ptr, size len) {
        ui8 *byte_ptr = (ui8 *)ptr;
        for (size i = 0; i < len; i++) {
                write_ui32_hex(byte_ptr[i]);
                write("\n");
        }
}
