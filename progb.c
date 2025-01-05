#include "io.h"
#include "common.h"
#include <stddef.h>

// function that is defined in boot.s, calling this essentially works as
// a "cooperative interrupt" which when called switches the running process
// from progb code to proga code
void switch_a();
void output_mem2();
void write_ui32_hex();

// this struct serves as a way to essentially "display memory", in that
// we can look at the memory being used in mem2 by calling function
// ouput_mem2
typedef struct mem2 {
        int id;
        float value;
        char name[8];
} mem2_t;

// progb main, pretty self explanatory
void progb_main() {
        ui64 y = 0;
        while (y < 100000000) {
                y++;
		ui64 x = 0;
		while (x<10) {
			x++;
		}
                if (y == 500000) {
			write("running b...\n");
		}
		if (y == 1000000) {
			write("running b...\n");
        	}
	}
	// lets print some memory byte by byte
	write("lets try to print some memory (progb)\n");
	mem2_t s = { 123, 1.23, "hello!" };
	output_mem2(&s, sizeof(mem2_t));
	write("cooperative switch from progb\n");
	switch_a();
}

// ouput_mem2 function, it takes in a ptr to anything (a struct in our case)
// and a size of the struct which will serve to determine the number of
// write statements
void output_mem2(void *ptr, size len) {
        ui8 *byte_ptr = (ui8 *)ptr;
        for (size i = 0; i < len; i++) {
                write_ui32_hex(byte_ptr[i]);
                write("\n");
        }
}
