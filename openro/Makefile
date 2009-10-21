# $Id: Makefile 57 2009-07-21 00:09:26Z sergio $
CC=g++
RM=rm -f
CP=cp
MKDIR=mkdir -p

CFLAGS=-I. -I../roint/include -I../ronet/include -I../rogl/include -I../roengine/include -I../sdl_engine/include -DUNICODE
LDFLAGS=-L../roint -L../rogl -L../ronet -L../roengine -L../sdl_engine \
	-lroengine -lrogl.dll -lronet.dll -lroint.dll -lsdle.dll -lopengl32 -lglu32 -lwsock32 -lz -lmingw32 -lSDLmain -lSDL -mwindows 
	
#DEBUG
CFLAGS+=-g -Wall
#LOGSYS
CFLAGS+=-DUSE_LOGSYS -I../logsys/include
LDFLAGS+=-L../logsys -llogsys
#RELEASE
#CFLAGS+=-O2

DEPDIR=.deps
OBJDIR=.objs

TARGET=openro.exe

SRC=main.cc openro.cc gui_act.cc \
	gui/desktop_char.cc gui/desktop_create.cc gui/desktop_login.cc gui/desktop_service.cc \
	gui/ro_desktop.cc gui/desktop_ingame.cc

OBJ=$(SRC:%.cc=$(OBJDIR)/%.o)
DEPS=$(SRC:%.cc=$(DEPDIR)/%.P)

PREFIX=/usr/local


all: $(TARGET)

-include $(SRC:%.cc=$(DEPDIR)/%.P)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) 
	
clean:
	$(RM) $(OBJ) $(TARGET)
	
distclean:	
	$(RM) $(OBJ) $(TARGET)
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

# Regular objects
$(OBJDIR)/%.o: %.cc $(DEPDIR)/%.P
	@if test ! -d $(OBJDIR);\
		then mkdir $(OBJDIR);\
		mkdir $(OBJDIR)/gui;\
		mkdir $(OBJDIR)/objects;\
	fi
	$(CC) $(CFLAGS) -c -o $@ $<
