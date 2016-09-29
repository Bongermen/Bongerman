.global a_bomberman_right
.global a_bomberman_left
.global a_bomberman_up
.global a_bomberman_down
.global a_bomb_exploding
.global a_bomb_neutral
.global a_terrain
.global a_explosion
.global a_item_icons
.global a_player_icons
.global a_ready_icon
.global a_logo
.global a_trophy
.global a_clock
.global font

@ Remember to add extern pointers in assets.h
.section .data

.align 4
font:
    .incbin "font.bin"

.align 4
a_ready_icon:
    .incbin "ready_icon.bin"

.align 4
a_logo:
    .incbin "logo.bin"

.align 4
a_clock:
    .incbin "clock.bin"

.align 4
a_trophy:
    .incbin "trophy.bin"

.align 4
a_explosion:
    .incbin "explosion.bin"

.align 4
a_item_icons:
    .int 6  @ Number of frames
    .int 32 @ frame width
    .int 32 @ frame height
    .incbin "a_item_icons.bin"

.align 4
a_player_icons:
    .int 5  @ Number of frames
    .int 32 @ frame width
    .int 32 @ frame height
    .incbin "a_player_icons.bin"

.align 4
a_bomberman_down:
    .int 28  @ Number of frames
    .int 32 @ frame width
    .int 32 @ frame height
    .incbin "a_bomberman_down.bin"

.align 4
a_bomberman_up:
    .int 28  @ Number of frames
    .int 32 @ frame width
    .int 32 @ frame height
    .incbin "a_bomberman_up.bin"

.align 4
a_bomberman_left:
    .int 56  @ Number of frames
    .int 32 @ frame width
    .int 32 @ frame height
    .incbin "a_bomberman_left.bin"

.align 4
a_bomberman_right:
    .int 56  @ Number of frames
    .int 32 @ frame width
    .int 32 @ frame height
    .incbin "a_bomberman_right.bin"

.align 4
a_terrain:
    .int 4  @ Number of frames
    .int 32 @ frame width
    .int 32 @ frame height
    .incbin "a_terrain.bin"

.align 4
a_bomb_neutral:
    .int 2  @ Number of frames
    .int 32 @ frame width
    .int 32 @ frame height
    .incbin "a_bomb_neutral.bin"

.align 4
a_bomb_exploding:
    .int 2  @ Number of frames
    .int 32 @ frame width
    .int 32 @ frame height
    .incbin "a_bomb_exploding.bin"

.section .text
