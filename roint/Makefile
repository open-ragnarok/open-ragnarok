# $Id$
CC=g++
RM=rm -f
CP=cp
MKDIR=mkdir -p

CFLAGS=-Wall -g -Iinclude

MAJOR_VER=0
MINOR_VER=1

TARGET=libroint

SHARED_SO=$(TARGET).so
SHARED_MAJOR=$(SHARED_SO).$(MAJOR_VER)

SHARED=$(SHARED_MAJOR).$(MINOR_VER)
STATIC=$(TARGET).a

SRC=act.cc gat.cc grf.cc ro.cc rsm.cc spr.cc constants.cc gnd.cc \
	ro_strings.cc rsw.cc common.cc ro_object.cc rsw_objects.cc \
	python/pyspr.cc python/pyrsm.cc python/pyact.cc python/pygrf.cc \
	python/pyrsw.cc python/pystrings.cc \
	tinyxml/tinyxml.cc tinyxml/tinyxmlerror.cc tinyxml/tinyxmlparser.cc

SHARED_OBJ=$(SRC:%.cc=%.lo)

STATIC_OBJ=$(SRC:%.cc=%.o)

PREFIX=/usr/local

HEADERS=act.h constants.h cp949.h gat.h gnd.h grf.h impdecl.h \
  object.h resource.h ro.h ro_strings.h rsm.h rsw.h spr.h \
  struct_io.h
  
LDFLAGS=-lz

all: static shared

install: $(SHARED) $(STATIC)
	$(CP) $(SHARED) $(PREFIX)/lib
	$(CP) $(STATIC) $(PREFIX)/lib
	ln -sf $(SHARED) $(PREFIX)/lib/$(SHARED_MAJOR)
	ln -sf $(SHARED) $(PREFIX)/lib/$(SHARED_SO)
	$(MKDIR) $(PREFIX)/include/roint
	$(CP) $(HEADERS) $(PREFIX)/include/roint
	# TODO: LDCONFIG

uninstall:
	$(RM) $(PREFIX)/lib/$(SHARED)
	$(RM) $(PREFIX)/lib/$(STATIC)
	$(RM) $(PREFIX)/lib/$(SHARED_MAJOR)
	$(RM) -r $(PREFIX)/include/roint	

shared $(SHARED): $(SHARED_OBJ)
	$(CC) -shared $(LDFLAGS) -Wl,-soname,$(SHARED_MAJOR) -o $(SHARED) $^

static $(STATIC): $(STATIC_OBJ)
	$(AR) rcs $(STATIC) $^

clean distclean:
	$(RM) $(SHARED_OBJ) $(STATIC_OBJ)
	$(RM) $(SHARED) $(STATIC)

python/%.lo: python/%.cc
	$(CC) -fPIC $(CFLAGS) -c -o $@ $<

%.lo: %.cc
	$(CC) -fPIC $(CFLAGS) -c -o $@ $<

python/%.o: python/%.cc
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cc
	$(CC) $(CFLAGS) -c -o $@ $<
