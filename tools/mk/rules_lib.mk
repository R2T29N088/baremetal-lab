OBJS := $(ASRCS:%.S=$(OBJ_DIR)%.o)
OBJS += $(CSRCS:%.c=$(OBJ_DIR)%.o)

DIR := $(notdir $(CURDIR))
LIB := $(OBJ_DIR)$(DIR).a

all: $(LIB)

$(OBJ_DIR)%.o: %.S
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_DIR)%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(LIB): $(OBJS)
	$(AR) rcs $@ $(OBJS)

DEPS := $(OBJS:%.o=%.d)
-include $(DEPS)
