ROOT_DIR := $(CURDIR)/
OUT_DIR:= $(ROOT_DIR)out/
OBJ_DIR := $(ROOT_DIR)obj/
EXTERNAL_DIR := $(ROOT_DIR)external/
TOOLCHAIN_DIR := $(EXTERNAL_DIR)toolchain/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/
LIBGCC_DIR := $(EXTERNAL_DIR)toolchain/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/lib/gcc/arm-none-eabi/13.2.1/
TOOLS_DIR := $(ROOT_DIR)tools/
MK_DIR := $(TOOLS_DIR)mk/
export ROOT_DIR OUT_DIR OBJ_DIR EXTERNAL_DIR TOOLS_DIR MK_DIR TOOLCHAIN_DIR LIBGCC_DIR

TARGET ?= qemu-lm3s6965evb
PLATFORM_DIR := $(ROOT_DIR)platform/$(TARGET)/
include $(PLATFORM_DIR)platform.mk

ARCH_DIR := $(ROOT_DIR)arch/$(ARCH)
INCLUDE_DIR := $(ROOT_DIR)include/
MAIN_DIR := $(ROOT_DIR)main/
RUNTIME_DIR := $(ROOT_DIR)runtime/

export TARGET PLATFORM_DIR ARCH_DIR INCLUDE_DIR MAIN_DIR RUNTIME_DIR

all:
	if [ ! -d $(OUT_DIR) ]; then mkdir -p $(OUT_DIR); fi
	if [ ! -d $(OBJ_DIR) ]; then mkdir -p $(OBJ_DIR); fi
	make -C $(ARCH_DIR) -f Makefile
	make -C $(MAIN_DIR) -f Makefile
	make -C $(RUNTIME_DIR) -f Makefile
	make -C $(PLATFORM_DIR) -f Makefile

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(OUT_DIR)
