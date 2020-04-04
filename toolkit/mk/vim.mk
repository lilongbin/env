OBJS := obj_vim
# OBJS := obj_toolkit

DEBUGON = 
ifeq ($(DEBUGON),)
    DEBUG = @echo >/dev/null
else
    DEBUG = @echo
endif

all: $(OBJS)
	$(DEBUG) "building: $(OBJS)"
	$(warning "building: $(OBJS)")

obj_vim:
	echo "" > ~/.viminfo
	cd ${HOME}; tar -zcvf vim_cfg_bk.tgz .vim* --exclude=*.pyc

obj_toolkit:
	cd ${HOME}; tar -zcvf toolkit.tgz toolkit --exclude=*.pyc

.PHONY: clean
clean:
	-rm -v vim_cfg_bk.tgz toolkit.tgz

