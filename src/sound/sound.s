@ This file is part of the Team 28 Project
@ Licensing information can be found in the LICENSE file
@ (C) 2014 The Team 28 Authors. All rights reserved.
.global setup_sound
.global update_sound

@ ------------------------------------------------------------------------------
@ GPIO Ports
@ ------------------------------------------------------------------------------
.equ GPIO_FSEL0,       0x20200000
.equ GPIO_FSEL1,       0x20200004
.equ GPIO_FSEL2,       0x20200008
.equ GPIO_FSEL3,       0x2020000C
.equ GPIO_FSEL4,       0x20200010
.equ GPIO_FSEL5,       0x20200014
.equ GPIO_SET0,        0x2020001C
.equ GPIO_SET1,        0x20200020
.equ GPIO_CLR0,        0x20200028
.equ GPIO_CLR1,        0x2020002C
.equ GPIO_LEV0,        0x20200034
.equ GPIO_LEV1,        0x20200038
.equ GPIO_EDS0,        0x20200040
.equ GPIO_EDS1,        0x20200044
.equ GPIO_REN0,        0x2020004C
.equ GPIO_REN1,        0x20200050
.equ GPIO_FEN0,        0x20200058
.equ GPIO_FEN1,        0x2020005C
.equ GPIO_HEN0,        0x20200064
.equ GPIO_HEN1,        0x20200068
.equ GPIO_LEN0,        0x20200070
.equ GPIO_LEN1,        0x20200074
.equ GPIO_AREN0,       0x2020007C
.equ GPIO_AREN1,       0x20200080
.equ GPIO_AFEN0,       0x20200088
.equ GPIO_AFEN1,       0x2020008C
.equ GPIO_PUD,         0x20200094
.equ GPIO_UDCLK0,      0x20200098
.equ GPIO_UDCLK1,      0x2020009C

@ ------------------------------------------------------------------------------
@ Clock manager
@ ------------------------------------------------------------------------------
.equ CM_PWMCTL,        0x201010A0
.equ CM_PWMDIV,        0x201010A4

@ ------------------------------------------------------------------------------
@ Direct Memory Access
@ ------------------------------------------------------------------------------
.equ DMA0_CS,          0x20007000
.equ DMA0_CONBLK,      0x20007004
.equ DMA_INT_STATUS,   0x20007FE0
.equ DMA_ENABLE,       0x20007FF0

@ ------------------------------------------------------------------------------
@ Pulse Width modulator
@ ------------------------------------------------------------------------------
.equ PWM_CTL,          0x2020C000
.equ PWM_STA,          0x2020C004
.equ PWM_DMAC,         0x2020C008
.equ PWM_RNG1,         0x2020C010
.equ PWM_DAT1,         0x2020C014
.equ PWM_FIF1,         0x2020C018
.equ PWM_RNG2,         0x2020C020
.equ PWM_DAT2,         0x2020C024

@ ------------------------------------------------------------------------------
@ Macro that simplifies adding new sounds
@ ------------------------------------------------------------------------------
.macro sound name
  .long 0                    @ Play flag
  .long \name\()_start       @ Address of current chunk
  .long \name\()_start       @ Start address of sample
  .long \name\()_end         @ End address of sample
  .global snd_play_\name
  snd_play_\name:            @ Start playing
    push  {r0}
    mov   r0, #1
    str   r0, [pc, #-32]
    pop   {r0}
    mov   pc, lr
  .global snd_stop_\name
  snd_stop_\name:
    push  {r0}
    mov   r0, #0
    str   r0, [pc, #-52]
    pop   {r0}
    mov   pc, lr
.endm

.section .text
@ ------------------------------------------------------------------------------
@ Initialises the sound module, setting up GPIO 40 and 45 to use PWM and
@ programming DMA channel 1 with two control blocks chained together that
@ write data to the GPIO ports
@
@ Arguments:
@   none
@ Returns:
@   none
@ Clobbers:
@   r0 - r3
@ ------------------------------------------------------------------------------
setup_sound:
  @ Copy first chunk of background music into buffers
  ldr         r0, =theme_start
  ldr         r1, =dma_buffer_0
  ldr         r2, =0x2000
  mov         r3, #0
1:
  ldrb        r3, [r0], #1
  str         r3, [r1], #4
  subs        r2, r2, #1
  bne         1b

  ldr         r1, =dma_buffer_1
  ldr         r2, =0x2000
1:
  ldrb        r3, [r0], #1
  str         r3, [r1], #4
  subs        r2, r2, #1
  bne         1b

  @ Set GPIO 40 & 45 to PWM
  ldr         r0, =GPIO_FSEL4
  ldr         r1, [r0]
  ldr         r2, =0x00038007
  bic         r1, r1, r2
  ldr         r2, =0x00020004
  orr         r1, r1, r2
  str         r1, [r0]

  @ Setup clock
  ldr         r0, =CM_PWMDIV
  ldr         r1, =0x5A002000
  str         r1, [r0]
  ldr         r0, =CM_PWMCTL
  ldr         r1, =0x5A000016
  str         r1, [r0]

  @ Setup PWM
  ldr         r1, =0x00002C48
  ldr         r0, =PWM_RNG1
  str         r1, [r0]
  ldr         r0, =PWM_RNG2
  str         r1, [r0]
  ldr         r0, =PWM_CTL
  ldr         r1, =0x00002161
  str         r1, [r0]

  @ Setup PWM to use DMA
  ldr         r0, =PWM_DMAC
  ldr         r1, =0x80000001
  str         r1, [r0]

  @ Enable DMA0
  ldr         r0, =DMA_ENABLE
  ldr         r1, =0x00000001
  str         r1, [r0]

  @ Set DMA0 control block
  ldr         r0, =DMA0_CONBLK
  adr         r1, DMA_CTRL_1
  str         r1, [r0]

  @ Start DMA0
  ldr         r0, =DMA0_CS
  ldr         r1, =0x00000001
  str         r1, [r0]

  mov         pc, lr

@ ------------------------------------------------------------------------------
@ Should be called when DMA triggers an interrupt, but unfortunately the
@ hardware seems incapable of triggering it. Fortunately, we can poll for the
@ interrupt flag in the DMA interrupt status register and call the function
@ ourselves
@
@ Arguments:
@   none
@ Clobbers:
@   none
@ Returns:
@   none
@ ------------------------------------------------------------------------------
update_sound:
  stmfd       sp!, {r0 - r10, lr}

  ldr         r0, =DMA_INT_STATUS
  ldr         r1, [r0]
  tst         r1, r1
  ldmeqfd     sp!, {r0 - r10, pc}

  ldr         r0, =DMA0_CS
  ldr         r1, =0x00000005
  str         r1, [r0]

  @ Swap buffers
  ldr         r1, =buffer_index
  ldr         r0, [r1]
  add         r0, r0, #1
  and         r0, r0, #1
  str         r0, [r1]

  @ Find target buffer
  tst         r0, r0
  ldrne       r0, =dma_buffer_0
  ldreq       r0, =dma_buffer_1

  @ Copy background sound
  ldr         r1, =theme_start
  ldr         r2, =theme_ptr
  ldr         r3, [r2]
  ldr         r4, =theme_end
  sub         r4, r4, #0x2000
  add         r3, r3, #0x2000
  cmp         r3, r4
  movge       r3, r1
  str         r3, [r2]

  ldr         r4, =0x2000
  mov         r2, r0
1:
  ldrb        r5, [r3], #1
  lsl         r5, r5, #3
  str         r5, [r2], #4
  subs        r4, r4, #1
  bne         1b

  @ Play sounds
  ldr         r9, =sounds
  mov         r10, #9
1:
  ldr         r5, [r9]
  tst         r5, r5
  beq         3f

  ldr         r5, [r9, #4]
  ldr         r3, =0x2000
  mov         r7, r0
2:
  ldrb        r6, [r5], #1
  sub         r6, r6, #0x7F
  ldr         r8, [r7]
  add         r8, r8, r6, lsl #3
  str         r8, [r7], #4

  subs        r3, r3, #1
  bne         2b

  ldr         r3, [r9, #8]
  ldr         r4, [r9, #12]
  cmp         r5, r4
  movge       r5, r3
  str         r5, [r9, #4]
  movge       r5, #0
  strge       r5, [r9]
3:
  add         r9, #56
  subs        r10, r10, #1
  bne         1b

  ldmfd       sp!, {r0 - r10, pc}


.ltorg
.section .text
@ ------------------------------------------------------------------------------
@ DMA control structures - chained after each other
@ ------------------------------------------------------------------------------
.align 5
DMA_CTRL_0:
  .long 0x00050141    @ Attributes
  .long dma_buffer_0  @ Source address
  .long 0x7E20C018    @ Destination Address
  .long 0x8000        @ Transfer length
  .long 0
  .long DMA_CTRL_1

.align 5
DMA_CTRL_1:
  .long 0x00050141    @ Attributes
  .long dma_buffer_1  @ Source address
  .long 0x7E20C018    @ Destination Address
  .long 0x8000        @ Transfer length
  .long 0
  .long DMA_CTRL_0


.align 4
@ ------------------------------------------------------------------------------
@ DMA buffers
@ ------------------------------------------------------------------------------
dma_buffer_0:
  .space 0x8000, 0
dma_buffer_1:
  .space 0x8000, 0


.align 2
@ ------------------------------------------------------------------------------
@ Sound effect states
@ ------------------------------------------------------------------------------
buffer_index:
  .long 1

sounds:
  sound pickup
  sound timer_ticking
  sound explosion
  sound plant
  sound death
  sound crowd_cheer
  sound player_ready
  sound game_name
  sound end_game

.section .data
@ ------------------------------------------------------------------------------
@ Background music
@ ------------------------------------------------------------------------------
theme_start: .incbin "assets/sounds/bomberman_theme.bin"
theme_end:
theme_ptr:   .long theme_start

@ ------------------------------------------------------------------------------
@ Short sounds effects
@ ------------------------------------------------------------------------------
pickup_start:   .incbin "assets/sounds/pickup.bin"
pickup_end:
timer_ticking_start:   .incbin "assets/sounds/timer_ticking.bin"
timer_ticking_end:
explosion_start:     .incbin "assets/sounds/explosion.bin"
explosion_end:
plant_start:    .incbin "assets/sounds/plant.bin"
plant_end:
death_start:     .incbin "assets/sounds/death.bin"
death_end:
crowd_cheer_start: 	  .incbin "assets/sounds/crowd_cheer.bin"
crowd_cheer_end:
player_ready_start:   .incbin "assets/sounds/player_ready.bin"
player_ready_end:
game_name_start:  .incbin "assets/sounds/game_name.bin"
game_name_end:
end_game_start:   .incbin "assets/sounds/end_game.bin"
end_game_end:
