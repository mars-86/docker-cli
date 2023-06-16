IDIR=inc
SDIR=src
EXIDIR=../common
EXSDIR=../common/src
MAINDIR=./
CC=gcc
CFLAGS=-I $(IDIR)

OUTDIR=bin
ODIR=obj
LDIR=lib

LIBS=-lws2_32 -lpthread

_DEPS = init.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_EXDEPS = common.h
EXDEPS = $(patsubst %,$(EXIDIR)/%,$(_EXDEPS))

_OBJ = main.o init.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_EXTOBJ = cmd_line.o error.o process.o
EXTOBJ = $(patsubst %,$(ODIR)/%,$(_EXTOBJ))

$(ODIR)/%.o: $(MAINDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(EXSDIR)/%.c $(EXDEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

install: $(OUTDIR) $(ODIR) $(OUTDIR)/dockerd

$(OUTDIR):
	mkdir $@

$(ODIR):
	mkdir $@

$(OUTDIR)/dockerd: $(OBJ) $(EXTOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
