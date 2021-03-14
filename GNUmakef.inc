# -*- makefile -*-
# Include file for GNU C++ build

.PHONY: all clean sourcelist

all: $(TARGET)

include $(TOP)/GNUmakef.def

# Messagebase formats
include $(TOP)/MSGBase.def

# Config files formats
include $(TOP)/Config.def

FOBJPATH=$(TOP)/$(OBJPATH)/$(PLATFORM)/$(TARGET)
FDEPPATH=$(TOP)/$(OBJPATH)/$(PLATFORM)/$(TARGET)
FLIBPATH=$(TOP)/$(LIBPATH)/$(PLATFORM)

.SUFFIXES: .c .cpp .all .rc .cxx

sourcelist: bld$(PLATFORM).inc

bld$(PLATFORM).inc: $(TARGET).all
	@echo making sourcelist
	@$(GREP) -w $(PLATFORM) $< \
		| $(SED) 's;^\([[:alnum:]_]*\)[ ]*\([[:alnum:]_]*\).*;SOURCES+=\1.\2;' > $@; [ -s $@ ] || rm -f $@

$(FOBJPATH)/%$(OBJEXT): %.cpp
	@echo building $(basename $<)$(OBJEXT)
	$(CXX) -c -MD $(CPPFLAGS) -o $@ $<
	@$(SHELL) -c "[ -s $(patsubst %.cpp,%.d,$<) ] &&					\
		echo -n $(FOBJPATH)/>$(FDEPPATH)/$(patsubst %.cpp,%.d,$<) &&		\
		$(CAT) $(patsubst %.cpp,%.d,$<)>>$(FDEPPATH)/$(patsubst %.cpp,%.d,$<) &&\
		rm $(patsubst %.cpp,%.d,$<) || true"

$(FOBJPATH)/%$(OBJEXT): %.cxx
	@echo building $(basename $<)$(OBJEXT)
	$(CXX) -c -MD $(CPPFLAGS) -o $@ $<
	@$(SHELL) -c "[ -s $(patsubst %.cxx,%.d,$<) ] &&					\
		echo -n $(FOBJPATH)/>$(FDEPPATH)/$(patsubst %.cxx,%.d,$<) &&		\
		$(CAT) $(patsubst %.cxx,%.d,$<)>>$(FDEPPATH)/$(patsubst %.cxx,%.d,$<) &&\
		rm $(patsubst %.cxx,%.d,$<) || true"

$(FOBJPATH)/%$(OBJEXT): %.c
	@echo building $(basename $<)$(OBJEXT)
	$(CC) -c -MD $(CFLAGS) -o $@ $<
	@$(SHELL) -c "[ -s $(patsubst %.c,%.d,$<) ] &&					\
		echo -n $(FOBJPATH)/>$(FDEPPATH)/$(patsubst %.c,%.d,$<) &&		\
		$(CAT) $(patsubst %.c,%.d,$<)>>$(FDEPPATH)/$(patsubst %.c,%.d,$<) &&	\
		rm $(patsubst %.c,%.d,$<) || true"


SOURCES=
ifeq ($(wildcard $(TOP)/$(OBJPATH)/$(PLATFORM)/source.ls?),$(TOP)/$(OBJPATH)/$(PLATFORM)/source.lst)
include bld$(PLATFORM).inc
endif

OBJS=$(addprefix $(FOBJPATH)/,$(patsubst %.c,%$(OBJEXT),$(patsubst %.cpp,%$(OBJEXT),$(patsubst %.cxx,%$(OBJEXT),$(filter %.c %.cpp %.cxx %.rc,$(SOURCES))))))
DEPS = $(wildcard $(FDEPPATH)/*.d)
ALLOBJS = $(wildcard $(FOBJPATH)/*.o)

clean:
	@echo cleaning...
	@-rm -f $(ALLOBJS) $(DEPS) bld$(PLATFORM).inc

ifneq ($(DEPS),)
-include $(DEPS)
endif
