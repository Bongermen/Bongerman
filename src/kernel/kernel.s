.globl _start
.section .init
_start:
    mov sp, #0x8000                  @ stack pointer

    @ enable floating point arithmetic
    mrc p15, 0, r0, c1, c0, 2 
    orr r0, r0, #0x300000 
    orr r0, r0, #0xC00000
    mcr p15, 0, r0, c1, c0, 2
    mov r0, #0x40000000
    fmxr fpexc,r0

    @ enable L1 cache
    mrc p15,0,r0,c1,c0,0
    orr r0,#0x4
    orr r0,#0x800
    orr r0,#0x1000
    mcr p15,0,r0,c1,c0,0

    @ start main
    bl kernel_main
hang:
    b hang

@ dummy function for use in C files to circumvent compiler optimization
.globl dummy
dummy:
    bx lr
