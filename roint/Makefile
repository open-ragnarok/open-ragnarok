# $Id$
CC=g++
RM=rm -f
CP=cp
MKDIR=mkdir -p

CFLAGS=-Wall -g -Iinclude
LDFLAGS=-lz

MAJOR_VER=0
MINOR_VER=1

DEPDIR=.deps
OBJDIR=.objs

TARGET=libroint

SHARED_SO=$(TARGET).so
SHARED_MAJOR=$(SHARED_SO).$(MAJOR_VER)

SHARED=$(SHARED_MAJOR).$(MINOR_VER)
STATIC=$(TARGET).a

SRC=act.cc gat.cc grf.cc ro.cc rsm.cc spr.cc gnd.cc pal.cc constants.cc \
	ro_strings.cc rsw.cc common.cc ro_object.cc rsw_objects.cc log.cc \
	python/pyspr.cc python/pyrsm.cc python/pyact.cc python/pygrf.cc \
	python/pyrsw.cc python/pystrings.cc \
	tinyxml/tinyxml.cc tinyxml/tinyxmlerror.cc tinyxml/tinyxmlparser.cc

SHARED_OBJ=$(SRC:%.cc=$(OBJDIR)/%.lo)
STATIC_OBJ=$(SRC:%.cc=$(OBJDIR)/%.o)
DEPS=$(SRC:%.cc=$(DEPDIR)/%.P)

PREFIX=/usr/local
  
## OPTIONAL SWITCHES

#DEBUG
CFLAGS+=-DDEBUG

#LOGSYS
CFLAGS+=-DUSE_LOGSYS -I../logsys/include
LDFLAGS+=-L../logsys -llogsys

## END OF OPTIONAL SWITCHES

all: static

-include $(SRC:%.cc=$(DEPDIR)/%.P)

install: $(SHARED) $(STATIC)
	$(CP) $(SHARED) $(PREFIX)/lib
	$(CP) $(STATIC) $(PREFIX)/lib
	ln -sf $(SHARED) $(PREFIX)/lib/$(SHARED_MAJOR)
	ln -sf $(SHARED) $(PREFIX)/lib/$(SHARED_SO)
	
	$(MKDIR) $(PREFIX)/include/ro
	$(MKDIR) $(PREFIX)/include/tinyxml
	$(MKDIR) $(PREFIX)/include/ro/python
	$(MKDIR) $(PREFIX)/include/ro/types
	
	$(CP) include/ro/* $(PREFIX)/include/ro
	$(CP) include/tinyxml/* $(PREFIX)/include/tinyxml
	$(CP) include/ro/python/* $(PREFIX)/include/ro/python
	$(CP) include/ro/types/* $(PREFIX)/include/ro/types
	# TODO: LDCONFIG

uninstall:
	$(RM) $(PREFIX)/lib/$(SHARED)
	$(RM) $(PREFIX)/lib/$(STATIC)
	$(RM) $(PREFIX)/lib/$(SHARED_MAJOR)
	$(RM) -r $(PREFIX)/include/ro	

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
		mkdir $(DEPDIR)/python;\
		mkdir $(DEPDIR)/tinyxml;\
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
		mkdir $(OBJDIR)/python;\
		mkdir $(OBJDIR)/tinyxml;\
	fi
	$(CC) -fPIC $(CFLAGS) -c -o $@ $<

# Regular objects
$(OBJDIR)/%.o: %.cc $(DEPDIR)/%.P
	@if test ! -d $(OBJDIR);\
		then mkdir $(OBJDIR);\
		mkdir $(OBJDIR)/python;\
		mkdir $(OBJDIR)/tinyxml;\
	fi
	$(CC) $(CFLAGS) -DROINT_STATIC -c -o $@ $<
