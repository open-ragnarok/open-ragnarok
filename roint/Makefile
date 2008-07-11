# $id$
CC=g++
RM=rm -f

CFLAGS=-Wall -g -I.

MAJOR_VER=0
MINOR_VER=1

TARGET=libroint

SHARED_SO=$(TARGET).so
SHARED_MAJOR=$(SHARED_SO).$(MAJOR_VER)

SHARED=$(SHARED_MAJOR).$(MINOR_VER)
STATIC=$(TARGET).a

SRC=act.cc gat.cc grf.cc ro.cc rsm.cc spr.cc constants.cc gnd.cc \
	object.cc ro_strings.cc rsw.cc \
	python/pyspr.cc python/pyrsm.cc

SHARED_OBJ=$(SRC:%.cc=%.lo)

STATIC_OBJ=$(SRC:%.cc=%.o)

all: shared static

shared: $(SHARED_OBJ)
	$(CC) -shared -Wl,-soname,$(SHARED_MAJOR) -o $(SHARED) $^

static: $(STATIC_OBJ)
	$(AR) rcs $(STATIC) $^

clean:
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
