# nod's Makefile.
#

CC=       gcc
RM=       rm -rf
STD=      gnu99

CFLAGS=   -Wall -Wpedantic

CFLAGS+=  $(MYCFLAGS) -std=$(STD)
LDFLAGS+= $(MYLDFLAGS)

SRC= lexer.c main.c parser.c util.c
OBJ= $(SRC:.c=.o)
TRG= nod

default: release
build: $(TRG)

release:  CFLAGS+=  -O2
release:  build
debug:    CFLAGS+=  -g -DDEBUG_
debug:    LDFLAGS+= -g
debug:    build

$(TRG): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(TRG) $(OBJ)

.PHONY: default build release debug clean
