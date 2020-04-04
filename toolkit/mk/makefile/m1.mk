# product.mk

USR_ROOT := XXX
include m1/m1.mk
include m2/m2.mk

OBJS =  $(M1_OBJS)
OBJS += $(M2_OBJS)

#m1.mk
TMP_SRC_DIR := $(USR_ROOT)
USR_OBJ_DIR := $(USR_ROOT)

TMP_SOURCES := $(M1_SRC_DIR)/XXX.C \
               $(M1_SRC_DIR)/XXY.C

M1_OBJS := $(patsubst %.c,$(USR_OBJ_DIR)/%.o, $(notdir $(TMP_SOURCES)))
M1_DEPS := $(patsubst %.c,$(USR_OBJ_DIR)/%.d, $(notdir $(TMP_SOURCES)))

include $(USR_ROOT)/makefile/rule.make

