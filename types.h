#pragma once

typedef unsigned char      ui8;
typedef unsigned short     ui16;
typedef unsigned int       ui32;
typedef unsigned long long ui64;

typedef signed char        i8;
typedef signed short       i16;
typedef signed int         i32;
typedef signed long long   i64;

#if defined(RV32)
  typedef ui32 usize;
  typedef i32 size;

# define _LIT_UI32 UL
# define _LIT_UI64 ULL

#elif defined(RV64)
  typedef ui64 usize;
  typedef i64 size;
#else
# error "RV32 xor RV64 needs to be defined!"
#endif
