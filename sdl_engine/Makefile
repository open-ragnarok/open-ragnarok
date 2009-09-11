# $Id$
TARGET=libsdle.a

SRC=sdl_engine.cc gui.cc frame.cc engine.cc texture.cc texture_bmp.cc texture_png.cc \
	image.cc image_bmp.cc blob.cc font.cc

DEPDIR = .deps
OBJDIR = obj
CC = g++
RM = rm -f
AR = ar

CFLAGS=-Wall -g
LDFLAGS=-lfreetype -lpng -lz

df = $(DEPDIR)/$(*F)
OBJ=$(SRC:%.cc=$(OBJDIR)/%.o)

CFLAGS+=-Iinclude -I../container/include
CFLAGS+=-DDEBUG

all: $(TARGET)

$(TARGET): $(OBJ)
	$(AR) -r $@ $(OBJ)
	$(AR) -t $@

$(OBJDIR)/%.o: %.cc
	@if test ! -d $(OBJDIR);\
		then mkdir $(OBJDIR);\
	fi
	@if test ! -d $(DEPDIR);\
		then mkdir $(DEPDIR);\
	fi
	@$(CC) -M $(CFLAGS) -o $(df).d $<; \
	cp $(df).d $(df).P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	    -e '/^$$/ d' -e 's/$$/ :/' < $(df).d >> $(df).P; \
	$(RM) $(df).d
	$(CC) -o $@ -c $< $(CFLAGS)

-include $(SRC:%.cc=$(DEPDIR)/%.P)

clean:
	$(RM) $(OBJ) $(TARGET)
	$(RM) $(SRC:%.cc=$(DEPDIR)/%.P)
	