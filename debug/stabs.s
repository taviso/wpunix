#
# vim: ft=asm
#
# This is a manually created symbol file template. If you want to create a
# symbol file to debug a binary without symbols, you can use this file to
# define stabs.
#
# Compile:
#
#   $ as -gstabs symbols.s -o symbols.o
#
# Now you can do this in gdb:
#
#   (gdb) add-symbol-file symbols.o
#

.altmacro

# Some of the common stabs types we can use.
.equ N_GSYM, 0x20
.equ N_FUN, 0x24
.equ N_STSYM, 0x26
.equ N_ROSYM, 0x2c
.equ N_RSYM, 0x40
.equ N_SLINE, 0x44
.equ N_DSLINE, 0x46
.equ N_BSLINE, 0x48
.equ N_FLINE, 0x4c
.equ N_SSYM, 0x60
.equ N_SO, 0x64
.equ N_LSYM, 0x80
.equ N_PSYM, 0xa0
.equ N_LBRAC, 0xc0
.equ N_RBRAC, 0xe0
.equ N_BCOMM, 0xe2
.equ N_ECOMM, 0xe4
.equ N_ECOML, 0xe8

# Set this to the size of a long.
.equ PARAM_SIZE, 4

# Make enough space to fill sections without confusing gdb.
.section .text
.fill _textsize, 1, 0

.section .data
.fill _datasize, 1, 0

.section .bss
.fill _bsssize, 1, 0

# Predefined types, prefix with * for a pointer
#
# -1    int, 32 bit signed integral type.
# -2    char, 8 bit type holding a character.
# -3    short, 16 bit signed integral type.
# -4    long, 32 bit signed integral type.
# -5    unsigned char, 8 bit unsigned integral type.
# -6    signed char, 8 bit signed integral type.
# -7    unsigned short, 16 bit unsigned integral type.
# -8    unsigned int, 32 bit unsigned integral type.
# -9    unsigned, 32 bit unsigned integral type.
# -10   unsigned long, 32 bit unsigned integral type.
# -11   void, type indicating the lack of a value.
# -12   float, IEEE single precision.
# -13   double, IEEE double precision.
# -14   long double, IEEE double precision.
# -16   boolean. 32 bit type.
# -17   short real. IEEE single precision.
# -18   real. IEEE double precision.
# -19   stringptr.
# -30   wchar. Wide character, 16 bits wide, unsigned.
# -31   long long, 64 bit signed integral type.
# -32   unsigned long long, 64 bit unsigned integral type.

.equ int, -1
.equ int32_t, -1
.equ char, -2
.equ int16_t, -3
.equ short, -3
.equ long, -4
.equ uint8_t, -5
.equ int8_t, -6
.equ uint16_t, -7
.equ uint32_t, -8
.equ unsigned, -9
.equ size_t, -10
.equ void, -11
.equ float, -12
.equ double, -13
.equ bool, -16
.equ uint64_t, -32
.equ int64_t, -31

# This is used to verify definitions are sorted.
.set _prevaddr, 0

# This is used to reset after an .include
.macro resetorder
    .set _prevaddr, 0
.endm

# type is a number from the list above, or a custom type.
.macro function name, address, ptr=, type
    .if _prevaddr > \address
        .warning "Function \name was not in ascending numerical order."
    .endif
    .set _prevaddr, \address
    .set _arg, 0
    .stabs "\name:f\ptr\type", N_FUN, 0, 0, \address
.endm

.macro param name, ptr=, type
    .set _arg, _arg + 1
    .stabs "\name:p\ptr\type", N_PSYM, 0, 0, PARAM_SIZE+_arg*PARAM_SIZE
.endm

.macro variable name, address, ptr=, type
    .stabs "\name:S\ptr\type", N_STSYM, 0, 0, \address
.endm
