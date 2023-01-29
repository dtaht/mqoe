PREFIX = /usr/local
MANDIR = $(PREFIX)/share/man
CDEBUGFLAGS = -Os -g -Wall
DEFINES = $(PLATFORM_DEFINES)
INCLUDES = -Iinclude/ -I..
CFLAGS = $(CDEBUGFLAGS) $(DEFINES) $(EXTRA_DEFINES) $(INCLUDES) 
LDLIBS = -lrt util2.o
LIBS=-I/usr/local/include -lcgic
DEBUG=-DDEBUG 
#DEBUG=-DDEBUG -DDUMMY_SERVER -DDUMMY_CLIENT
parse=shaped_parse.c
PROGS=mqoe shaped_parse
HTMLDIR=/var/www/mqoe
CGIDIR=/usr/lib/cgi-bin
BINDIR=$(HOME)/bin
VERSION=include/version.h

SRCS = mqoe.c netlink.c util2.c config_parse.c klingon.c
OBJS = mqoe.o netlink.o util2.o config_parse.o klingon.o

.PHONY: install

mqoe: $(OBJS) $(VERSION)
	$(CC) $(CFLAGS) $(LDFLAGS) -o mqoe $(OBJS) $(LDLIBS)

shaped_parse:
	$(CC) $(CFLAGS) $(parse) -lcsv -o parse

all: $(PROGS)
	$(CC) $(parse) -lcsv -o $(PROGS)

install:

install-stripped:

clean:
	-rm -f $(PROGS) version.h *.o *~ core TAGS gmon.out

etags:
	find . -type f \( -name \*.c -o -name \*.h \) | xargs etags --append

include/version.h: $(SRCS)
	./generate-version.sh > include/version.h

.SUFFIXES: .man .html

.man.html:
	mandoc -Thtml $< > $@

.PHONY: all install install.minimal uninstall clean
