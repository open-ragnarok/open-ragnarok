# $Id: Makefile 57 2009-07-21 00:09:26Z sergio $
CC=g++
RM=rm -f
CP=cp
MKDIR=mkdir -p

CFLAGS=-Wall -g -I. -I../roint/include -I../ronet/include -I../rogl/include -I../roengine/include

DEPDIR=.deps
OBJDIR=.objs

TARGET=openro.exe

SRC=main.cc openro.cc gui_act.cc \
	gui/desktop_char.cc gui/desktop_create.cc gui/desktop_login.cc gui/desktop_service.cc \
	gui/ro_desktop.cc

OBJ=$(SRC:%.cc=$(OBJDIR)/%.o)
DEPS=$(SRC:%.cc=$(DEPDIR)/%.P)

PREFIX=/usr/local

LDFLAGS=-L../roint -L../rogl -L../ronet -L../roengine \
	-lroengine -lrogl -lronet -lroint -lopengl32 -lglu32 -lwsock32 -lz -lmingw32 -lSDLmain -lSDL -mwindows 

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
	$(CC) $(CFLAGS) -DROINT_STATIC -c -o $@ $<
