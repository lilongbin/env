# define var
EMPTY:=/ #
SEPSYM :=:#
PATH_TMP := $(sort $(dir $(patsubst %.c,$(TMP_SRC_DIR)/%.c,$(TMP_SOURCES))))
vpath %.c $(subst $(EMPTY),$(SEPSYM), $(PATH_TMP))

# generate dependency
$(USR_OBJ_DIR)/%.d: %.c
    @echo Making $@ dependency ...
    $(CC) -MM $(CFLAGS) $< > $@.$$$$
    @sed "s,$(*F)\.o[: ]*,$@ $(@D)/&,g" $@.$$$$ > $@
    @rm -f $@.$$$$

# compile
$(USR_OBJ_DIR)/%.o: %.c
    @echo Making $@
    $(CC) $(COPTS) -o$@ $<
