#include "common.h"
#include "io.h"
#include "types.h"

// this is a simple function that will simply call proga_main() once
// and progb_main() once... needs to be initialized here because it is
// called in main which is defined before scheduler
void scheduler();
void sbi_shutdown();


// main function which will run via the call main from boot.s in the
// _start function
int main(int argc, char *argv[]){
	// output showing that main has been entered, mainly there for
	// debugging purposes
        write("starting program (entered main)...\n");
	scheduler();
	// this return will never run but is used so compiler doesn't
	// complain, code will execute
        return 1;
}

// this function demonstrates our placement of the .text section of
// proga at 0x84000000
// running this function will be the same as doing proga_main()
void jump_to_proga() {
        void (*f)() = (void*)0x84000000; 	// .text of proga sits here
        f();					// call function ptr
}

// this function serves as a test to show that if progb is called first
// instead of proga, the result is the same - 5 processes will be run
void jump_to_progb() {
	void (*f)() = (void*)0x84001000;	// .text of progb sits here
	f();					// call function ptr
}

void scheduler() {
	// scheduler only needs to run proga_main() for the boot.s code
	// to run, it will switch between proga and progb until the
	// global counter in boot.s reaches 5

	// a,b,a,b,a
	jump_to_proga();

	// this uncommented code serves to show that if progb is called
	// first, it will still run 5 processes in the order b,a,b,a,b
	// to check, simply comment jump_to_proga() and uncomment this jump
	//jump_to_progb();
}

// this code simply can be called from boot.s which will shut down the
// system, this is meant to be called after the counter variable in boot.s
// reaches 5... this func is used for if proga is the last running process
void end_main_a() {
	write("instead of a cooperative switch back to proga, 5 processes");
	write(" have been run.\n");
	write("calling sbi_shutdown()...\n");
	// shut down (end execution)
	sbi_shutdown();
}

// this code is the same as a but if progb is last running process
void end_main_b() {
	write("instead of a cooperative switch back to progb, 5 processes");
        write(" have been run.\n");
        write("calling sbi_shutdown()...\n");
        // shut down (end execution)
        sbi_shutdown();
}
