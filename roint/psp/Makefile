OBJS = act.o gat.o gnd.o grf.o rsm.o rsw.o spr.o common.o constants.o ro.o ro_object.o ro_strings.o rsw_objects.o stdafx.o

CFLAGS = -I./include -I/usr/local/include -O2 -G0 -Wall -D__PSP__
CXXFLAGS = -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBS = -lstdc++ -lm -lz

all: libro.a

libro.a: $(OBJS) $(OBJA)
	$(AR) rc $@ $(OBJS) $(OBJA)
	-@ ($(RANLIB) $@ || true) >/dev/null 2>&1

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak 
