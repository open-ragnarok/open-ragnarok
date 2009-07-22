# $Id: Makefile 57 2009-07-21 00:09:26Z sergio $
CC=g++
RM=rm -f
CP=cp
MKDIR=mkdir -p

CFLAGS=-Wall -g -Iinclude -I../roint/include

MAJOR_VER=0
MINOR_VER=1

DEPDIR=.deps
OBJDIR=.objs

TARGET=libronet

SHARED_SO=$(TARGET).so
SHARED_MAJOR=$(SHARED_SO).$(MAJOR_VER)

SHARED=$(SHARED_MAJOR).$(MINOR_VER)
STATIC=$(TARGET).a

SRC=connection.cc mutex.cc net_tools.cc packet_factory.cc packet_vector.cc packet.cc \
	ronet.cc tcp_connection.cc ucbuffer.cc \
	packets/pkt_charcreate.cc packets/pkt_charcreated.cc packets/pkt_charcreateerror.cc \
	packets/pkt_charlist.cc packets/pkt_charlogin.cc packets/pkt_keepalive.cc \
	packets/pkt_login.cc packets/pkt_serverlist.cc

SHARED_OBJ=$(SRC:%.cc=$(OBJDIR)/%.lo)
STATIC_OBJ=$(SRC:%.cc=$(OBJDIR)/%.o)
DEPS=$(SRC:%.cc=$(DEPDIR)/%.P)

PREFIX=/usr/local

LDFLAGS=-lz

all: static

-include $(SRC:%.cc=$(DEPDIR)/%.P)

install: $(SHARED) $(STATIC)
	$(CP) $(SHARED) $(PREFIX)/lib
	$(CP) $(STATIC) $(PREFIX)/lib
	ln -sf $(SHARED) $(PREFIX)/lib/$(SHARED_MAJOR)
	ln -sf $(SHARED) $(PREFIX)/lib/$(SHARED_SO)
	$(MKDIR) $(PREFIX)/include/ronet
	$(CP) include/ronet/* $(PREFIX)/include/ronet
	# TODO: LDCONFIG

uninstall:
	$(RM) $(PREFIX)/lib/$(SHARED)
	$(RM) $(PREFIX)/lib/$(STATIC)
	$(RM) $(PREFIX)/lib/$(SHARED_MAJOR)
	$(RM) -r $(PREFIX)/include/ronet

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
		mkdir $(DEPDIR)/packets;\
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
		mkdir $(OBJDIR)/packets;\
	fi
	$(CC) -fPIC $(CFLAGS) -c -o $@ $<

# Regular objects
$(OBJDIR)/%.o: %.cc $(DEPDIR)/%.P
	@if test ! -d $(OBJDIR);\
		then mkdir $(OBJDIR);\
		mkdir $(OBJDIR)/packets;\
	fi
	$(CC) $(CFLAGS) -DROGL_STATIC -c -o $@ $<
