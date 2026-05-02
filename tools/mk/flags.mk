# COMMON
CFLAGS := -ffunction-sections -fdata-sections \
			-fno-tree-loop-distribute-patterns \
			-O2 \
			-g3 \
			-Wextra -Wall -MMD -MP
# LTO
ifeq ($(LTO),ON)
CFLAGS += -flto
endif

# CPU
ifeq ($(CPU),cortex-m0plus)
CPUFLAGS := -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft
endif
CFLAGS += $(CPUFLAGS)

# CMSIS
CMSIS_ROOT_DIR := $(ROOT_DIR)external/cmsis/
CMSIS_CORE := $(CMSIS_ROOT_DIR)CMSIS/Core/Include/
CFLAGS += -I $(CMSIS_CORE)

# include
CFLAGS += -I $(INCLUDE_DIR)
CFLAGS += -I $(INCLUDE_DIR)platform/$(TARGET)/
