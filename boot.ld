/*In this I did the memory mapping of the upper and lower of 128MiB*/
/*I placed proga and progb in the upper part of the 128MiB*/
/* I then included the rest of the os code*/
MEMORY{
        dram_space (rwx) : ORIGIN = 0x80000000, LENGTH = 64M
        hard_drive (rwx) : ORIGIN = 0x84000000, LENGTH = 64M
}

SECTIONS
{
        /* place progA and progB in the upper 64MiB */
        .text 0x84000000: {
                proga.o(.text)
        } > hard_drive
        .text 0x84001000: {
                progb.o(.text)
        } > hard_drive

        /* OS CODE */
        .text 0x81000000 : {
                boot.o(.text)
                *(.text)
        } > dram_space
        
        .data : {
                *(.data)
        } > dram_space
        
        .bss : {
                *(.bss)
        } > dram_space

        .rodata : {
                *(.rodata)
        } > dram_space
}
