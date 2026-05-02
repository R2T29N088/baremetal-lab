AS := $(TOOLCHAIN_DIR)arm-none-eabi-as
CC := $(TOOLCHAIN_DIR)arm-none-eabi-gcc
ifeq ($(LTO),ON)
AR := $(TOOLCHAIN_DIR)arm-none-eabi-gcc-ar
else
AR := $(TOOLCHAIN_DIR)arm-none-eabi-ar
endif
LD := $(TOOLCHAIN_DIR)arm-none-eabi-ld
OBJCOPY := $(TOOLCHAIN_DIR)arm-none-eabi-objcopy
OBJDUMP := $(TOOLCHAIN_DIR)arm-none-eabi-objdump
export AS CC AR LD OBJCOPY OBJDUMP
