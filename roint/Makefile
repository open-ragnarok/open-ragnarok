# $id$
CC=gcc
RM=rm -f

MAJOR_VER=0
MINOR_VER=1

SHARED_SO=roint.so
SHARED_MAJOR=$(SHARED_SO).$(MAJOR_VER)

SHARED=$(SHARED_MAJOR).$(MINOR_VER)
STATIC=roint.a

SRC=act.cc gat.cc grf.cc ro.cc rsm.cc spr.cc constants.cc gnd.cc object.cc ro_strings.cc rsw.cc

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

%.lo: %.cc
	$(CC) -fPIC -g -c -Wall -o $@ $<

%.o: %.cc
	$(CC) -g -c -Wall -o $@ $<
