CEXAMPLE_MOD_DIR := $(USERMOD_DIR)

# Add all C files to SRC_USERMOD.
SRC_USERMOD += $(CEXAMPLE_MOD_DIR)/module.c
SRC_USERMOD += $(CEXAMPLE_MOD_DIR)/mongoose.c
SRC_USERMOD += $(CEXAMPLE_MOD_DIR)/main.c

# add our module folder to include paths
CFLAGS_USERMOD += -I$(CEXAMPLE_MOD_DIR)
