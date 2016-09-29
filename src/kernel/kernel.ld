SECTIONS {
.text 0x8000 : {*(.init)
                *(.text)
                *(.bss)
                *(.rodata*)
                *(COMMON)
                }
.data : { *(.data)
        }

/DISCARD/ : { *(*) }
}
