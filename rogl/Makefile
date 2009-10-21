# $Id: Makefile 57 2009-07-21 00:09:26Z sergio $
CC=g++
RM=rm -f
CP=cp
MKDIR=mkdir -p

CFLAGS=-Wall -g -Iinclude -I../roint/include -I../sdl_engine/include
LDFLAGS=-L../sdl_engine/ -L../roint/ -lroint -lz -lsdle.dll -lopengl32 -lglu32

MAJOR_VER=0
MINOR_VER=1

DEPDIR=.deps
OBJDIR=.objs

PROJECTNAME=rogl
TARGET=lib$(PROJECTNAME)

HOST_PLATFORM = ${shell $(CPP) -dumpmachine}

SRC=actgl.cc rogl.cc rsmgl.cc rswgl.cc sprgl.cc

SHARED_OBJ=$(SRC:%.cc=$(OBJDIR)/%.lo)
STATIC_OBJ=$(SRC:%.cc=$(OBJDIR)/%.o)
DEPS=$(SRC:%.cc=$(DEPDIR)/%.P)

PREFIX=/usr/local

## OPTIONAL SWITCHES
DEBUG_BUILD=1

#LOGSYS
CFLAGS+=-DUSE_LOGSYS -I../logsys/include
LDFLAGS+=-L../logsys -llogsys

## END OF OPTIONAL SWITCHES

STATIC=$(TARGET).a

# debug flags
ifeq ($(DEBUG_BUILD),1)
CFLAGS+=-g -DDEBUG
else
CFLAGS+=-O2
endif

# In mingw we should use .dll, not .so.
ifeq ($(HOST_PLATFORM), mingw32)
SHARED_SO=$(PROJECTNAME).dll
SHARED_MAJOR=lib$(SHARED_SO).a
SHARED=$(SHARED_SO)

SHARED_LDFLAGS=-shared -Wl,--out-implib,$(SHARED_MAJOR)
SHARED_CFLAGS=$(CFLAGS)
else
SHARED_SO=$(TARGET).so
SHARED_MAJOR=$(SHARED_SO).$(MAJOR_VER)
SHARED=$(SHARED_MAJOR).$(MINOR_VER)

SHARED_LDFLAGS=-shared -Wl,-soname,$(SHARED_MAJOR)
SHARED_CFLAGS=-fPIC $(CFLAGS)
endif

all: static shared

-include $(SRC:%.cc=$(DEPDIR)/%.P)

install: $(SHARED) $(STATIC)
	$(CP) $(SHARED) $(PREFIX)/lib
	$(CP) $(STATIC) $(PREFIX)/lib
	ln -sf $(SHARED) $(PREFIX)/lib/$(SHARED_MAJOR)
	ln -sf $(SHARED) $(PREFIX)/lib/$(SHARED_SO)
	$(MKDIR) $(PREFIX)/include/rogl
	$(CP) include/rogl/* $(PREFIX)/include/rogl
	# TODO: LDCONFIG

uninstall:
	$(RM) $(PREFIX)/lib/$(SHARED)
	$(RM) $(PREFIX)/lib/$(STATIC)
	$(RM) $(PREFIX)/lib/$(SHARED_MAJOR)
	$(RM) -r $(PREFIX)/include/roint	

shared $(SHARED): $(SHARED_OBJ)
	$(CC) $(SHARED_LDFLAGS) -o $(SHARED) $^ $(LDFLAGS)

static $(STATIC): $(STATIC_OBJ)
	$(AR) rcs $(STATIC) $^ 
	
clean:
	$(RM) $(SHARED_OBJ) $(STATIC_OBJ)
	$(RM) $(SHARED) $(STATIC)
	$(RM) $(SHARED_MAJOR)
	
distclean:	
	$(RM) $(SHARED_OBJ) $(STATIC_OBJ)
	$(RM) $(SHARED) $(STATIC)
	$(RM) $(OBJDIR) $(DEPDIR)

# Dependencies
$(DEPDIR)/%.d: %.cc
	@if test ! -d $(DEPDIR);\
		then mkdir $(DEPDIR);\
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
	fi
	$(CC) $(SHARED_CFLAGS) -c -o $@ $<

# Regular objects
$(OBJDIR)/%.o: %.cc $(DEPDIR)/%.P
	@if test ! -d $(OBJDIR);\
		then mkdir $(OBJDIR);\
	fi
	$(CC) $(CFLAGS) -DROGL_STATIC -c -o $@ $<
