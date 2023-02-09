IDIR=inc
SDIR=src
MAINDIR=./
CC=gcc
CFLAGS=-I $(IDIR)

OUTDIR=bin
ODIR=obj
LDIR=lib

_DEPS =
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(MAINDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

install: $(OUTDIR) $(ODIR) $(OUTDIR)/docker

$(OUTDIR):
	mkdir $@

$(ODIR):
	mkdir $@

$(OUTDIR)/docker: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
