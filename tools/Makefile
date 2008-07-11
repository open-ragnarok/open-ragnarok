CC=g++
RM=rm -f

CFLAGS=-I../roint -Wall -g
LDFLAGS=-L../roint -lroint -lz

all: act_dump rsm_dump rsw_dump spr_export

act_dump: act_dump.cc
	$(CC) $(CFLAGS) -c -o act_dump.o $<
	$(CC) -o $@ act_dump.o $(LDFLAGS)

rsm_dump: rsm_dump.cc
	$(CC) $(CFLAGS) -c -o rsm_dump.o $<
	$(CC) -o $@ rsm_dump.o $(LDFLAGS)

rsw_dump: rsw_dump.cc
	$(CC) $(CFLAGS) -c -o rsw_dump.o $<
	$(CC) -o $@ rsw_dump.o $(LDFLAGS)

spr_export: spr_export.cc
	$(CC) $(CFLAGS) -c -o spr_export.o $<
	$(CC) -o $@ spr_export.o $(LDFLAGS)

clean:
	$(RM) act_dump act_dump.o rsm_dump rsm_dump.o rsw_dump rsw_dump.o spr_export spr_export.o

