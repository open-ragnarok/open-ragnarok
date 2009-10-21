# $Id$
CC = g++
RM = rm -f
AR = ar

CFLAGS+=-Iinclude -I/usr/include -I../roint/include -I../rogl/include -I../sdl_engine/include -I../ronet/include -DUNICODE
LDFLAGS=-L/usr/lib -lroint.dll -lsdle.dll -lronet.dll -lrogl.dll -lfreetype -lpng -ljpeg -lz -lSDL -lopengl32 -lglu32

PROJECTNAME=roengine
TARGET=lib$(PROJECTNAME)

SRC=cache_manager.cc file_manager.cc gl_object_cache.cc stdafx.cc camera.cc frustum.cc ro_engine.cc texturecache.cc \
	charobj.cc full_act.cc ro_object_cache.cc texturemanager.cc file_data.cc gl_object.cc spr_cache.cc \
	gui/button.cc gui/dialog.cc gui/label.cc gui/window.cc gui/buttonbar.cc gui/element.cc gui/list.cc \
	gui/chatwindow.cc gui/event.cc gui/progressbar.cc gui/desktop.cc gui/gui.cc gui/textinput.cc \
	objects/act_object.cc objects/homunobj.cc objects/rsm_object.cc objects/actor.cc objects/mobobj.cc \
	objects/rsw_object.cc objects/cube_object.cc objects/npcobj.cc

## OPTIONAL SWITCHES
DEBUG_BUILD=1

#LOGSYS
CFLAGS+=-DUSE_LOGSYS -I../logsys/include
LDFLAGS+=-L../logsys -llogsys

## END OF OPTIONAL SWITCHES
## Not much to see from here...

DEPDIR=.deps
OBJDIR=.objs

OBJ=$(SRC:%.cc=$(OBJDIR)/%.o)
DEPS=$(SRC:%.cc=$(DEPDIR)/%.P)

PREFIX=/usr/local
	
HOST_PLATFORM = ${shell $(CPP) -dumpmachine}

STATIC=$(TARGET).a

# debug flags
ifeq ($(DEBUG_BUILD),1)
CFLAGS+=-g -DDEBUG
else
CFLAGS+=-O2
endif

all: $(STATIC)

-include $(SRC:%.cc=$(DEPDIR)/%.P)

$(STATIC): $(OBJ)
	$(AR) rcs $(STATIC) $^ 
	
clean:
	$(RM) $(OBJ) $(STATIC)
	
distclean:	
	$(RM) $(OBJ) $(STATIC)
	$(RM) -R $(OBJDIR) $(DEPDIR)

# Dependencies
$(DEPDIR)/%.d: %.cc
	@if test ! -d $(DEPDIR);\
		then mkdir $(DEPDIR);\
		mkdir $(DEPDIR)/objects;\
		mkdir $(DEPDIR)/gui;\
	fi
	@$(CC) -M $(CFLAGS) -o $@ $<

$(DEPDIR)/%.P: $(DEPDIR)/%.d
	@cp $< $@; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	    -e '/^$$/ d' -e 's/$$/ :/' < $< >> $@; \
	$(RM) $<	

# Regular objects
$(OBJDIR)/%.o: %.cc $(DEPDIR)/%.P
	@if test ! -d $(OBJDIR);\
		then mkdir $(OBJDIR);\
		mkdir $(OBJDIR)/objects;\
		mkdir $(OBJDIR)/gui;\
	fi
	$(CC) $(CFLAGS) -c -o $@ $<

