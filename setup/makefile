IDIR=installer/inc
SDIR=installer/src
MAIN=./
CC=gcc
CFLAGS=-I $(IDIR)

OUTDIR=bin
ODIR=obj
LDIR =../lib

_DEPS =
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(MAIN)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

install: $(OUTDIR) $(ODIR) $(OUTDIR)/setup

$(OUTDIR):
	mkdir $@

$(ODIR):
	mkdir $@

$(OUTDIR)/setup: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
