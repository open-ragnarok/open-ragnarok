# $Id$
CC = g++
RM = rm -f
AR = ar

CFLAGS+=-Iinclude -I/usr/include -DUNICODE
LDFLAGS=-L/usr/lib -lfreetype -lpng -ljpeg -lz -lSDL -lopengl32 -lglu32

PROJECTNAME=sdle
TARGET=lib$(PROJECTNAME)

SRC=blob.cc console.cc engine.cc font.cc frame.cc ft_font.cc glf_font.cc gui.cc image.cc image_bmp.cc \
	sdl_engine.cc sdle_string.cc texture.cc texture_jpeg.cc texture_png.cc vertex.cc

## OPTIONAL SWITCHES
DEBUG_BUILD=1

#LOGSYS
CFLAGS+=-DUSE_LOGSYS -I../logsys/include
LDFLAGS+=-L../logsys -llogsys

## END OF OPTIONAL SWITCHES
## Not much to see from here...

DEPDIR=.deps
OBJDIR=.objs

SHARED_OBJ=$(SRC:%.cc=$(OBJDIR)/%.lo)
STATIC_OBJ=$(SRC:%.cc=$(OBJDIR)/%.o)
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
	$(CC) $(SHARED_CFLAGS) -DSDLENGINE_DLL -c -o $@ $<

# Regular objects
$(OBJDIR)/%.o: %.cc $(DEPDIR)/%.P
	@if test ! -d $(OBJDIR);\
		then mkdir $(OBJDIR);\
	fi
	$(CC) $(CFLAGS) -c -o $@ $<

