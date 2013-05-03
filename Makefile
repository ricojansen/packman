#CFLAGS = -d4
CFLAGS = -O2 -Wall
#CFLAGS = -DUSE
CC=gcc

# LINK = dlib:amiga.lib
LINK = 

ROBJS = packman.o cleanup.o config.o kludge.o misc.o \
		pack.o parseaddress.o io.o modifypacket.o \
        changeaddr.o change.o
RSRCS = packman.c cleanup.c config.c kludge.c misc.c \
		pack.c parseaddress.c io.c modifypacket.c \
        changeaddr.c change.c

SOBJS = showpkt.o io.o misc.o printpacket.o config.o parseaddress.o
SSRCS = showpkt.c io.c misc.c printpacket.c config.c parseaddress.c

COBJS = check.o
CSRCS = check.c

REXE=packman
CEXE=check
SEXE=showpkt

all: $(REXE) $(CEXE)

$(REXE)  :   $(ROBJS)
	$(CC) $(ROBJS) $(LINK) -o $(REXE) $(CFLAGS)

clean:
	rm -f $(ROBJS) $(SOBJS) $(COBJS) $(REXE) $(CEXE)
