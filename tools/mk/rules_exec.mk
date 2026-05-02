OBJS := $(ASRCS:%.S=$(OBJ_DIR)%.o)
OBJS += $(CSRCS:%.c=$(OBJ_DIR)%.o)

LIBS := $(wildcard $(OBJ_DIR)*.a)
OBJS_FROM_LIBS := $(foreach lib,$(LIBS), \
    $(addprefix $(dir $(lib)),$(shell $(AR) t $(lib))))

ifeq ($(CPU),cortex-m0plus)
LIBGCC := $(LIBGCC_DIR)thumb/v6-m/nofp/libgcc.a
endif

ELF := $(OUT_DIR)$(TARGET).elf
MAP := $(OUT_DIR)$(TARGET).map
DISASM := $(OUT_DIR)$(TARGET).disasm
BIN := $(OUT_DIR)$(TARGET).bin
LD_SCRIPT := $(TARGET).ld
LDFLAGS := -nostdlib -nostartfiles -e Reset_Handler
LDFLAGS += -T $(LD_SCRIPT) -Wl,-Map=$(MAP) -Wl,--gc-sections
ifeq ($(LTO),ON)
LDFLAGS += -flto
endif

all: $(BIN)

$(OBJ_DIR)%.o: %.S
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_DIR)%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(ELF): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(OBJS_FROM_LIBS) $(LIBGCC) -o $@
	$(OBJDUMP) -f -h -d $@ > $(DISASM)

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

DEPS := $(OBJS:%.o=%.d)
-include $(DEPS)
