CC        = arm-none-eabi-gcc
CFLAGS    = -march=armv6zk -mtune=arm1176jzf-s -std=c99 -O3 -Wall -Werror -mfloat-abi=hard -mfpu=vfp -I $(ASSET_DIR) -nostartfiles
AFLAGS    = -march=armv6zk -mfloat-abi=hard -mfpu=vfp -I $(ASSET_DIR)
ARM       = arm-none-eabi

BUILD_DIR     = build
ASSET_DIR     = assets
SRC_DIR       = src

LINKER_SCRIPT = $(shell find $(SRC_DIR) -name kernel.ld)
KERNEL_ELF    = $(BUILD_DIR)/kernel.elf
KERNEL_IMG    = $(BUILD_DIR)/kernel.img
KERNEL_LIST   = $(BUILD_DIR)/kernel.list

SRC     = $(shell find $(SRC_DIR) -name "*.c")
ASM     = $(shell find $(SRC_DIR) -name "*.s")

OBJ     = $(SRC:%.c=%.o)
ASM_OBJ = $(ASM:%.s=%.o)

.PHONY: all clean

all: $(KERNEL_IMG) $(KERNEL_LIST)

$(ASM_OBJ) : %.o: %.s
	$(ARM)-as $(AFLAGS) $< -o $@

$(KERNEL_ELF): $(ASM_OBJ) $(OBJ)
	$(CC) $(CFLAGS) $^ -T $(LINKER_SCRIPT) -o $@

$(KERNEL_IMG): $(KERNEL_ELF)
	$(ARM)-objcopy $< -O binary $@

$(KERNEL_LIST): $(KERNEL_ELF)
	$(ARM)-objdump -d $< > $@

clean:
	rm -f $(OBJ)
	rm -f $(ASM_OBJ)
	rm -f $(KERNEL_ELF)
	rm -f $(KERNEL_IMG)
	rm -f $(KERNEL_LIST)
