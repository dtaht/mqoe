LIBS=-I/usr/local/include -lcgic
DEBUG=-DDEBUG 
#DEBUG=-DDEBUG -DDUMMY_SERVER -DDUMMY_CLIENT
parse=shaped_parse.c
PROGS=shaped_parse
HTMLDIR=/var/www/mqoe
CGIDIR=/usr/lib/cgi-bin
BINDIR=$(HOME)/bin

.PHONY: install

shaped_parse:
	$(CC) $(CFLAGS) $(parse) -lcsv -o parse

all: $(PROGS)  
	$(CC) $(parse) -lcsv -o $(PROGS)

install: 
	
install-stripped: 

clean:

etags:	
	find src -type f \( -name \*.c -o -name \*.h \) | xargs etags --append
