# NOD Makefile.
#

CC=       gcc
RM=       rm -rf
AR=       ar rc
RANLIB=   ranlib

STD=      c89
CFLAGS=   -Wall -Werror
LDFLAGS=

CFLAGS+=  $(MYCFLAGS) -std=$(STD)
LDFLAGS+= $(MYLDFLAGS)

CORE-SRC:=  nod.c lexer.c
CORE-OBJ:=  $(CORE-SRC:.c=.o)
CORE-TRG:=  libnod.a

SRC:=  main.c
OBJ:=  $(SRC:.c=.o)
TRG:=  nod
DYN:=

DEP:=  .depend

ifeq ($(P),1)
  CFLAGS+= -ansi -pedantic
endif

all: release
build: $(TRG) $(CORE-TRG)

release: build
release: CFLAGS+= -O2

debug: build
debug: CFLAGS+=  -g -DNOD_DEBUG_
debug: LDFLAGS+= -g

-include $(DEP)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TRG): $(OBJ) $(CORE-OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(DYN)

$(CORE-TRG): $(CORE-OBJ)
	$(AR) $@ $?
	$(RANLIB) $@

depend:
	echo "# Auto-generated with 'make depend'. Do not modify." > $(DEP)
	for f in $(SRC) $(CORE-SRC) ; do \
	  o=$${f%%.c}.o ; \
	  $(CC) -MM $$f -MT $$o >> $(DEP) ; \
	done

clean:
	$(RM) $(TRG) $(CORE-TRG) $(OBJ) $(CORE-OBJ)

help:
	@echo "NOD Makefile"
	@echo "  build       Build with default flags."
	@echo "  release     Build for release (default)."
	@echo "  debug       Build for debugging."
	@echo "Utilities:"
	@echo "  depend      Generate $(DEP) to optimise build."
	@echo "  clean       Remove generated files."
	@echo "  help        Show this help."
	@echo "Customisations:"
	@echo "  CC=...      C compiler (def.: $(CC))."
	@echo "  RM=...      Remove command (def.: $(RM))"
	@echo "  STD=...     C standard to use (def.: $(STD))"
	@echo "  MYCFLAGS=...    Custom C compiler flags."
	@echo "  MYLDFLAGS=...   Custom linker flags."
	@echo "  P=1         Force standard compliance."

.PHONY: all build release debug depend clean help
