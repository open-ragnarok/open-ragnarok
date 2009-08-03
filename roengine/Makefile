# $Id: Makefile 57 2009-07-21 00:09:26Z sergio $
CC=g++
RM=rm -f
CP=cp
MKDIR=mkdir -p

CFLAGS=-Wall -g -Iinclude -I../roint/include -I../ronet/include -I../rogl/include

MAJOR_VER=0
MINOR_VER=1

DEPDIR=.deps
OBJDIR=.objs

TARGET=libroengine

SHARED_SO=$(TARGET).so
SHARED_MAJOR=$(SHARED_SO).$(MAJOR_VER)

SHARED=$(SHARED_MAJOR).$(MINOR_VER)
STATIC=$(TARGET).a

SRC=camera.cc file_data.cc file_manager.cc frustum.cc full_act.cc gl_object_cache.cc \
	gl_object.cc ro_engine.cc ro_object_cache.cc sdl_engine.cc texturemanager.cc \
	gui/button.cc gui/desktop.cc gui/element.cc gui/event.cc gui/glf_font.cc \
	gui/gui.cc gui/label.cc gui/list.cc gui/textinput.cc gui/window.cc gui/font.cc \
	objects/act_object.cc objects/cube_object.cc objects/rsm_object.cc \
	objects/rsw_object.cc

SHARED_OBJ=$(SRC:%.cc=$(OBJDIR)/%.lo)
STATIC_OBJ=$(SRC:%.cc=$(OBJDIR)/%.o)
DEPS=$(SRC:%.cc=$(DEPDIR)/%.P)

PREFIX=/usr/local

LDFLAGS=-lz

all: static

-include $(SRC:%.cc=$(DEPDIR)/%.P)

install: $(SHARED) $(STATIC)
	$(CP) $(SHARED) $(PREFIX)/lib
	$(CP) $(STATIC) $(PREFIX)/lib
	ln -sf $(SHARED) $(PREFIX)/lib/$(SHARED_MAJOR)
	ln -sf $(SHARED) $(PREFIX)/lib/$(SHARED_SO)
	$(MKDIR) $(PREFIX)/include/roengine
	$(CP) include/roengine/* $(PREFIX)/include/roengine
	# TODO: LDCONFIG

uninstall:
	$(RM) $(PREFIX)/lib/$(SHARED)
	$(RM) $(PREFIX)/lib/$(STATIC)
	$(RM) $(PREFIX)/lib/$(SHARED_MAJOR)
	$(RM) -r $(PREFIX)/include/roengine

shared $(SHARED): $(SHARED_OBJ)
	$(CC) -shared $(LDFLAGS) -Wl,-soname,$(SHARED_MAJOR) -o $(SHARED) $^

static $(STATIC): $(STATIC_OBJ)
	$(AR) rcs $(STATIC) $^ 
	
clean:
	$(RM) $(SHARED_OBJ) $(STATIC_OBJ)
	$(RM) $(SHARED) $(STATIC)
	
distclean:	
	$(RM) $(SHARED_OBJ) $(STATIC_OBJ)
	$(RM) $(SHARED) $(STATIC)
	$(RM) $(OBJDIR) $(DEPDIR)

# Dependencies
$(DEPDIR)/%.d: %.cc
	@if test ! -d $(DEPDIR);\
		then mkdir $(DEPDIR);\
		mkdir $(DEPDIR)/gui;\
		mkdir $(DEPDIR)/objects;\
	fi
	@$(CC) -M $(CFLAGS) -o $@ $<

$(DEPDIR)/%.P: $(DEPDIR)/%.d
	@cp $< $@; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	    -e '/^$$/ d' -e 's/$$/ :/' < $< >> $@; \
	$(RM) $<	

# Shared objects
$(OBJDIR)/%.lo: %.cc $(DEPDIR)/%.P
	@if test ! -d $(OBJDIR);\
		then mkdir $(OBJDIR);\
		mkdir $(OBJDIR)/gui;\
		mkdir $(OBJDIR)/objects;\
	fi
	$(CC) -fPIC $(CFLAGS) -c -o $@ $<

# Regular objects
$(OBJDIR)/%.o: %.cc $(DEPDIR)/%.P
	@if test ! -d $(OBJDIR);\
		then mkdir $(OBJDIR);\
		mkdir $(OBJDIR)/gui;\
		mkdir $(OBJDIR)/objects;\
	fi
	$(CC) $(CFLAGS) -DROGL_STATIC -c -o $@ $<
