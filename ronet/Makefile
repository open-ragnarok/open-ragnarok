# $Id: Makefile 57 2009-07-21 00:09:26Z sergio $
CC=g++
RM=rm -f
CP=cp
MKDIR=mkdir -p

CFLAGS=-Wall -g -Iinclude -I../roint/include
LDFLAGS=-lz

MAJOR_VER=0
MINOR_VER=1

DEPDIR=.deps
OBJDIR=.objs

PROJECTNAME=ronet
TARGET=lib$(PROJECTNAME)

HOST_PLATFORM = ${shell $(CPP) -dumpmachine}

SRC=connection.cc log.cc mutex.cc net_tools.cc packet.cc packet_factory.cc packet_vector.cc \
	ronet.cc tcp_connection.cc ucbuffer.cc \
	packets/generic_packets.cc packets/pkt_actionrequest.cc packets/pkt_actoraction.cc packets/pkt_actordisplay.cc \
	packets/pkt_actormove.cc packets/pkt_actorspawn.cc packets/pkt_actorwalking.cc packets/pkt_attackrange.cc \
	packets/pkt_authfailed.cc packets/pkt_charcreate.cc packets/pkt_charcreated.cc packets/pkt_charcreateerror.cc \
	packets/pkt_charleavescreen.cc packets/pkt_charlist.cc packets/pkt_charlogin.cc packets/pkt_charposition.cc \
	packets/pkt_charselect.cc packets/pkt_displaystat.cc packets/pkt_gmbroad.cc packets/pkt_guildinforequest.cc \
	packets/pkt_guildmessage.cc packets/pkt_hotkeylist.cc packets/pkt_hpupdateparty.cc packets/pkt_inventoryitems.cc \
	packets/pkt_inventoryitemsstackable.cc packets/pkt_keepalive.cc packets/pkt_keepalivemap.cc packets/pkt_login.cc \
	packets/pkt_loginerror.cc packets/pkt_mapacctsend.cc packets/pkt_mapchange.cc packets/pkt_maploaded.cc packets/pkt_maplogin.cc \
	packets/pkt_maploginsuccess.cc packets/pkt_mapmove.cc packets/pkt_mapmoveok.cc packets/pkt_npccancel.cc packets/pkt_npccontinue.cc \
	packets/pkt_otherspeech.cc packets/pkt_ownspeech.cc packets/pkt_playerequip.cc packets/pkt_recvnpctalk.cc \
	packets/pkt_recvnpctalknext.cc packets/pkt_recvnpctalkresponses.cc packets/pkt_sendnpcnumber.cc packets/pkt_sendnpcresponse.cc \
	packets/pkt_sendnpctext.cc packets/pkt_serverlist.cc packets/pkt_servertick.cc packets/pkt_skilllist.cc packets/pkt_statinfo.cc \
	packets/pkt_statinfo2.cc packets/pkt_statsinfo.cc packets/pkt_talk.cc packets/pkt_updatestatus.cc packets/pkt_zenyexp.cc
	
SHARED_OBJ=$(SRC:%.cc=$(OBJDIR)/%.lo)
STATIC_OBJ=$(SRC:%.cc=$(OBJDIR)/%.o)
DEPS=$(SRC:%.cc=$(DEPDIR)/%.P)

PREFIX=/usr/local

## OPTIONAL SWITCHES
DEBUG_BUILD=1

#LOGSYS
CFLAGS+=-DUSE_LOGSYS -I../logsys/include
LDFLAGS+=-L../logsys -llogsys

## END OF OPTIONAL SWITCHES

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
LDFLAGS+=-lwsock32
else
SHARED_SO=$(TARGET).so
SHARED_MAJOR=$(SHARED_SO).$(MAJOR_VER)
SHARED=$(SHARED_MAJOR).$(MINOR_VER)

SHARED_LDFLAGS=-shared -Wl,-soname,$(SHARED_MAJOR)
SHARED_CFLAGS=-fPIC $(CFLAGS)
endif

all: static shared

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
	$(RM) $(SHARED_MAJOR)

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
	$(CC) $(SHARED_CFLAGS) -c -o $@ $<

# Regular objects
$(OBJDIR)/%.o: %.cc $(DEPDIR)/%.P
	@if test ! -d $(OBJDIR);\
		then mkdir $(OBJDIR);\
		mkdir $(OBJDIR)/packets;\
	fi
	$(CC) $(CFLAGS) -DRONET_STATIC -c -o $@ $<
